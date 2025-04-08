#ifndef _nit_clamir_scc_utils_scc_common_hpp_
#define _nit_clamir_scc_utils_scc_common_hpp_

#include <cstdio>
#include <format>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

namespace nit::clamir::utils::scc::commmon
{
    template<typename type>
    std::vector<int> load(const std::string& path) {

        int fd =
            open(path.c_str(), O_RDONLY | O_SYNC);

        if (fd < 0) {
            perror("failed to open scale file");
            exit(errno);
        }

        std::vector<int> array;
        type value = 16384;

        while (read(fd, &value, sizeof(value)) == sizeof(type))
        {
            array.push_back((int)value);
        }

        close(fd);

        return array;
    }

    int save(std::string path, std::vector<int> array)
    {
        int fd =
            open(path.c_str(), O_CREAT | O_RDWR | O_SYNC, 0644);

        if (fd < 0) {
            perror("failed to create scale file");
            exit(errno);
        }

        for (auto& value : array)
        {
            write(fd, &value, sizeof(int));
        }

        close(fd);

        return 0;
    }
}

#endif