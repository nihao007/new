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
# img_file_pack.py Function implementation: Generating the package for image files, which include specific header.

import os
import sys


def calc_file_crc(addr: bytes, crc: int = 0xFFFF) -> int:
    for byte in addr:
        crc ^= byte
        for _ in range(8):
            if crc & 1:
                crc >>= 1
                crc ^= 0xA001
            else:
                crc >>= 1
    return crc & 0xFFFF  # 确保结果为16位


def read_hex(string: str):
    try:
        ret = int(string, 16)
        return ret
    except ValueError:
        return 0


def read_bin_file(bin_size: int, bin_file_name: str, pad_byte: int = 0xFF) -> bytes:
    bin_offset = 512
    try:
        with open(bin_file_name, 'rb') as bin_file:
            file_data = bin_file.read()
    except IOError as e:
        raise Exception("open firm.bin failed! \n") from e

    # 计算文件长度
    file_len = len(file_data)
    if (file_len + bin_offset) > bin_size:
        raise Exception("len of %s larger than %d! \n" % (bin_file_name, bin_size))

    bin_file_array = bytearray(bin_size)
    bin_file_array[:bin_offset] = bytes([pad_byte]) * bin_offset
    bin_file_array[bin_offset:bin_offset + file_len] = file_data
    # 在文件内容后填充pad_byte，直到达到指定的长度
    bin_file_array[bin_offset + file_len:] = bytes([pad_byte]) * (bin_size - bin_offset - file_len)
    return bin_file_array


def add_bin_head(bin_file: bytearray) -> int:
    """
     * ************************************************************************************************************
     *                                              APP FLASH 空间布局要求
     *
     *  FlashStart(0x00100000): +-------------------+-------------------+-------------------+-------------------+
     *                          | AppLen(4 Bytes)   | Checksum(4 Bytes) |   resv(4 Bytes)   | HeadCheck(4 Bytes)|
     *    AppStart(0x00100010): +---------------------------------------+---------------------------------------+
     *                          |                                      APP                                      |
     *                          |                                                                               |
     * VecterStart(0x00100200): |  Vecter(320 Bytes)                                                            |
     *                          |                                                                               |
     *                          |                                                                               |
     *                          +----------------------------------------+--------------------------------------+
     *                  AppEnd: = AppStart + AppLen
     *
     * 说明：
     *     1、AppLen、Checksum、HeadCheck字段使用小端字节序。
     *     2、AppLen必须是16字节的整数倍（实际bin文件尾部填充0xFF补齐长度）。表示从AppStart开始的文件长度(Byte个数)。
     *     3、Checksum的检查起始位置是AppStart，校验长度由AppLen指定。校验算法为进位累加范围内的uint8字节，计算累加和。
     *     4、在打包时，先补足APP的长度到16的整数倍，然后计算APP的Checksum，再把AppLen和Checksum写到前面的字段里。resv填0xFF.
     *     5、HeadCheck字段是AppLen和Checksum域共8个字节的checksum。通过这个校验，可以保障len和checksum字段没有错误。
     *     6、APP的Vector（中断向量表）必须放在VecterStart开始的320个字节中。 bootrom程序通过调用其中的ResetHandle引导APP执行。
     **************************************************************************************************************
    """
    # 计算从第16字节开始的校验和
    boot_checksum = sum(bin_file[16:])

    # 将长度信息（不包括头部16字节）写入二进制文件数组的前4字节
    len_minus_16 = len(bin_file) - 16
    bin_file[0:4] = len_minus_16.to_bytes(4, 'little')

    # 将校验和写入二进制文件数组的第4到第8字节
    bin_file[4:8] = boot_checksum.to_bytes(4, 'little')

    # 计算头部（前8字节）的校验和
    head_checksum = sum(bin_file[:8])

    # 将头部校验和写入二进制文件数组的第12到第16字节
    bin_file[12:16] = head_checksum.to_bytes(4, 'little')

    # 返回boot校验和
    return boot_checksum


def pack_firm_file(bin_file: bytes, w_len: int, output_bin_file_name: str) -> int:
    try:
        # 以二进制写模式打开输出文件
        with open(output_bin_file_name, 'wb') as firm_bin:
            # 将字节数组写入文件
            firm_bin.write(bin_file[:w_len])
    except IOError as e:
        raise Exception("create firm.bin failed! \n") from e


def main(args):
    if len(args) != 5:
        print("Input Error! The valid command format is:\n")
        print("3071_pack boot.bin boot_size output_boot.bin pad\n")
        return

    str_bin_name = args[1]
    strbinsize = args[2]
    stroutputbin = args[3]
    str_pad = args[4]

    bin_size = read_hex(strbinsize)
    pad = read_hex(str_pad)
    g_bin_file = read_bin_file(bin_size, str_bin_name, pad)
    checksum = add_bin_head(g_bin_file)
    pack_firm_file(g_bin_file, bin_size, stroutputbin)

    print("************** packed success! ***********************\n")
    print(f"\t len       = {bin_size}")
    print(f"\t checksum  = {checksum}")
    print("******************************************************\n")


if __name__ == "__main__":
    main(sys.argv)
