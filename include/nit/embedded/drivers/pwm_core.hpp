#ifndef nit_embedded_drivers_pwm_core_hpp_
#define nit_embedded_drivers_pwm_core_hpp_

#include <nit/embedded/drivers/memory_mapped_device.hpp>

#include <nit/embedded/utils/bitfield.h>

using namespace nit::embedded::drivers;

namespace nit::embedded::drivers {

class pwm_core : public memory_mapped_device<pwm_core> {
public:
  pwm_core(size_t offset, size_t size, std::string path = "/dev/mem")
      : memory_mapped_device<pwm_core>(offset, size, path, nullptr) {}

  void frequency_set(unsigned int frequency) {
    return write<uint16_t>(0, frequency_to_prescaler_value(frequency));
  }

  unsigned int frequency_get() { return read<uint16_t>(0); }

  void duty_set(double duty) {

    if (duty > 1.0) {
      duty = 1.0;
    }

    if (duty < 0) {
      duty = 0.0;
    }

    write<uint16_t>(1, duty_to_prescaler_value(duty, frequency_));
  }

  double duty_get() { return read<uint16_t>(1); }

  void save_config(std::string path) {}

  void load_config(std::string path) {}

private:
  template <uint32_t S_AXI_FREQ_HZ = 100000000>
  static uint16_t frequency_to_prescaler_value(double frequency) {
    return ((uint16_t)(((double)S_AXI_FREQ_HZ) / frequency));
  }

  template <uint32_t S_AXI_FREQ_HZ = 100000000>
  static uint16_t duty_to_prescaler_value(double duty, double frequency) {
    return duty * frequency_to_prescaler_value<S_AXI_FREQ_HZ>(frequency);
  }
  unsigned int frequency_;
  double duty;
};
} // namespace nit::embedded::drivers

#endif