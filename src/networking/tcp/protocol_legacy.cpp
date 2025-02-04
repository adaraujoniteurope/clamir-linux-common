#include "networking/tcp/protocol_legacy.hpp"

#include <cinttypes>
#include <iostream>
#include <iomanip>

std::ostream& operator<<(std::ostream& os, packet& req)
{
    os
    << "{" << std::endl
    << "data: 0x" << std::hex << std::setw(sizeof(uint32_t)) << std::setfill('0') << (int) req.data << std::endl
    << "route_id: 0x" << std::hex << std::setw(sizeof(uint16_t)) << std::setfill('0') << (int) req.route_id_get() << std::endl
    << "endpoint: 0x" << std::hex << std::setw(sizeof(uint8_t)) << std::setfill('0') << (int) req.endpoint.get() << std::endl
    << "rw: 0x" << std::hex << std::setw(sizeof(uint8_t)) << std::setfill('0') << (int) req.rw.get() << std::endl
    << "group: 0x" << std::hex << std::setw(sizeof(uint8_t)) << std::setfill('0') << (int) req.group.get() << std::endl
    << "value: 0x" << std::hex << std::setw(sizeof(uint16_t)) << std::setfill('0') << (int) req.value.get() << std::endl
    << "}" << std::endl;
    return os;
}