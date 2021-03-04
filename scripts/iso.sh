#!/bin/sh
DIR="$(cd "$(dirname "$0")" && pwd)"

set -e
. $DIR/build.sh

mkdir -p ../isodir
mkdir -p ../isodir/boot
mkdir -p ../isodir/boot/grub

cp $DIR/../sysroot/boot/perry.kernel $DIR/../isodir/boot/perry.kernel
cat > $DIR/../isodir/boot/grub/grub.cfg << EOF
menuentry "perry" {
	multiboot /boot/perry.kernel
}
EOF
grub-mkrescue -o perry.iso $DIR/../isodir

mkdir -p $DIR/../bin
mv perry.iso $DIR/../bin/perry.iso