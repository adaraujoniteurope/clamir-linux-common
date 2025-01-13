#ifndef NETWORKING_TCP_PROTOCOL_HPP_
#define NETWORKING_TCP_PROTOCOL_HPP_

#include <string>
#include <memory>
#include <array>
#include <vector>

#include <cereal/cereal.hpp>

enum message_type
{
    /**
     * Integral Types
     */
    MESSAGE_TYPE_INT8,
    MESSAGE_TYPE_UINT8,

    MESSAGE_TYPE_INT16,
    MESSAGE_TYPE_UINT16,

    MESSAGE_TYPE_INT32,
    MESSAGE_TYPE_UINT32,

    MESSAGE_TYPE_FLOAT,
    MESSAGE_TYPE_DOUBLE,

    /**
     * Array Types
     */
    MESSAGE_TYPE_STRING,

    MESSAGE_TYPE_MAX,
};

enum request_type {
    REQUEST_TYPE_READ,
    REQUEST_TYPE_WRITE,
    REQUEST_TYPE_MAX
};

struct payload
{
    message_type request;
    message_type type;
    std::string key;
    std::vector<char> data;
    
    template<class archiver>
    void serialize(archiver& ar) 
    {
        ar(request, type, key);
    }
};


#endif