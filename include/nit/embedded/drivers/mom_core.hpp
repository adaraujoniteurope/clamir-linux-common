#ifndef nit_embedded_drivers_mom_core_hpp_
#define nit_embedded_drivers_mom_core_hpp_

#include <nit/embedded/drivers/memory_mapped_device.hpp>
#include <nit/embedded/drivers/pwm_core.hpp>
#include <nit/embedded/drivers/utils.hpp>

#include <nit/embedded/utils/bitfield.h>

using namespace nit::embedded::drivers;

namespace nit::embedded::drivers
{
    class mom_core : public memory_mapped_device<mom_core>
    {
    public:

        mom_core(size_t offset, size_t size, std::string path = "/dev/mem") : memory_mapped_device<mom_core>(offset, size, path, nullptr)
        {
        }

        int threshold_get() { return read<uint32_t>(0x4000 + 0); }
        void threshold_set(int value) { return write<uint32_t>(0x4000 + 0, value); }

        int intensity_min_get() { return read<uint32_t>(0x4000 + 1); }
        void intensity_min_set(int value) { return write<uint32_t>(0x4000 + 1, value); }

        int intensity_max_get() { return read<uint32_t>(0x4000 + 2); }
        void intensity_max_set(int value) { return write<uint32_t>(0x4000 + 2, value); }

        int mom_00_get() { return read<uint32_t>(0x4000 + 3); }
        void mom_00_set(int value) { return write<uint32_t>(0x4000 + 3, value); }

        int mom_01_get() { return read<uint32_t>(0x4000 + 4); }
        void mom_01_set(int value) { return write<uint32_t>(0x4000 + 4, value); }

        int mom_10_get() { return read<uint32_t>(0x4000 + 5); }
        void mom_10_set(int value) { return write<uint32_t>(0x4000 + 5, value); }

        int mom_11_get() { return read<uint32_t>(0x4000 + 6); }
        void mom_11_set(int value) { return write<uint32_t>(0x4000 + 6, value); }

        int mom_02_get() { return read<uint32_t>(0x4000 + 7); }
        void mom_02_set(int value) { return write<uint32_t>(0x4000 + 7, value); }

        int mom_20_get() { return read<uint32_t>(0x4000 + 8); }
        void mom_20_set(int value) { return write<uint32_t>(0x4000 + 8, value); }

        int start_track_mom_t_get() { return read<uint32_t>(0x4000 + 9); }
        void start_track_mom_t_set(int value) { return write<uint32_t>(0x4000 + 9, value); }

        int track_nbr_get() { return read<uint32_t>(0x4000 + 10); }
        void track_nbr_set(int value) { return write<uint32_t>(0x4000 + 10, value); }

        int mode_get() { return read<uint32_t>(0x4000 + 11); }
        void mode_set(int value) { return write<uint32_t>(0x4000 + 11, value); }

        int time_track_low_get() { return read<uint32_t>(0x4000 + 12); }
        void time_track_low_set(int value) { return write<uint32_t>(0x4000 + 12, value); }

        int time_track_high_get() { return read<uint32_t>(0x4000 + 13); }
        void time_track_high_set(int value) { return write<uint32_t>(0x4000 + 13, value); }

        int frame_max_get() { return read<uint32_t>(0x4000 + 14); }
        void frame_max_set(int value) { return write<uint32_t>(0x4000 + 14, value); }

        int reference_track_get() { return read<uint32_t>(0x4000 + 15); }
        void reference_track_set(int value) { return write<uint32_t>(0x4000 + 15, value); }

        int end_of_track_get() { return read<uint32_t>(0x4000 + 16); }
        void end_of_track_set(int value) { return write<uint32_t>(0x4000 + 16, value); }

    };
}

#endif