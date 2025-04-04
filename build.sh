#!/bin/bash -e

#export CC=/usr/bin/arm-linux-gnueabihf-gcc
#export CXX=/usr/bin/arm-linux-gnueabihf-g++

rm -rf /home/arthur/Work/Software/clamir-linux-daemon/build-cmaketools

. /home/arthur/Work/Petalinux/clamir-petalinux-v2024.2-sdk/environment-setup-cortexa9t2hf-neon-xilinx-linux-gnueabi

cmake -G Ninja -B /home/arthur/Work/Software/clamir-linux-daemon/build-cmaketools -S . "-DCMAKE_C_COMPILER_WORKS=TRUE" "-DCMAKE_CXX_COMPILER_WORKS=TRUE" "-DNIT_CLAMIR_HOST_MOCKUP=OFF" "-DNIT_CLAMIR_ENABLE_TESTING=OFF" "-DNIT_CLAMIR_ENABLE_TESTING=No" "-DNIT_CLAMIR_ENABLE_PROFINET=YES",  "-DCMAKE_BUILD_TYPE=Debug" ]
cmake --build /home/arthur/Work/Software/clamir-linux-daemon/build-cmaketools -j$(nproc)
