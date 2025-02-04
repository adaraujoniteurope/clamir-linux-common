#ifndef UTILS_CONF_FILE_H_
#define UTILS_CONF_FILE_H_


#include <fstream>
#include <filesystem>

#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>

template<class state_type>
int config_file_save_to_file(state_type* state, const char *path)
{   
    try {
        std::ofstream os(path, std::ios::binary);
        cereal::JSONOutputArchive archive( os );
        archive(state->config);
    } catch (std::exception & ex)
    {
        std::cout << ex.what() << std::endl;
        return -1;
    }

    return 0;
}

template<class state_type>
int config_file_load_from_file(state_type* state, const char *path)
{
    if (!std::filesystem::exists(path)) {
        return -1;
    }
    
    try {
        std::ifstream is(path, std::ios::binary);
        cereal::JSONInputArchive archive( is );
        archive(state->config);
    } catch (std::exception & ex)
    {
        std::cout << ex.what() << std::endl;
        return -1;
    }

    return 0;
}

#endif