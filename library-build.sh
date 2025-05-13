#!/bin/bash -e

. /home/arthur/Work/Petalinux/clamir-petalinux-v2024.2-sdk/environment-setup-cortexa9t2hf-neon-xilinx-linux-gnueabi

cmake -G Ninja -B build-target -S . "-DNIT_CLAMIR_DEBUGGING=TRUE" "-DCMAKE_C_COMPILER_WORKS=TRUE" "-DCMAKE_CXX_COMPILER_WORKS=TRUE" "-DNIT_CLAMIR_HOST_MOCKUP=OFF" "-DNIT_CLAMIR_ENABLE_TESTING=OFF" "-DNIT_CLAMIR_ENABLE_TESTING=No" "-DNIT_CLAMIR_ENABLE_PROFINET=YES",  "-DCMAKE_BUILD_TYPE=Debug"
cmake --build build-target -j$(nproc)
