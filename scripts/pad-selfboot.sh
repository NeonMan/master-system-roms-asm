#/bin/sh
#Pad ROM size to exactly 32K of size
FSIZE=`stat -c %s "$1"`
PSIZE=`expr 32768 - $FSIZE`
dd if=/dev/zero of="$1" bs=1 seek=$FSIZE count=$PSIZE
