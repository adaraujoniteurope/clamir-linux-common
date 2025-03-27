#include <nit/embedded/networking/tcp/protocol_legacy.hpp>

#include <cinttypes>
#include <iostream>
#include <iomanip>

std::ostream& operator<<(std::ostream& os, packet& req)
{
    os
    << "{ "
    << "\"route_id\": 0x" << std::hex << std::setw(sizeof(uint16_t)) << std::setfill('0') << (int) req.route_id_get() << ", "
    << "\"endpoint\": 0x" << std::hex << std::setw(sizeof(uint8_t)) << std::setfill('0') << (int) req.endpoint << ", "
    << "\"rw\": 0x" << std::hex << std::setw(sizeof(uint8_t)) << std::setfill('0') << (int) req.rw << ", "
    << "\"group\": 0x" << std::hex << std::setw(sizeof(uint8_t)) << std::setfill('0') << (int) req.group << ", "
    << "\"value\": 0x" << std::hex << std::setw(sizeof(uint16_t)) << std::setfill('0') << (int) req.value
    << "}";
    
    return os;
}