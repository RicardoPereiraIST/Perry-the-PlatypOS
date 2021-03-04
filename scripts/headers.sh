#!/bin/sh
DIR="$(cd "$(dirname "$0")" && pwd)"

set -e
. $DIR/config.sh

mkdir -p "$SYSROOT"

for PROJECT in $SYSTEM_HEADER_PROJECTS; do
  (cd $DIR/../src/$PROJECT && DESTDIR="$SYSROOT" $MAKE install-headers)
done
