#ifndef nit_embedded_drivers_gen_core_hpp_
#define nit_embedded_drivers_gen_core_hpp_

#include <nit/embedded/drivers/memory_mapped_device.hpp>
#include <nit/embedded/drivers/pwm_core.hpp>
#include <nit/embedded/drivers/utils.hpp>

#include <nit/embedded/utils/bitfield.h>

using namespace nit::embedded::drivers::utils;

namespace nit::embedded::drivers
{
    class gen_core : public memory_mapped_device<gen_core>
    {
    public:

        struct gen_core_config : public config_base
        {

            int version_id = 0;
            int enable_mom = 0;
            int enable_roi = 0;
            int enable_pwm = 0;
            int enable_irq = 0;
            int change_op_mode = 0;
            int irq_microblaze = 0;
            int digital_in_0 = 0;
            int digital_in_1 = 0;
            int digital_out_0 = 0;
            int digital_out_1 = 0;
            int digital_out_conf = 0;
            int digital_in_2 = 0;
            int digital_in_3 = 0;
            int digital_out_2 = 0;
            int digital_out_3 = 0;

            template<typename archiver>
            void serialize(const archiver& ar, const int version)
            {
                ar & boost::serialization::make_nvp("version_id", version_id);
                ar & boost::serialization::make_nvp("enable_mom", enable_mom);
                ar & boost::serialization::make_nvp("enable_roi", enable_roi);
                ar & boost::serialization::make_nvp("enable_pwm", enable_pwm);
                ar & boost::serialization::make_nvp("enable_irq", enable_irq);
                ar & boost::serialization::make_nvp("change_op_mode", change_op_mode);
                ar & boost::serialization::make_nvp("irq_microblaze", irq_microblaze);
                ar & boost::serialization::make_nvp("digital_in_0", digital_in_0);
                ar & boost::serialization::make_nvp("digital_in_1", digital_in_1);
                ar & boost::serialization::make_nvp("digital_out_0", digital_out_0);
                ar & boost::serialization::make_nvp("digital_out_1", digital_out_1);
                ar & boost::serialization::make_nvp("digital_out_conf", digital_out_conf);
                ar & boost::serialization::make_nvp("digital_in_2", digital_in_2);
                ar & boost::serialization::make_nvp("digital_in_3", digital_in_3);
                ar & boost::serialization::make_nvp("digital_out_2", digital_out_2);
                ar & boost::serialization::make_nvp("digital_out_3", digital_out_3);
            }
        };

        gen_core(size_t offset = 0x50000000, size_t size = 0x00010000, std::string path = "/dev/mem") : memory_mapped_device<gen_core>(offset, size, path, nullptr)
        {
        }

        int version_id_get() { return read<uint32_t>(0); }
        void version_id_set(int value) { return write<uint32_t>(0, value); }

        int enable_mom_get() { return read<uint32_t>(1); }
        void enable_mom_set(int value) { return write<uint32_t>(1, value); }

        int enable_roi_get() { return read<uint32_t>(2); }
        void enable_roi_set(int value) { return write<uint32_t>(2, value); }

        int enable_pwm_get() { return read<uint32_t>(3); }
        void enable_pwm_set(int value) { return write<uint32_t>(3, value); }

        int enable_irq_get() { return read<uint32_t>(4); }
        void enable_irq_set(int value) { return write<uint32_t>(4, value); }

        int change_op_mode_get() { return read<uint32_t>(5); }
        void change_op_mode_set(int value) { return write<uint32_t>(5, value); }

        int irq_microblaze_get() { return read<uint32_t>(6); }
        void irq_microblaze_set(int value) { return write<uint32_t>(6, value); }

        int digital_in_0_get() { return read<uint32_t>(7); }
        void digital_in_0_set(int value) { return write<uint32_t>(7, value); }

        int digital_in_1_get() { return read<uint32_t>(8); }
        void digital_in_1_set(int value) { return write<uint32_t>(8, value); }

        int digital_out_0_get() { return read<uint32_t>(9); }
        void digital_out_0_set(int value) { return write<uint32_t>(9, value); }

        int digital_out_1_get() { return read<uint32_t>(10); }
        void digital_out_1_set(int value) { return write<uint32_t>(10, value); }

        int digital_out_conf_get() { return read<uint32_t>(11); }
        void digital_out_conf_set(int value) { return write<uint32_t>(11, value); }

        int digital_in_2_get() { return read<uint32_t>(12); }
        void digital_in_2_set(int value) { return write<uint32_t>(12, value); }

        int digital_in_3_get() { return read<uint32_t>(13); }
        void digital_in_3_set(int value) { return write<uint32_t>(13, value); }

        int digital_out_2_get() { return read<uint32_t>(14); }
        void digital_out_2_set(int value) { return write<uint32_t>(14, value); }

        int digital_out_3_get() { return read<uint32_t>(15); }
        int digital_out_3_set(int value) { write<uint32_t>(15, value); }

    };
}

#endif