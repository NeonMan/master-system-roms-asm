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

#Makes a plain ROM bootable by using the first 16K of data as VRAM and the
#8K past 0xC000 the code section of a ROM

#
# Original ROM format must be:
#
# Offset   Description
# 0x0000  +-----------+
#         | VRAM blob |
#         |   (16K)   | <-- Although is optional to give VRAM any valid data
# 0x3FFF  +-----------+     the bytes must be there so alignment is kept.
# 0x4000  +-----------+
#         |  Padding  |
#         |   (32K)   | <-- This range of bytes is ignored.
# 0xBFFF  +-----------+     The 32K of padding are a result of `.org 0xC000`
# 0xC000  +-----------+
#         |Program ROM|
#         |   ( 8K)   |
# 0xDFFF  +-----------+

import sys

USAGE = '''\
Usage: make_bootable.py INPUT_FILE OUT_FILE [--no-vram]\
'''

# ------------
# --- Main ---
# ------------
if __name__ == '__main__':

  enable_vram = True
  if len(sys.argv) == 4:
    if sys.argv[3] == '--no-vram':
      enable_vram = False

  if (len(sys.argv) > 4) or (len(sys.argv) < 3):
    print(USAGE)
    exit()
  
  path_in   = sys.argv[1]
  path_out  = sys.argv[2]
  buff = None
  
  with open(path_in, 'rb') as f:
    buff = f.read()
    f.close()
  
  with open(path_out, 'wb') as f:
    f.write(buff[0xC000:0xE000])
    if enable_vram:
      f.write(buff[0x0000:0x4000])
    f.close()
