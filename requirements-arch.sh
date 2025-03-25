# pacman -Syu cmake ninja gcc-arm-linux-gnueabihf git

git config --global user.name "Arthur de Araújo Farias"
git config --global user.email "adaraujo@niteurope.com"

export CC=/usr/bin/arm-none-eabi-gcc
export CXX=/usr/bin/arm-none-eabi-g++

cmake -G Ninja -B build-target/osal -S third-party/osal -DCMAKE_INSTALL_PREFIX=/usr -DNIT_CLAMIR_ENABLE_TESTING=No -DNIT_CLAMIR_ENABLE_PROFINET=YES  -DCMAKE_BUILD_TYPE=Debug
cmake --build build-target/osal -j$(nproc)
cmake --install build-target/osal

cmake -G Ninja -B build-target/p-net -S third-party/p-net -DCMAKE_INSTALL_PREFIX=/usr -DNIT_CLAMIR_ENABLE_TESTING=No -DNIT_CLAMIR_ENABLE_PROFINET=YES  -DCMAKE_BUILD_TYPE=Debug
cmake --build build-target/p-net -j$(nproc)
cmake --install build-target/p-net