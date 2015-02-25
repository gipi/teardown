#!/bin/sh
 
       ###########################################################
       #### mkimage.sh put in directory DG834GV4_V5.01.16_src ####
       ###########################################################
 
       if [ "$1" = "" ] || [ "$2" = "" ] || [ "$3" = "" ]; then
         echo "Usage:" $0 "orignal_image openwrt-96348GW-10-squashfs-cfe.bin new_image"
         exit
       fi
 
       echo
       echo "Original Image:" $1
       echo "bcm963xx_fs_kernel:" $2
       echo "New Image:" $3
       echo
       echo "Press 'y' to continue"
 
       read yn
 
       if [ "$yn" = "y" ]; then
               tools/makeImage $3 $1 $2
               echo $3 "Created!"
       else
               echo "EXIT!"
       fi