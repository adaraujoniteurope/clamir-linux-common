#ifndef _NIT_EMBEDDED_DRIVERS_UTILS_HPP_
#define _NIT_EMBEDDED_DRIVERS_UTILS_HPP_

#include <cinttypes>

namespace nit::embedded::drivers::utils
{
    template<uint32_t S_AXI_FREQ_HZ = 100000000>
    static uint16_t frequency_to_prescaler_value(double frequency)
    {
        return ((uint16_t)(((double)S_AXI_FREQ_HZ)/frequency));
    }

    template<uint32_t S_AXI_FREQ_HZ = 100000000>
    static uint16_t duty_to_prescaler_value(double duty, double frequency)
    {
        return duty * frequency_to_prescaler_value<S_AXI_FREQ_HZ>(frequency);
    }
}

#endif