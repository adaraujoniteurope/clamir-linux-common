#ifndef NETWORKING_TCP_PROTOCOL_HPP_
#define NETWORKING_TCP_PROTOCOL_HPP_

#include <nit/embedded/utils/bitfield.h>

#include <iomanip>
#include <iostream>

struct packet
{
    public:

    uint32_t endpoint;
    uint32_t rw;
    uint32_t group;
    uint32_t value;

    uint16_t route_id_get() { uint16_t v = endpoint; v |= group << 8; return v; }

    void route_id_set(uint16_t v)
    {
        group = (v & 0xFF00) >> 8;
        endpoint = (v & 0x00FF) >> 0;
    }

    packet(const uint32_t& v = 0) {
        endpoint = bitfield<uint32_t, 6, 0>::get(v);
        rw = bitfield<uint32_t, 7, 7>::get(v);
        group = bitfield<uint32_t, 11, 8>::get(v);
        value = bitfield<uint32_t, 31, 16>::get(v);
    }

    static packet parse(const uint32_t& value) {
        return packet(value);
    }

    static const uint32_t encode(const packet& p) {
        uint32_t value = 0x00;
        value |= bitfield<uint32_t, 6, 0>::set(p.endpoint);
        value |= bitfield<uint32_t, 7, 7>::set(p.rw);
        value |= bitfield<uint32_t, 11, 8>::set(p.group);
        value |= bitfield<uint32_t, 31, 16>::set(p.value);
        return value;
    }

    private:
};

std::ostream& operator<<(std::ostream& os, packet& req);
std::istream& operator>>(std::istream& is, packet& req);

#endif