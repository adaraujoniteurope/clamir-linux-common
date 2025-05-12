#ifndef UTILS_CONF_FILE_H_
#define UTILS_CONF_FILE_H_


#include <fstream>
#include <filesystem>
#include <exception>
#include <iostream>

#include <syslog.h>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

template<class state_type>
int config_file_save_to_file(state_type* state, const char *path)
{   
    try {
        std::ofstream os(path, std::ios::binary);
        boost::archive::xml_oarchive archive( os );
        archive <<  boost::make_nvp("config", state->config);
    } catch (std::exception & ex)
    {
        syslog(LOG_INFO, ex.what());
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
        boost::archive::xml_iarchive archive( is );
        archive >> boost::make_nvp("config", state->config);
    } catch (std::exception & ex)
    {
        syslog(LOG_INFO, ex.what());
        return -1;
    }

    return 0;
}

#endif