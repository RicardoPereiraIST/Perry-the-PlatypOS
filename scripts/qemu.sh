#!/bin/bash
DIR="$(cd "$(dirname "$0")" && pwd)"

set -e
. $DIR/iso.sh

ISO="${DIR}/../bin/perry.iso"

if [[ $1 == "debug" ]]; then
	qemu-system-$($DIR/target-triplet-to-arch.sh $HOST) -S -monitor stdio -no-reboot -no-shutdown -d int -gdb tcp::1234 -boot d -cdrom $ISO -m 512 &
else
    qemu-system-$($DIR/target-triplet-to-arch.sh $HOST) -monitor stdio -cdrom $ISO -m 512 #-no-reboot -d int -no-shutdown
fi