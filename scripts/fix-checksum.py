#!/usr/bin/env python3

# Copyright 2015 Juan Luis Álvarez Martínez
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#Set the correct checksum bytes on the ROM header

#Header format

# TMR SEGA     8B
# Reserverd    2B
# Checksum     2B
# Product code 2.5B
# Version      0.5B
# Region       0.5B
# ROM size     0.5B

import sys

USAGE = '''\
Usage: fix-checksum.py INPUT_FILE \
'''

# ------------
# --- Main ---
# ------------
if __name__ == '__main__':
  if len(sys.argv) != 2:
    print(USAGE)
    exit()
  
  path_in   = sys.argv[1]
  buff = None
  
  with open(path_in, 'rb') as f:
    buff = f.read()
    f.close()
  
  size = len(buff)
  header_location = None
  header_size = None
  
  if   size >= 1024*32:
    header_location = (32*1024) - 16
    header_size = 0x4C
  elif size >= 1024*16:
    header_location = (16*1024) - 16
    header_size = 0x4B
  elif size >= 1024*8:
    header_location = (8*1024) - 16
    header_size = 0x4A
  else:
    #Add padding to the ROM to fill 8K
    buff = buff + bytes("?" * ((8*1024) - len(buff)), 'ASCII')
    header_location = (8*1024) - 16
    header_size = 0x4A
    
  
  sum = 0
  
  for b in buff[:header_location]:
    sum = (sum + b) & 0xFFFF
  
  header = bytes("TMR SEGA", 'ASCII') #TMR sega
  header = header + bytes((0,0))      #Reserved
  header = header + bytes( (sum & 0xFF, (sum >> 8)& 0xFF) ) #Checksum
  header = header + bytes( (0,0,0) ) #Product code & version
  header = header + bytes((header_size,))
  
  with open(path_in, 'wb') as f:
    f.write(buff[:header_location])
    f.write(header)
    f.write(buff[header_location + 16:])
    f.close()