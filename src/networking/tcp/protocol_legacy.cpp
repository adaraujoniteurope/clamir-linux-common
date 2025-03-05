#include "networking/tcp/protocol_legacy.hpp"

#include <cinttypes>
#include <iostream>
#include <iomanip>

std::ostream& operator<<(std::ostream& os, packet& req)
{
    os
    << "{ "
    << "\"data\": 0x" << std::hex << std::setw(sizeof(uint32_t)) << std::setfill('0') << (int) req.data << ", "
    << "\"route_id\": 0x" << std::hex << std::setw(sizeof(uint16_t)) << std::setfill('0') << (int) req.route_id_get() << ", "
    << "\"endpoint\": 0x" << std::hex << std::setw(sizeof(uint8_t)) << std::setfill('0') << (int) req.endpoint.get() << ", "
    << "\"rw\": 0x" << std::hex << std::setw(sizeof(uint8_t)) << std::setfill('0') << (int) req.rw.get() << ", "
    << "\"group\": 0x" << std::hex << std::setw(sizeof(uint8_t)) << std::setfill('0') << (int) req.group.get() << ", "
    << "\"value\": 0x" << std::hex << std::setw(sizeof(uint16_t)) << std::setfill('0') << (int) req.value.get()
    << "}";
    return os;
}