#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <sys/mman.h>

#define NIT_SCC_CORE_SCALE_BASE_ADDRESS 0x46000000
#define NIT_SCC_CORE_SCALE_BASE_SIZE 0x00008000

#define NIT_SCC_CORE_OFFSET_BASE_ADDRESS 0x48000000
#define NIT_SCC_CORE_OFFSET_BASE_SIZE 0x00008000

void print_usage();
int generage_scale(const char* path, double scale = 1.0);

int main(int argc, char* argv[])
{
    int fd =
        open("/dev/mem", O_CREAT | O_RDWR | O_SYNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
        
    if (fd < 0) {
        perror("failed to open /dev/mem");
        exit(errno);
    }

    int* map = (int*) mmap(NULL, NIT_SCC_CORE_SCALE_BASE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, NIT_SCC_CORE_SCALE_BASE_ADDRESS);

    if (map == nullptr || map == (int*)(UINT32_MAX)) {
        perror("failed to map /dev/mem");
        exit(errno);
    }

    int val = 16768;

    for (int i = 0; i < 64*64; i++) {
        write(fd, &val, sizeof(val));
    }

    close(fd);

    return 0;
}