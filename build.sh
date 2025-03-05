#!/bin/bash -e

#export CC=/usr/bin/arm-linux-gnueabihf-gcc
#export CXX=/usr/bin/arm-linux-gnueabihf-g++

rm -rf /home/arthur/Work/Software/clamir-linux-daemon/build-cmaketools

cmake -G Ninja -B /home/arthur/Work/Software/clamir-linux-daemon/build-cmaketools -S . -DNIT_CLAMIR_ENABLE_TESTING=No -DNIT_CLAMIR_ENABLE_PROFINET=YES  -DCMAKE_BUILD_TYPE=Debug
cmake --build /home/arthur/Work/Software/clamir-linux-daemon/build-cmaketools -j$(nproc)
