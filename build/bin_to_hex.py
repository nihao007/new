#!/usr/bin/env python
# -*- coding: utf-8 -*-

# @copyright Copyright (c) 2025, HiSilicon (Shanghai) Technologies Co., Ltd. All rights reserved.
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
# following conditions are met:
# 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
# disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
# following disclaimer in the documentation and/or other materials provided with the distribution.
# 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
# products derived from this software without specific prior written permission.
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
# USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# bin_to_hex.py Function implementation: Convert the bin format file to hex format file.

import sys
import struct


def write_line(fp_out, addr: int, dtype: int, data: bytes):
    checksum = len(data) + ((addr >> 8) & 0xFF) + (addr & 0xFF) + dtype
    for byte in data:
        checksum += byte
    checksum = 0x01 + ~checksum & 0xFF

    line = f":{len(data):02X}{addr:04X}{dtype:02X}"
    for byte in data:
        line += f"{byte:02X}"
    line += f"{checksum:02X}\n"
    fp_out.write(line)


def parse_input_parameters(args):
    if len(args) not in [6, 7]:
        print("command format: bintohex <xx.bin> <xx.hex> <startaddr> <filelen> <linelen> <swap/NA>")
        print("eg.bintohex test.bin test.hex 0x10000 0x1000 0x10 swap")
        sys.exit(-1)

    input_params = {
        'ifn': args[1],
        'ofn': args[2],
        'startaddr': int(args[3], 16),
        'filelen': int(args[4], 16),
        'linelen': int(args[5], 16),
        'swap_bit': 1 if len(args) == 7 and args[6] == 'swap' else 0
    }

    if input_params['linelen'] not in [4, 8, 16, 32, 64, 128]:
        print("linelen must be power of 2.")
        sys.exit(-1)

    return input_params


def main():
    input_params = parse_input_parameters(sys.argv)
    with open(input_params['ifn'], 'rb') as fp_in, open(input_params['ofn'], 'w') as fp_out:
        startaddr = input_params['startaddr']
        endaddr = startaddr + input_params['filelen']
        linelen = input_params['linelen']
        swap_bit = input_params['swap_bit']

        prebase = 0xFFFF
        while startaddr < endaddr:
            base = (startaddr >> 16) & 0xFFFF
            if prebase != base:
                write_line(fp_out, 0, 4, struct.pack('>H', base))
                prebase = base

            buf = fp_in.read(linelen)
            if linelen != len(buf):
                break

            if swap_bit:
                buf = bytearray(buf)
                tmp = [0] * 4
                for i in range(0, linelen, 4):
                    tmp[0] = buf[i]
                    tmp[1] = buf[i + 1]
                    tmp[2] = buf[i + 2]
                    tmp[3] = buf[i + 3]

                    buf[i] = tmp[3]
                    buf[i + 1] = tmp[2]
                    buf[i + 2] = tmp[1]
                    buf[i + 3] = tmp[0]
            write_line(fp_out, startaddr & 0xFFFF, 0, buf)
            startaddr += linelen

        fp_out.write(":00000001FF")
        print("\r\n ======================== bin to hex file is OK! ======================== \r\n")


if __name__ == "__main__":
    main()
