#!/bin/sh

if [ "$1" = "" ]; then
    echo "Missing loopback device path"
    exit 1
fi

sudo umount /mnt/USB
sudo losetup -d "$1"
