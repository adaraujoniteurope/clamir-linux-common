#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

void print_usage();
int generage_scale(const char* path, double scale = 1.0);

int main(int argc, char* argv[])
{
    std::string path = "scale.dat";
    double scale = 1.0;

    for (;;)
    {
        switch (getopt(argc, argv, "fs:h")) // note the colon (:) to indicate that 'b' has a parameter and is not a switch
        {
        case 'f':
            printf("parameter 'f' specified with the value %s\n", optarg);

            continue;

        case 's':
            {
                auto ss = std::stringstream(std::string(optarg));
                ss >> scale;
                continue;
            }

        case '?':
        case 'h':
        default:
            print_usage();
            break;

        case -1:
            break;
        }

        break;
    }

    printf("file: 'f' specified with the value %s\n", path.c_str());
    printf("scale: 's' specified with the value %lf\n", scale);

    if (generage_scale(path.c_str(), scale) < 0) {
        printf("failed to generate data");
    }

    return 0;
}

void print_usage() {
    printf("clamir-scc-util-generate-scale-default: usage\n");
}

int generage_scale(const char* path, double scale) {

    int fd =
        open(path, O_TRUNC | O_CREAT | O_RDWR | S_IRUSR | S_IWUSR);
        chmod(path, 0644);

    if (fd < 0) {
        perror("failed to create scale file");
        exit(errno);
    }

    int val = 16768;

    for (int i = 0; i < 64*64; i++) {
        write(fd, &val, sizeof(val));
    }

    close(fd);

    return 0;
}