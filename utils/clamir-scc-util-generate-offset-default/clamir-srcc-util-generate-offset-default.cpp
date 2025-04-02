#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

void print_usage();
int generage_offset(const char* path, double offset = 1.0);

int main(int argc, char* argv[])
{
    std::string path = "offset.dat";
    double offset = 0.0;

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
                ss >> offset;
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
    printf("offset: 's' specified with the value %lf\n", offset);

    if (generage_offset(path.c_str(), offset) < 0) {
        printf("failed to generate data");
    }

    return 0;
}

void print_usage() {
    printf("clamir-scc-util-generate-offset-default: usage\n");
}

int generage_offset(const char* path, double offset) {

    int fd =
        open(path, O_TRUNC | O_CREAT | O_RDWR | S_IRUSR | S_IWUSR);
        chmod(path, 0644);

    if (fd < 0) {
        perror("failed to create offset file");
        exit(errno);
    }

    int val = 16768;

    /**
     * doing this waay because ofstream will format as stringk
     */
    for (int i = 0; i < 64*64; i++) {
        write(fd, &val, sizeof(val));
    }

    close(fd);

    return 0;
}