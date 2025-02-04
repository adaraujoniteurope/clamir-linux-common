#ifndef NETWORKING_TCP_PROTOCOL_HPP_
#define NETWORKING_TCP_PROTOCOL_HPP_

#include <utils/bitfield.h>

#include <iomanip>
#include <iostream>

struct packet
{
    uint32_t data;

    bitfield<decltype(data), 6, 0> endpoint;
    bitfield<decltype(data), 7, 7> rw;
    bitfield<decltype(data), 11, 8> group;
    bitfield<decltype(data), 31, 16> value;

    uint16_t route_id_get() { uint16_t v = endpoint.get(); v |= group.get() << 8; return v; }
    void route_id_set(uint16_t v) { group.set(v >> 8); endpoint.set(v); }

    packet(uint32_t packet) : data(packet), endpoint(data), rw(data), group(data), value(data) {}

};

std::ostream& operator<<(std::ostream& os, packet& req);
std::istream& operator>>(std::istream& is, packet& req);

#endif