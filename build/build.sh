#!/bin/bash

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
# build.sh Function implementation: local build entry file, which is used to 
# build project to generate image.

#switch to sdk project root Dir
set -e

curr_dir=$(pwd)
cd $curr_dir/..

if [ "$1"  == "clean" ]; then
    echo "clean all building files"

    #clean
    python build/build.py clean -t gcc_arm -b debug
elif [ "$1"  == "all" ]; then
    echo "building all"
    #clean
    rm -rf out/
    python build/build.py clean -t gcc_arm -b debug
    #build
    python build/build.py build -t gcc_arm -b debug

    #package 512 bytes(header) to bin file
    python build/img_file_pack.py out/bin/target.bin 0x19000 out/bin/target_boot.bin 0xFF
    #bin file to hex file
    python build/bin_to_hex.py out/bin/target_boot.bin out/bin/target_boot.hex 0x00100000 0x19000 0x10  swap
elif [ "$1"  == "compile" ]; then
    echo "only compiling"
    #only build
    python build/build.py build -t gcc_arm -b debug
else
    echo "Wrong input !!!"
fi
