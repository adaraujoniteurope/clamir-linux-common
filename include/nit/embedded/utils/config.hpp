#ifndef _NIT_EMBEDDE_UTILS_LOGGER_HPP_
#define _NIT_EMBEDDE_UTILS_LOGGER_HPP_

#include <mutex>
#include <queue>
#include <thread>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <any>

#include <nit/embedded/utils/logger.hpp>

namespace nit::embedded::utils
{
    class config
    {
        private:
        public:

            std::unordered_map<std::string, std::any<double, std::string>> read_from(const char * path) {
                
                // nit::embedded::utils::logger::debug() << __func__ << std::endl;

                // pseudocode implementation for reading file contents and setting environment values
                // std::ifstream ifs;
                // for (auto& line: lines) {
                //     try {
                //         auto token;
                //         line >> token;
                //     } catch (std::exception& e) {
                //     }
                // }

            }

            std::string get_string(std::string key) { return std::string(""); }
            double get_number(std::string key) { return 0.0; }

        private:
    };
}

#endif