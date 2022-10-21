#!/bin/sh

[ -f fake_usb.img ] || bzip2 -dk fake_usb.img.bz2

device_name=`sudo losetup -f`
sudo losetup -fP fake_usb.img
sudo mkdir -p /mnt/USB
sudo mount ${device_name}p1 /mnt/USB
