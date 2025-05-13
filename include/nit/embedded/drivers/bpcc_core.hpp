#ifndef nit_embedded_drivers_bpcc_core_hpp_
#define nit_embedded_drivers_bpcc_core_hpp_

#include <nit/embedded/drivers/memory_mapped_device.hpp>
#include <nit/embedded/drivers/pwm_core.hpp>
#include <nit/embedded/drivers/utils.hpp>

#include <nit/embedded/utils/bitfield.h>

using namespace nit::embedded::drivers;

namespace nit::embedded::drivers
{
    class bpcc_core : public memory_mapped_device<bpcc_core>
    {
    public:

        bpcc_core(size_t offset, size_t size, std::string path = "/dev/mem") : memory_mapped_device<bpcc_core>(offset, size, path, nullptr)
        {
        }
    };
}

#endif