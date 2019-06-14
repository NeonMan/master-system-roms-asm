#!/bin/sh
#Append $1 [$2] to the ramload.sms ROM

#get ramload.sms location
if [ -z "$LOADER_ROM" ]
then
  LOADER_ROM=selfboot.sms
fi

#Get ROM segment size
ROM_SIZE=`stat "$1" --format=%s`
if [ $? -ne 0 ]
then
  echo "Usage: $0 ROM_DATA [VRAM_DATA]"
  exit 1
fi
if [ $ROM_SIZE -gt 8192 ]
then
  echo "ROM size too big: $ROM_SIZE > 8192"
  exit 1
fi
PADDING_SIZE=`expr 8192 - $ROM_SIZE`

#Get VRAM segment size
VRAM_SIZE=`stat "$2" --format=%s 2>/dev/null`
if [ $? -ne 0 ]
then
  VRAM_SIZE=0
fi
if [ $VRAM_SIZE -gt 16384 ]
then
  echo "VRAM size too big: $VRAM_SIZE > 16384"
  exit 1
fi
PADDING_VRAM_SIZE=`expr 16384 - $VRAM_SIZE`

#Show info
echo "Loader ROM: $LOADER_ROM"
echo "       ROM: $1"
echo "  ROM size: $ROM_SIZE (+$PADDING_SIZE padding)"
echo "  VRAM ROM: $2"
echo " VRAM size: $VRAM_SIZE (+$PADDING_VRAM_SIZE padding)"

#Append the ROM data
OUT_FILE="$1.boot.sms"
cat "$LOADER_ROM" "$1" > "$OUT_FILE"

#Append padding if needed
if [ $PADDING_SIZE -gt 0 ]
then
  SEEK_ADDR=`stat "$LOADER_ROM" --format=%s 2>/dev/null`
  SEEK_ADDR=`expr $SEEK_ADDR + $ROM_SIZE`
  cat /dev/zero | tr '\0' '\377' | dd of="$OUT_FILE" bs=1 count=$PADDING_SIZE seek=$SEEK_ADDR 2>/dev/null
fi

#Append VRAM if needed
if [ $VRAM_SIZE -gt 0 ]
then
  cat "$2" >> "$OUT_FILE"
fi

#Add VRAM padding
if [ $PADDING_VRAM_SIZE -gt 0 ]
then
  SEEK_ADDR=`expr 16384 + $VRAM_SIZE`
  dd if=/dev/zero of="$OUT_FILE" bs=1 count=$PADDING_VRAM_SIZE seek=$SEEK_ADDR 2>/dev/null
fi

#Resulting ROM must be *exactly* 32K
OUT_SIZE=`stat "$OUT_FILE" --format=%s`
if [ $OUT_SIZE -ne 32768 ]
then
  echo "Resulting ROM size missmatch (Bug?)"
  exit 1
fi
