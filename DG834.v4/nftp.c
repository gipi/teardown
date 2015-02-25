/*****************************************************************************\
*                                                                             *
*  nftp                                                                       *
*                                                                             *
*  Upload a firmware image to a bricked Netgear router using raw Ethernet     *
*  frames.                                                                    *
*                                                                             *
*  Only tested with a DG834Gv4. Don't blame me if this breaks your router!    *
*                                                                             *
*  First version written by matteo (aka rootkit). I tried upslug2 but it      *
*  didn't work and eventually uploaded something that still didn't make the   *
*  router boot.                                                               *
*                                                                             *
*  After looking at a packet dump of a proper upgrade done using the Windows  *
*  tool, I figured enhancing this program would be easier than trying to      *
*  understand how upslug2 is supposed to work. I don't like C++.              *
*                                                                             *
*  This code isn't pretty, but I hacked it together quickly to get the job    *
*  done. Hope it'll help someone.                                             *
*                                                                             *
*  Wilmer van der Gaast. <wilmer@gaast.net>                                   *
*                                                                             *
\*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
                     
#define NFTP_PROBE_LEN		0x40
#define NFTP_MAX_PKT_LEN	1600
#define NFTP_PROBE_RESP_LEN	0x66
#define ETH_P_NFTP		0x8888

#define NFTP_BLOCK_SIZE		1024
#define IMG_VERIFY_BUF		65536
#define IMG_VERIFY_STRING	"sercomm"

typedef enum {
	NFTP_TYPE_HWINFO = 0,
	NFTP_TYPE_UPGRADESTART = 1,
	NFTP_TYPE_UPGRADEDATA = 2,
	NFTP_TYPE_REBOOT = 3,
	NFTP_TYPE_UPGRADEVERIFY = 4,
} nftp_type_t;

#define DEBUG

#ifdef DEBUG
	#define D(x, ...) fprintf(stderr, x"\n", __VA_ARGS__)
#else
	#define D(...)
#endif

void usage(char *arg0)
{
	fprintf(stderr, "Usage: %s -u/-v iface file.img\n"
			"Example:\n\t %s -u eth0 firmware.img\n", arg0, arg0);
	exit(1);
}

int sockfd;
unsigned char src_mac[ETH_ALEN];
unsigned char dst_mac[ETH_ALEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
struct sockaddr_ll socket_address;
unsigned char pkt_buffer[NFTP_MAX_PKT_LEN];
unsigned char *etherhead = pkt_buffer;
unsigned char *data = pkt_buffer + 14;
uint16_t *nftp_type = (uint16_t*) (pkt_buffer + 14);
uint16_t *nftp_sequence = (uint16_t*) (pkt_buffer + 16);
uint16_t *nftp_offset = (uint16_t*) (pkt_buffer + 18);
uint16_t *nftp_chunk = (uint16_t*) (pkt_buffer + 20);
uint16_t *nftp_payload_len = (uint16_t*) (pkt_buffer + 22);
unsigned char *nftp_payload = pkt_buffer + 24;
int send_pkt_len;
int recv_pkt_len;

int nftp_send()
{
	int st;

	/*set the frame header*/
	memcpy((void*)pkt_buffer, (void*)dst_mac, ETH_ALEN);
	memcpy((void*)(pkt_buffer+ETH_ALEN), (void*)src_mac, ETH_ALEN);
	((struct ethhdr*)etherhead)->h_proto = ETH_P_NFTP;

	socket_address.sll_halen = ETH_ALEN;
	memcpy(socket_address.sll_addr, dst_mac, ETH_ALEN);

	(*nftp_sequence)++;
	send_pkt_len = (*nftp_payload_len) + 24;

	st = sendto(sockfd, pkt_buffer, send_pkt_len, 0,
		(struct sockaddr*)&socket_address, sizeof(socket_address));

	if (st == -1) {
		perror("sendto");
		/* Rules of proper programming don't apply in a hack like this. :-P */
		exit(1);
	}
}

int nftp_recv()
{
	uint16_t st;
	
	do {
		recv_pkt_len = recvfrom(sockfd, pkt_buffer, NFTP_PROBE_RESP_LEN, 0, NULL, NULL);
		if (recv_pkt_len == -1) {
			perror("recvfrom");
			return 1;
		}
	} while (((struct ethhdr*)etherhead)->h_proto != ETH_P_NFTP);
	
	if (*nftp_payload_len == 2) {
		st = *(uint16_t*)(nftp_payload);
	} else {
		st = 0;
	}
	
	return st;
}

int nftp_sendrecv()
{
	uint16_t sequence;
	int st;
	
	nftp_send();
	sequence = *nftp_sequence;
	
	while (1) {
		st = nftp_recv();
		
		if (*nftp_sequence != sequence) {
			D("Received unexpected packet seq=%d (expected %d)",
			  *nftp_sequence, sequence);
			continue;
		} else if (st != 0) {
			fprintf(stderr, "Received non-0 response from router, aborting.\n");
			exit(1);
		}
		
		return st;
	}
}

int nftp_sendfile(int imgfd, int imgsize, char *status)
{
	int imgoffset;
	
	lseek(imgfd, 0, SEEK_SET);
	*nftp_chunk = imgoffset = 0;
	while (read(imgfd, nftp_payload, NFTP_BLOCK_SIZE) == NFTP_BLOCK_SIZE) {
		*nftp_payload_len = NFTP_BLOCK_SIZE;
		nftp_sendrecv();
		*nftp_chunk += (NFTP_BLOCK_SIZE >> 4);
		imgoffset += NFTP_BLOCK_SIZE;
		fprintf( stderr, "\r%s: %10d/%d bytes",status, imgoffset, imgsize );
	}
}

int main(int argc, char *argv[])
{
	int send_result = 0, res, imgfd, upgrade = 0;
	struct stat imginfo;
	struct ifreq iface;
	char *buf, *s;

	if(argc < 3)
		usage(argv[0]);

	if (strcmp(argv[1], "-u") == 0) {
		upgrade = 1;
	} else if (strcmp(argv[1], "-v") != 0) {
		usage(argv[0]);
	}

	/* Initialize the raw socket stuff. Most of this can be reused during
	   the whole session. */
	sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (sockfd == -1) {
		if(geteuid() != 0) {
			fprintf(stderr, "You should probably run this program as root.\n");
		}
		perror("socket");
		return 1;
	}
	seteuid(getuid());
	strncpy(iface.ifr_name, argv[2], IFNAMSIZ);

	imgfd = open(argv[3], O_RDONLY);
	fstat(imgfd, &imginfo);
	if (imginfo.st_size % NFTP_BLOCK_SIZE) {
		fprintf(stderr, "File size should be a multiple of %d.\n", NFTP_BLOCK_SIZE);
		return 1;
	}

	res = ioctl(sockfd, SIOCGIFHWADDR, &iface);
	if(res < 0){
		perror("ioctl");
		exit(1);
	}

	/*our MAC address*/
	memcpy(src_mac, iface.ifr_hwaddr.sa_data, ETH_ALEN);
	D("Sending frame on %s (%x:%x:%x:%x:%x:%x)", iface.ifr_name,
	  src_mac[0], src_mac[1], src_mac[2], src_mac[3], src_mac[4], src_mac[5]);

	/*RAW communication*/
	socket_address.sll_family   = PF_PACKET;
	/*we don't use a protocoll above ethernet layer
	*   ->just use anything here*/
	socket_address.sll_protocol = htons(ETH_P_NFTP);

	/*index of the network device
	* see full code later how to retrieve it*/
	res = ioctl(sockfd, SIOCGIFINDEX, &iface);
	if(res < 0){
		perror("ioctl");
		exit(1);
	}
	socket_address.sll_ifindex  =iface.ifr_ifindex;

	/* ARP hardware identifier is ethernet */
	socket_address.sll_hatype   = ARPHRD_ETHER;

	/* target is another host */
	socket_address.sll_pkttype  = PACKET_OTHERHOST;

	*nftp_type = NFTP_TYPE_HWINFO;
	send_pkt_len = NFTP_PROBE_LEN;

	nftp_send();
	nftp_recv();

	/* Now we know where to talk to, stop broadcasting! */
	memcpy(dst_mac, pkt_buffer + ETH_ALEN, ETH_ALEN);
	D("Found a router at %x:%x:%x:%x:%x:%x", dst_mac[0], dst_mac[1], dst_mac[2], dst_mac[3], dst_mac[4], dst_mac[5]);
	D("Router is a %s", pkt_buffer + 0x1C);
	D("Current version is %x%x", pkt_buffer[0x4A], pkt_buffer[0x4B]);
	D("Max upgrade size is %d kb", (pkt_buffer[0x16] | (pkt_buffer[0x17] << 8) | (pkt_buffer[0x18] << 16)) - 20);

	/* Some kind of verification of the firmware image. This really is
	   just some guess work based on what I saw in upslug2 and the images
	   for my router (not compatible with upslug2). */
	buf = malloc(IMG_VERIFY_BUF);
	lseek(imgfd, -IMG_VERIFY_BUF, SEEK_END);
	read(imgfd, buf, IMG_VERIFY_BUF);
	s = (char*) memmem(buf, IMG_VERIFY_BUF, nftp_payload, (size_t) (*nftp_payload_len));
	if (s == NULL ||
	    strncasecmp(s - strlen(IMG_VERIFY_STRING), IMG_VERIFY_STRING, strlen(IMG_VERIFY_STRING)) != 0 ||
	    strncasecmp(s + *nftp_payload_len, IMG_VERIFY_STRING, strlen(IMG_VERIFY_STRING)) != 0) {
		fprintf(stderr, "Could not find \"magic hardware header\" in this image.\n"
		                "Uploading this is not recommended but press Enter if you wish to continue.\n");
		read(0, buf, 1);
	}
	
	*nftp_type = NFTP_TYPE_UPGRADESTART;
	*nftp_offset = 0;
	*nftp_chunk = 0;
	*nftp_payload_len = 0;

	nftp_sendrecv();

	if (upgrade) {
		/* When the first packet comes in, the router will start
		   erasing flash before it sends an ACK. Keep the user
		   updated in the meantime. */
		printf("Erasing flash, this will take around ten seconds...\n");
		*nftp_type = NFTP_TYPE_UPGRADEDATA;
		nftp_sendfile(imgfd, imginfo.st_size, "Upgrading");
		printf("\nUpload completed, will now verify:\n");
	}
	
	*nftp_type = NFTP_TYPE_UPGRADEVERIFY;
	nftp_sendfile(imgfd, imginfo.st_size, "Verifying");

	if (upgrade) {
		*nftp_type = NFTP_TYPE_REBOOT;
		*nftp_payload_len = 0;
		nftp_sendrecv();
	}

	printf("\nFirmware updated/verified successfully!\n");
	
	return 0;
}