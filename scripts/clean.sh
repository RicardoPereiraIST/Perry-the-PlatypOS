#!/bin/sh
DIR="$(cd "$(dirname "$0")" && pwd)"

set -e
. $DIR/config.sh

for PROJECT in $PROJECTS; do
  (cd $DIR/../src/$PROJECT && $MAKE clean)
done

rm -rf ../sysroot
rm -rf ../isodir
rm -rf ../bin/perry.iso