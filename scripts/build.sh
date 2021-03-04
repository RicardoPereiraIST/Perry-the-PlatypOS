#!/bin/sh
DIR="$(cd "$(dirname "$0")" && pwd)"

set -e
. $DIR/headers.sh

for PROJECT in $PROJECTS; do
  (cd $DIR/../src/$PROJECT && DESTDIR="$SYSROOT" $MAKE install)
done
