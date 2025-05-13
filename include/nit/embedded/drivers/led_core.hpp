#ifndef nit_embedded_drivers_arm_core_hpp_
#define nit_embedded_drivers_arm_core_hpp_

#include <nit/embedded/drivers/memory_mapped_device.hpp>
#include <nit/embedded/drivers/pwm_core.hpp>

#include <nit/embedded/utils/bitfield.h>

using namespace nit::embedded::drivers;
using namespace nit::embedded::utils;

namespace nit::embedded::drivers {

struct led_core_config : public config_base {
  size_t base_address = 0x40001000;
  size_t size = 4096;

  uint16_t led_r_frequency = 20000;
  double led_r_duty = 0.5;

  uint16_t led_g_frequency = 20000;
  double led_g_duty = 0.5;

  uint16_t led_b_frequency = 20000;
  double led_b_duty = 0.5;

  template <typename archiver>
  void serialize(const archiver &ar, const int version) {
    ar &boost::serialization::make_nvp("led_r_frequency", led_r_frequency);
    ar &boost::serialization::make_nvp("led_r_duty", led_r_duty);

    ar &boost::serialization::make_nvp("led_g_frequency", led_g_frequency);
    ar &boost::serialization::make_nvp("led_g_duty", led_g_duty);

    ar &boost::serialization::make_nvp("led_b_frequency", led_b_frequency);
    ar &boost::serialization::make_nvp("led_b_duty", led_b_duty);
  }
};

class led_core : public memory_mapped_device<led_core> {
public:
  led_core(size_t offset, size_t size, std::string path = "/dev/mem")
      : memory_mapped_device<led_core>(offset, size, path, nullptr) {}

  led_core(led_core_config &config)
      : memory_mapped_device<led_core>(config.offset, config.size, config.path,
                                       nullptr) {
    _config = config;

    write<uint16_t>(
        1, frequency_to_prescaler_value(_config.led_g_frequency));
    write<uint16_t>(
        5, frequency_to_prescaler_value(_config.led_g_frequency));
    write<uint16_t>(
        3, frequency_to_prescaler_value(_config.led_b_frequency));

    write<uint16_t>(0, duty_to_prescaler_value(_config.led_r_duty,
                                                      _config.led_r_frequency));
    write<uint16_t>(4, duty_to_prescaler_value(_config.led_g_duty,
                                                      _config.led_g_frequency));
    write<uint16_t>(2, duty_to_prescaler_value(_config.led_b_duty,
                                                      _config.led_b_frequency));
  }

  void frequency_all_set(unsigned int frequency = 20.000) {
    write<uint16_t>(1, frequency_to_prescaler_value(frequency));
    write<uint16_t>(5, frequency_to_prescaler_value(frequency));
    write<uint16_t>(3, frequency_to_prescaler_value(frequency));
    _frequency = frequency;
  }

  void led_r_frequency_set(int frequency) {
    write<uint16_t>(1, frequency_to_prescaler_value(frequency));
  }

  int led_r_frequency_get() { return read<uint16_t>(1); }

  void led_g_frequency_set(int frequency) {
    write<uint16_t>(3, frequency_to_prescaler_value(frequency));
  }

  int led_g_frequency_get() { return read<uint16_t>(3); }

  void led_b_frequency_set(int frequency) {
    write<uint16_t>(5, frequency_to_prescaler_value(frequency));
  }

  int led_b_frequency_get() { return read<uint16_t>(5); }

  void led_r_set(double duty = 1.0) {
    if (duty > 1.0) {
      duty = 1.0;
    }

    if (duty < 0.0) {
      duty = 0.0;
    }

    write<uint16_t>(0, duty_to_prescaler_value(1.0 - duty, _frequency));
  }

  void led_g_set(double duty = 1.0) {

    if (duty > 1.0) {
      duty = 1.0;
    }

    if (duty < 0.0) {
      duty = 0.0;
    }

    write<uint16_t>(4, duty_to_prescaler_value(1.0 - duty, _frequency));
  }

  void led_b_set(double duty = 1.0) {

    if (duty > 1.0) {
      duty = 1.0;
    }

    if (duty < 0.0) {
      duty = 0.0;
    }

    write<uint16_t>(2, duty_to_prescaler_value(1.0 - duty, _frequency));
  }

  double led_r_get() { return read<uint16_t>(0); }

  double led_g_get() { return read<uint16_t>(4); }

  double led_b_get() { return read<uint16_t>(2); }

private:
  template <uint32_t S_AXI_FREQ_HZ = 100000000>
  static uint16_t frequency_to_prescaler_value(double frequency) {
    return ((uint16_t)(((double)S_AXI_FREQ_HZ) / frequency));
  }

  template <uint32_t S_AXI_FREQ_HZ = 100000000>
  static uint16_t duty_to_prescaler_value(double duty, double frequency) {
    return duty * frequency_to_prescaler_value<S_AXI_FREQ_HZ>(frequency);
  }

  unsigned int _frequency;
  led_core_config _config;
};
} // namespace nit::embedded::drivers

#endif