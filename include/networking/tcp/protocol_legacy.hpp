#ifndef NETWORKING_TCP_PROTOCOL_HPP_
#define NETWORKING_TCP_PROTOCOL_HPP_

typedef struct payload_struct {
    uint8_t reserved0:7;
    uint8_t rw:1;
    uint8_t route_id:4;
    uint8_t reserved1:4;
} payload;

class protocol
{
    public:
    static constexpr payload* cast(const unsigned char* buffer, int size) { return (payload*)(buffer); }

    private:
    protocol() = delete;
    protocol(protocol&) = delete;
    protocol(protocol&&) = delete;
};

#endif