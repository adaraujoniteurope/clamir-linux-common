#!/bin/bash -e

export CC=/usr/bin/arm-linux-gnueabihf-gcc
export CXX=/usr/bin/arm-linux-gnueabihf-g++

cmake -G Ninja -B build -S . -DNIT_CLAMIR_ENABLE_TESTING=No -DNIT_CLAMIR_ENABLE_PROFINET=YES  -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j32
scp build/clamir root@192.168.0.12:/root
ssh -t root@192.168.0.12 'gdbserver 0.0.0.0:2000 /root/clamir'
