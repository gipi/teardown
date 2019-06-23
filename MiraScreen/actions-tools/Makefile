BINS = actions-tools

all: $(BINS)

actions-tools: actions-tools.o z.o checksum.o firm.o
actions-tools: LDFLAGS=-lz

clean:
	rm -vfr $(BINS) *.o

