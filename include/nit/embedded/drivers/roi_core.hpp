#ifndef nit_embedded_drivers_roi_core_hpp_
#define nit_embedded_drivers_roi_core_hpp_

#include <nit/embedded/drivers/memory_mapped_device.hpp>
#include <nit/embedded/drivers/pwm_core.hpp>
#include <nit/embedded/drivers/utils.hpp>

#include <nit/embedded/utils/bitfield.h>

using namespace nit::embedded::drivers::utils;

namespace nit::embedded::drivers
{
    class roi_core : public memory_mapped_device<roi_core>
    {
    public:

        roi_core(size_t offset = 0x50000000, size_t size = 0x00010000, std::string path = "/dev/mem") : memory_mapped_device<roi_core>(offset, size, path, nullptr)
        {
        }

        int x1_get() { return read<uint32_t>(0x8000 + 0); }
        void x1_set(int value) { return write<uint32_t>(0x8000 + 0, value); }

        int y1_get() { return read<uint32_t>(0x8000 + 1); }
        void y1_set(int value) { return write<uint32_t>(0x8000 + 1, value); }

        int x2_get() { return read<uint32_t>(0x8000 + 2); }
        void x2_set(int value) { return write<uint32_t>(0x8000 + 2, value); }

        int y2_get() { return read<uint32_t>(0x8000 + 3); }
        void y2_set(int value) { return write<uint32_t>(0x8000 + 3, value); }

        int round_get() { return read<uint32_t>(0x8000 + 4); }
        void round_set(int value) { return write<uint32_t>(0x8000 + 4, value); }

    };
}

#endif