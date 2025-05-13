#ifndef nit_embedded_drivers_scc_core_hpp_
#define nit_embedded_drivers_scc_core_hpp_

#include <nit/embedded/drivers/memory_mapped_device.hpp>
#include <nit/embedded/drivers/pwm_core.hpp>
#include <nit/embedded/drivers/utils.hpp>

#include <nit/embedded/utils/bitfield.h>

using namespace nit::embedded::drivers;

namespace nit::embedded::drivers
{
    class scc_core : public memory_mapped_device<scc_core>
    {
    public:

        scc_core(size_t offset = 0x40002000, size_t size = 0x1000, std::string path = "/dev/mem") : memory_mapped_device<scc_core>(offset, size, path, nullptr)
        {
        }
        
        int testing_disable_get() { return read<uint32_t>(0); }
        void testing_disable_set(int value) { return write<uint32_t>(0, value); }

        
        int testing_addr_get() { return read<uint32_t>(1); }
        void testing_addr_set(int value) { return write<uint32_t>(1, value); }

        
        int testing_wren_get() { return read<uint32_t>(2); }
        void testing_wren_set(int value) { return write<uint32_t>(2, value); }

        
        int testing_data_get() { return read<uint32_t>(3); }
        void testing_data_set(int value) { return write<uint32_t>(3, value); }

    };
}

#endif