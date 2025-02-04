#ifndef _utils_bitfield_h_
#define _utils_bitfield_h_

#include <cstddef>
#include <cstdint>

template<typename type, size_t msb, size_t lsb>
struct bitfield
{
    public:
    bitfield(type& value) : value(value) {}

    type get()
    {
        return (value & mask) >> lsb;
    }
    
    void set(type v)
    {
        value &= ~mask;
        value |= (v << lsb) & mask;
    }


    private:
    type mask = (type)(((uint64_t)((uint64_t)1 << ((uint64_t)msb+1))-(uint64_t)1) & ~(uint64_t)(((uint64_t)1 << ((uint64_t)lsb))-(uint64_t)1));
    type& value;

};


#endif