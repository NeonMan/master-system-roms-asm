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
  
  sum = 0
  
  for b in buff[:(32*1024) - 16]:
    sum = (sum + b) & 0xFFFF
  
  header = bytes("TMR SEGA", 'utf-8') #TMR sega
  header = header + bytes((0,0))      #Reserved
  header = header + bytes( (sum & 0xFF, (sum >> 8)& 0xFF) ) #Checksum
  header = header + bytes( (0,0,0) ) #Product code & version
  header = header + bytes((0x4C,))
  
  with open(path_in, 'wb') as f:
    f.write(buff[:(32*1024)-16])
    f.write(header)
    f.close()