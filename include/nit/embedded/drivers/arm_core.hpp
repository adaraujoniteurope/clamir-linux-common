#ifndef nit_embedded_drivers_arm_core_hpp_
#define nit_embedded_drivers_arm_core_hpp_

#include <nit/embedded/drivers/memory_mapped_device.hpp>

namespace nit::embedded::drivers
{

    class arm_core_config : public config_base
    {

    };

    template<>
    struct is_memory_mapeed_config_type<arm_core_config>
    {
        static const bool value = true;
    };

    class arm_core : public memory_mapped_device<arm_core, arm_core_config>
    {
    public:

        MEMORY_MAPPED_DRIVER_DECLARE_ACCESSORS(led_r, io_value, 0x0000)
        MEMORY_MAPPED_DRIVER_DECLARE_ACCESSORS(led_g, io_value, 0x0000)
        MEMORY_MAPPED_DRIVER_DECLARE_ACCESSORS(led_b, io_value, 0x0000)

        arm_core(const arm_core_config& _conf, std::shared_ptr<memory_mapped_device<arm_core, arm_core_config>> _base = nullptr)
        {
            m_config = _conf;
            m_base = _base;
        }

        template<typename archiver>
        void serialize(const archiver& ar, const int version)
        {
            io_value _led_r = led_r_get();
            io_value _led_g = led_g_get();
            io_value _led_b = led_b_get();

            ar& boost::serialization::make_nvp("led_r", _led_r);
            ar& boost::serialization::make_nvp("led_g", _led_g);
            ar& boost::serialization::make_nvp("led_b", _led_b);

            led_r_set(_led_r);
            led_g_set(_led_g);
            led_b_set(_led_b);
        }

    };
}

#endif