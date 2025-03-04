#!/bin/bash -e

export CC=/usr/bin/arm-linux-gnueabihf-gcc
export CXX=/usr/bin/arm-linux-gnueabihf-g++

cmake -G Ninja -B build-target/clamir -S . -DNIT_CLAMIR_ENABLE_TESTING=No -DNIT_CLAMIR_ENABLE_PROFINET=YES  -DCMAKE_BUILD_TYPE=Debug
cmake --build build-target/clamir -j$(nproc)