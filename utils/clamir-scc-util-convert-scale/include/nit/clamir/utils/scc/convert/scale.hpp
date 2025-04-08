#ifndef _nit_clamir_utils_scc_convert_scale_hpp_
#define _nit_clamir_utils_scc_convert_scale_hpp_

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

#include <nit/clamir/utils/scc/common.hpp>

namespace nit::clamir::utils::scc::convert::scale
{

    namespace detail
    {
        void print_usage() {
            std::cout << "clamir-scc-util-convert-scale: usage" << std::endl;
            std::cout << "  clamir-scc-util-convert-scale -i <input-file> -o <output-file> -t <float|double>" << std::endl;
            std::cout << "      -i: specify input file" << std::endl;
            std::cout << "      -o: specify output file" << std::endl;
            std::cout << "      -o: type of scale element options:" << std::endl;
            std::cout << "              float: (4 bytes little endian IEEE754)" << std::endl;
            std::cout << "              double: (8 bytes little endian IEEE754)" << std::endl;
        }

        std::string trim(const std::string& source) {
            std::string result(source);
            result.erase(0, result.find_first_not_of(" \n\r\t"));
            result.erase(result.find_last_not_of(" \n\r\t") + 1);
            return result;
        }

    }

    int main(int argc, char* argv[])
    {

        std::string output_path = "nit-scc-core-scale-default.dat";
        std::string input_path = "";
        std::string type_name = "float";

        for (;;)
        {
            switch (getopt(argc, argv, "i:o:t:h")) // note the colon (:) to indicate that 'b' has a parameter and is not a switch
            {
            case 'o':
                printf("parameter 'o' specified with the value %s\n", optarg);
                output_path = detail::trim(optarg);
                continue;

            case 't':
                printf("parameter 't' specified with the value %s\n", optarg);
                type_name = detail::trim(optarg);
                detail::trim(type_name);
                continue;

            case 'i':
                printf("parameter 'i' specified with the value %s\n", optarg);
                input_path = detail::trim(optarg);
                detail::trim(input_path);
                continue;

            case '?':
            case 'h':
            default:
                detail::print_usage();
                break;

            case -1:
                break;
            }

            break;
        }

        if (input_path.empty()) {
            std::cout << "no input path specified." << std::endl;
            exit(1);
        }

        if (std::filesystem::exists(output_path)) {
            std::filesystem::remove(output_path);
        }

        if (std::filesystem::file_size(input_path) != sizeof(float) * 64 * 64)
        {
            std::cout << "size is not compatible with clamir sensor..." << std::endl;
        }

        std::vector<int> scale;

        if (type_name == "float")
        {
            scale = nit::clamir::utils::scc::commmon::load<float>(input_path);
        }

        nit::clamir::utils::scc::commmon::save(output_path, scale);

        return 0;
    }
}
#endif