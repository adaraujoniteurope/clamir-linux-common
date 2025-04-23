#include <nit/embedded/application.hpp>

#include <map>
#include <iostream>
#include <thread>
#include <csignal>
#include <atomic>
#include <cmath>
#include <string>

#include <future>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include <nit/embedded/components/timer.hpp>

#include <nit/embedded/drivers/arm_core.h>
#include <nit/embedded/drivers/bpc_table_core.h>
#include <nit/embedded/drivers/scc_core.h>
#include <nit/embedded/drivers/control_unit_core.h>

#include <nit/embedded/drivers/common.h>
#include <nit/embedded/drivers/arm_core.h>
#include <nit/embedded/drivers/control_unit_core.h>
#include <nit/embedded/drivers/roi_core_field_table.h>
#include <nit/embedded/drivers/gen_core_field_table.h>
#include <nit/embedded/drivers/pwm_core_field_table.h>
#include <nit/embedded/drivers/mom_core_field_table.h>

#include <nit/embedded/drivers/process_core_field_table.h>
#include <nit/embedded/drivers/process_core.h>

#include <nit/embedded/networking/tcp/protocol_legacy.hpp>

#include <nit/embedded/math/algorithm.hpp>

#include <nit/embedded/vision/frame_generator.hpp>
#include <nit/embedded/drivers/scc_core.h>

#include <boost/signals2.hpp>

#include <sys/fcntl.h>
#include <sys/mman.h>

int send_response(int fd, packet& req)
{
    std::cout << "response:" << req << std::endl;
    auto data = packet::encode(req);
    return ::write(fd, &data, sizeof(data));
}

#define DEFINE_COMMAND_TARGET_WRITE_CALLBACK(driver_type, prefix, var_type, var)                                                              \
    int command_target_##prefix##_##var##_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd) \
    {                                                                                                                                         \
        std::cout << __func__ << std::endl;                                                                                                   \
                                                                                                                                              \
        prefix##_##var##_set((driver_type##_state_t *)route.pdata, req.value);                                                                \
                                                                                                                                              \
        var_type value = req.value;                                                                                                           \
        prefix##_##var##_get((driver_type##_state_t *)route.pdata, &value);                                                                   \
        if (req.value != value)                                                                                                               \
        {                                                                                                                                     \
            std::cout << __func__ << ": " << std::format("failed to set value {} != {}", req.value, value) << std::endl;                      \
        }                                                                                                                                     \
                                                                                                                                              \
        return 0;                                                                                                                             \
    }

#define DEFINE_COMMAND_TARGET_READ_CALLBACK(driver_type, prefix, var_type, var)                                                              \
    int command_target_##prefix##_##var##_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd) \
    {                                                                                                                                        \
        std::cout << __func__ << std::endl;                                                                                                  \
                                                                                                                                             \
        var_type value = req.value;                                                                                                          \
        prefix##_##var##_get((driver_type##_state_t *)route.pdata, &value);                                                                  \
        req.value = value;                                                                                                                   \
                                                                                                                                             \
        send_response(socket_fd, req);                                                                                       \
                                                                                                                                             \
        return 0;                                                                                                                            \
    }

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, int_time)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, int_time)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, bias_v)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, bias_v)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, offset_en)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, offset_en)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, offset_update)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, offset_update)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, shutter)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, shutter)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, bpc_en)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, bpc_en)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, bpc_mem_write)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, bpc_mem_write)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, bpc_identify)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, bpc_identify)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, temp1)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, temp1)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, temp2)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, temp2)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, temp3)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, temp3)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, temp4)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, temp4)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, trigger_usec)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, trigger_usec)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, black_level)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, black_level)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, sincronization)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, sincronization)

int command_target_nit_control_unit_core_save_embedded_conf_write(std::shared_ptr<application> app, command_processor_route& route, packet& req, int socket_fd)
{
    std::cout << __func__ << std::endl;
    app->save_all();
    return 0;
}

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, save_embedded_conf)

int command_target_nit_control_unit_core_arm_sw_version_write(std::shared_ptr<application> app, command_processor_route& route, packet& req, int socket_fd)
{
    std::cout << __func__ << std::endl;
    return 0;
}

int command_target_nit_control_unit_core_arm_sw_version_read(std::shared_ptr<application> app, command_processor_route& route, packet& req, int socket_fd)
{
    std::cout << __func__ << std::endl;
    req.value = 0x0006;
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_control_unit_core_drift_enable_write(std::shared_ptr<application> app, command_processor_route& route, packet& req, int socket_fd)
{
    std::cout << __func__ << std::endl;
    nit_control_unit_core_drift_enable_set((nit_control_unit_core_state_t*)route.pdata, req.value);
    uint16_t value = req.value;
    nit_control_unit_core_drift_enable_get((nit_control_unit_core_state_t*)route.pdata, &value);
    return 0;
}

int command_target_nit_control_unit_core_drift_enable_read(std::shared_ptr<application> app, command_processor_route& route, packet& req, int socket_fd)
{
    std::cout << __func__ << std::endl;
    uint16_t value = req.value;
    nit_control_unit_core_drift_enable_get((nit_control_unit_core_state_t*)route.pdata, &value);
    req.value = value;
    send_response(socket_fd, req);
    return 0;
}

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, drift_position)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, drift_position)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, drift_level)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, drift_level)

int command_target_nit_control_unit_core_magic_id_write(std::shared_ptr<application> app, command_processor_route& route, packet& req, int socket_fd)
{
    std::cout << __func__ << std::endl;
    ::write(socket_fd, nullptr, 0);
    return 0;
}

int command_target_nit_control_unit_core_magic_id_read(std::shared_ptr<application> app, command_processor_route& route, packet& req, int socket_fd)
{
    std::cout << __func__ << std::endl;
    req.value = 0x0700;
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_control_unit_core_fpga_version_write(std::shared_ptr<application> app, command_processor_route& route, packet& req, int socket_fd)
{
    std::cout << __func__ << std::endl;
    ::write(socket_fd, nullptr, 0);
    return 0;
}

int command_target_nit_control_unit_core_fpga_version_read(std::shared_ptr<application> app, command_processor_route& route, packet& req, int socket_fd)
{
    std::cout << __func__ << std::endl;
    req.value = 0x0002;
    send_response(socket_fd, req);
    return 0;
}

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, start_track_mom_t)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, start_track_mom_t)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, end_of_track)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, end_of_track)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, mode)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, mode)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, reference_track)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, reference_track)

// DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_mom_core, uint32_t, time_track_low)
// DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_mom_core, uint32_t, time_track_low)
#include <nit/embedded/drivers/common.h>

int command_target_nit_mom_core_time_track_low_read(std::shared_ptr<application> app, command_processor_route& route, packet& req, int socket_fd)
{
    std::cout << __func__ << std::endl;

    uint32_t high = 0;
    uint32_t low = 0;

    // high = unsafe_get<nit_mb_core_state_t, uint32_t>(&nit_mb_core_driver, nit_mom_core_time_track_high_offset);
    // high = unsafe_get<nit_mb_core_state_t, uint32_t>(&nit_mb_core_driver, nit_mom_core_time_track_low_offset);

    if (nit_mom_core_time_track_high_get(&nit_mb_core_driver, &high) < 0)
    {
        return -1;
    }

    if (nit_mom_core_time_track_low_get(&nit_mb_core_driver, &low) < 0)
    {
        return -1;
    }

    uint16_t value = ((((uint64_t)high << 32) | ((uint64_t)low << 0)) / 100e5);

    req.value = value;

    send_response(socket_fd, req);

    return 0;
}

int command_target_nit_mom_core_time_track_low_write(std::shared_ptr<application> app, command_processor_route& route, packet& req, int socket_fd)
{
    std::cout << __func__ << std::endl;

    uint64_t value = 0;
    value = req.value;
    value *= 100e5;

    if (nit_mom_core_time_track_high_set(&nit_mb_core_driver, (value & 0xFFFFFFFF00000000) >> 32) < 0)
    {
        return -1;
    }

    if (nit_mom_core_time_track_low_set(&nit_mb_core_driver, (value & 0x00000000FFFFFFFF) >> 0) < 0)
    {
        return -2;
    }

    return 0;
}

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_arm_core, nit_arm_core, uint16_t, soft_reset)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_arm_core, nit_arm_core, uint16_t, soft_reset)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, threshold)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, threshold)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_pwm_core, uint16_t, pwm)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_pwm_core, uint16_t, pwm)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, round)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, round)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_gen_core, uint16_t, enable_roi)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_gen_core, uint16_t, enable_roi)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, x1)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, x1)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, y1)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, y1)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, x2)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, x2)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, y2)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, y2)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_arm_core, nit_arm_core, uint16_t, led_r)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_arm_core, nit_arm_core, uint16_t, led_r)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_arm_core, nit_arm_core, uint16_t, led_g)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_arm_core, nit_arm_core, uint16_t, led_g)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_arm_core, nit_arm_core, uint16_t, led_b)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_arm_core, nit_arm_core, uint16_t, led_b)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_pwm_core, uint16_t, pwm_limit_max)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_pwm_core, uint16_t, pwm_limit_max)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_pwm_core, uint16_t, pwm_limit_min)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_pwm_core, uint16_t, pwm_limit_min)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_gen_core, uint16_t, digital_out_conf)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_gen_core, uint16_t, digital_out_conf)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_gen_core, uint16_t, digital_out_0)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_gen_core, uint16_t, digital_out_0)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_gen_core, uint16_t, digital_out_1)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_gen_core, uint16_t, digital_out_1)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_gen_core, uint16_t, digital_out_2)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_gen_core, uint16_t, digital_out_2)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_gen_core, uint16_t, digital_out_3)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_gen_core, uint16_t, digital_out_3)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, kp)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, kp)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, ki)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, ki)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, kd)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, kd)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, max_power)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, max_power)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, min_power)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, min_power)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, power_man)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, power_man)

int command_target_scc_core_calibrate_read(std::shared_ptr<application> app, command_processor_route& route, packet& req, int socket_fd)
{
    std::cout << __func__ << std::endl;
    uint32_t value = req.value;
    req.value = value;
    send_response(socket_fd, req);
    return 0;
}

int command_target_scc_core_calibrate_write(std::shared_ptr<application> app, command_processor_route& route, packet& req, int socket_fd)
{
    nit_scc_core_calibrate(&nit_scc_core_driver);
    return 0;
}

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, background_remove)
// DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, background_remove)

int command_target_nit_process_core_background_remove_write(std::shared_ptr<application> app, command_processor_route& route, packet& req, int socket_fd)
{

    if(!app->config_get().sensor_calibation_disable)
    {
        app->sensor_calibrate();
    }

    nit_control_unit_core_offset_en_set(&nit_control_unit_core_driver, 1);
    nit_control_unit_core_offset_update_set(&nit_control_unit_core_driver, 1);
    
    nit_control_unit_core_shutter_set(&nit_control_unit_core_driver, 1);

    if (app->host_mockup) {
        nit_framebuffer_core_operating_mode_set(&nit_framebuffer_core_driver, NIT_FRAMEBUFFER_CORE_OPERATING_MODE_TEST_UNIFORM_SHUTTER_CLOSED);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    nit_control_unit_core_shutter_set(&nit_control_unit_core_driver, 0);

    if (app->host_mockup) {
        nit_framebuffer_core_operating_mode_set(&nit_framebuffer_core_driver, NIT_FRAMEBUFFER_CORE_OPERATING_MODE_TEST_UNIFORM_SHUTTER_OPEN);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    nit_control_unit_core_offset_update_set(&nit_control_unit_core_driver, 0);

    if (app->host_mockup) {
        nit_framebuffer_core_operating_mode_set(&nit_framebuffer_core_driver, NIT_FRAMEBUFFER_CORE_OPERATING_MODE_TEST_PATTERN_BALL);
    }

    return 0;
}

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, set_ref_width)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, set_ref_width)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, power_limit_max)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, power_limit_max)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, power_limit_min)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, power_limit_min)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, width_ref)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, width_ref)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, pixel_mm_ratio)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, pixel_mm_ratio)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, pid_error)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, pid_error)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, end_of_process)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, end_of_process)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, limit_integral)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, limit_integral)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, limit_slew)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, limit_slew)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, buff_size)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, buff_size)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, enable_alarm)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, enable_alarm)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, alarm_max)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, alarm_max)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, alarm_min)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, alarm_min)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, alarm_time)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, alarm_time)

int command_target_nit_gen_core_serial_number_low_write(std::shared_ptr<application> app, command_processor_route& route, packet& req, int socket_fd)
{
    std::cout << __func__ << std::endl;
    return 0;
}

int command_target_nit_gen_core_serial_number_low_read(std::shared_ptr<application> app, command_processor_route& route, packet& req, int socket_fd)
{
    std::cout << __func__ << std::endl;

    req.route_id_set(0x04aa);
    req.value = 0x3231;

    send_response(socket_fd, req);

    req.route_id_set(0x04ab);
    req.value = 0x4541;

    send_response(socket_fd, req);

    req.route_id_set(0x04ac);
    req.value = 0x3434;

    send_response(socket_fd, req);

    req.route_id_set(0x04ad);
    req.value = 0x0037;

    send_response(socket_fd, req);

    return 0;
}

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, serial_number_low)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, serial_number_low)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, automeasure)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, automeasure)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, autoshutter_config)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, autoshutter_config)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, autoshutter_temp)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, autoshutter_temp)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, autoshutter_timer)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, autoshutter_timer)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, track_ref_start)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, track_ref_start)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, laser_external_control)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, laser_external_control)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, delay_laser_on)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, delay_laser_on)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, preheating_ena)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, preheating_ena)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, preheating_time)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, preheating_time)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, preheating_power)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, preheating_power)

std::map<uint16_t, command_processor_route> application::command_processor_routes_legacy = {

    {0x0501, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_int_time_read, .write = command_target_nit_control_unit_core_int_time_write}},
    {0x0502, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_bias_v_read, .write = command_target_nit_control_unit_core_bias_v_write}},
    {0x0503, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_offset_en_read, .write = command_target_nit_control_unit_core_offset_en_write}},
    {0x0504, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_offset_update_read, .write = command_target_nit_control_unit_core_offset_update_write}},
    {0x0505, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_shutter_read, .write = command_target_nit_control_unit_core_shutter_write}},
    {0x0506, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_bpc_en_read, .write = command_target_nit_control_unit_core_bpc_en_write}},
    {0x0507, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_bpc_mem_write_read, .write = command_target_nit_control_unit_core_bpc_mem_write_write}},
    {0x0508, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_bpc_identify_read, .write = command_target_nit_control_unit_core_bpc_identify_write}},
    {0x0509, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_temp1_read, .write = command_target_nit_control_unit_core_temp1_write}},
    {0x050A, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_temp2_read, .write = command_target_nit_control_unit_core_temp2_write}},
    {0x050B, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_temp3_read, .write = command_target_nit_control_unit_core_temp3_write}},
    {0x050C, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_temp4_read, .write = command_target_nit_control_unit_core_temp4_write}},

    {0x050D, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_drift_enable_read, .write = command_target_nit_control_unit_core_drift_enable_write}},
    {0x050E, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_drift_position_read, .write = command_target_nit_control_unit_core_drift_position_write}},
    {0x050F, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_drift_level_read, .write = command_target_nit_control_unit_core_drift_level_write}},

    {0x0520, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_trigger_usec_read, .write = command_target_nit_control_unit_core_trigger_usec_write}},
    {0x0521, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_black_level_read, .write = command_target_nit_control_unit_core_black_level_write}},
    {0x0522, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_sincronization_read, .write = command_target_nit_control_unit_core_sincronization_write}},
    {0x0523, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_save_embedded_conf_read, .write = command_target_nit_control_unit_core_save_embedded_conf_write}},

    {0x0524, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_arm_sw_version_read, .write = command_target_nit_control_unit_core_arm_sw_version_write}},
    {0x0525, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_magic_id_read, .write = command_target_nit_control_unit_core_magic_id_write}},
    {0x0526, {.pdata = (void*)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_fpga_version_read, .write = command_target_nit_control_unit_core_fpga_version_write}},

    {0x0402, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_kp_read, .write = command_target_nit_process_core_kp_write}},
    {0x0401, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_ki_read, .write = command_target_nit_process_core_ki_write}},
    {0x0403, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_kd_read, .write = command_target_nit_process_core_kd_write}},

    {0x0404, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_max_power_read, .write = command_target_nit_process_core_max_power_write}},
    {0x0405, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_min_power_read, .write = command_target_nit_process_core_min_power_write}},
    {0x0407, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_power_man_read, .write = command_target_nit_process_core_power_man_write}},
    {0x0409, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_background_remove_read, .write = command_target_nit_process_core_background_remove_write}},

    {0x040D, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_set_ref_width_read, .write = command_target_nit_process_core_set_ref_width_write}},
    {0x0418, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_power_limit_max_read, .write = command_target_nit_process_core_power_limit_max_write}},
    {0x0419, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_power_limit_min_read, .write = command_target_nit_process_core_power_limit_min_write}},
    {0x041F, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_width_ref_read, .write = command_target_nit_process_core_width_ref_write}},
    {0x0420, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_pixel_mm_ratio_read, .write = command_target_nit_process_core_pixel_mm_ratio_write}},
    {0x0421, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_pid_error_read, .write = command_target_nit_process_core_pid_error_write}},
    {0x0422, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_end_of_process_read, .write = command_target_nit_process_core_end_of_process_write}},
    {0x0423, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_limit_integral_read, .write = command_target_nit_process_core_limit_integral_write}},
    {0x0424, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_limit_slew_read, .write = command_target_nit_process_core_limit_slew_write}},
    {0x0425, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_buff_size_read, .write = command_target_nit_process_core_buff_size_write}},
    {0x04A5, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_buff_size_read, .write = command_target_nit_process_core_buff_size_write}},
    {0x0426, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_enable_alarm_read, .write = command_target_nit_process_core_enable_alarm_write}},
    {0x0427, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_alarm_max_read, .write = command_target_nit_process_core_alarm_max_write}},
    {0x0428, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_alarm_min_read, .write = command_target_nit_process_core_alarm_min_write}},
    {0x0429, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_alarm_time_read, .write = command_target_nit_process_core_alarm_time_write}},
    {0x042A, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_gen_core_serial_number_low_read, .write = command_target_nit_gen_core_serial_number_low_write}},
    {0x042E, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_automeasure_read, .write = command_target_nit_process_core_automeasure_write}},
    {0x042F, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_autoshutter_config_read, .write = command_target_nit_process_core_autoshutter_config_write}},
    {0x0431, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_autoshutter_temp_read, .write = command_target_nit_process_core_autoshutter_temp_write}},
    {0x0432, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_autoshutter_timer_read, .write = command_target_nit_process_core_autoshutter_timer_write}},
    {0x0433, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_track_ref_start_read, .write = command_target_nit_process_core_track_ref_start_write}},
    {0x0434, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_laser_external_control_read, .write = command_target_nit_process_core_laser_external_control_write}},
    {0x0435, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_delay_laser_on_read, .write = command_target_nit_process_core_delay_laser_on_write}},
    {0x0436, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_preheating_ena_read, .write = command_target_nit_process_core_preheating_ena_write}},
    {0x0437, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_preheating_time_read, .write = command_target_nit_process_core_preheating_time_write}},
    {0x0438, {.pdata = (void*)&nit_process_core_driver, .read = command_target_nit_process_core_preheating_power_read, .write = command_target_nit_process_core_preheating_power_write}},
    {0x0406, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_mom_core_start_track_mom_t_read, .write = command_target_nit_mom_core_start_track_mom_t_write}},
    {0x040a, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_mom_core_mode_read, .write = command_target_nit_mom_core_mode_write}},
    {0x0408, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_mom_core_end_of_track_read, .write = command_target_nit_mom_core_end_of_track_write}},

    {0x040B, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_mom_core_reference_track_read, .write = command_target_nit_mom_core_reference_track_write}},
    {0x040C, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_mom_core_time_track_low_read, .write = command_target_nit_mom_core_time_track_low_write}},
    {0x040E, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_arm_core_soft_reset_read, .write = command_target_nit_arm_core_soft_reset_write}},
    {0x040F, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_mom_core_threshold_read, .write = command_target_nit_mom_core_threshold_write}},
    
    {0x0410, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_roi_core_round_read, .write = command_target_nit_roi_core_round_write}},
    {0x0411, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_gen_core_enable_roi_read, .write = command_target_nit_gen_core_enable_roi_write}},
    {0x0412, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_roi_core_x1_read, .write = command_target_nit_roi_core_x1_write}},
    {0x0413, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_roi_core_y1_read, .write = command_target_nit_roi_core_y1_write}},
    {0x0414, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_roi_core_x2_read, .write = command_target_nit_roi_core_x2_write}},
    {0x0415, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_roi_core_y2_read, .write = command_target_nit_roi_core_y2_write}},
    {0x0416, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_mom_core_mode_read, .write = command_target_nit_mom_core_mode_write}},
    {0x041A, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_arm_core_led_r_read, .write = command_target_nit_arm_core_led_r_write}},
    {0x041B, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_arm_core_led_g_read, .write = command_target_nit_arm_core_led_g_write}},
    {0x041C, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_arm_core_led_b_read, .write = command_target_nit_arm_core_led_b_write}},
    {0x0417, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_pwm_core_pwm_read, .write = command_target_nit_pwm_core_pwm_write}},
    {0x041D, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_pwm_core_pwm_limit_max_read, .write = command_target_nit_pwm_core_pwm_limit_max_write}},
    {0x041E, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_pwm_core_pwm_limit_min_read, .write = command_target_nit_pwm_core_pwm_limit_min_write}},
    {0x0430, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_gen_core_digital_out_conf_read, .write = command_target_nit_gen_core_digital_out_conf_write}},
    {0x0439, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_gen_core_digital_out_0_read, .write = command_target_nit_gen_core_digital_out_0_write}},
    {0x043A, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_gen_core_digital_out_1_read, .write = command_target_nit_gen_core_digital_out_1_write}},
    {0x043B, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_gen_core_digital_out_2_read, .write = command_target_nit_gen_core_digital_out_2_write}},
    {0x043C, {.pdata = (void*)&nit_mb_core_driver, .read = command_target_nit_gen_core_digital_out_3_read, .write = command_target_nit_gen_core_digital_out_3_write}} };

void application::command_processor_legacy(int socket_fd)
{
    auto app = application::get_instance();

    unsigned char data[4096];

    int length = 0;
    int client_retries = 0;

    std::mutex m;

    while (!m_shutdown & (client_retries < 3))
    {
        // auto bytes_to_read = sizeof(data) - (ptr - data);

        // if (bytes_to_read == 0)
        // {
        //     std::cout << "buffer is full and couldn't parse any packets." << std::endl;
        //     std::cout << "dropping connection." << std::endl;
        //     break;
        // }

        length = read(socket_fd, data, sizeof(data));

        if (length < 0)
        {
            break;
        }

        if (length == 0)
        {
            client_retries++;
            continue;
        }

        if (length % 4 != 0)
        {
            // assume malformed packet
            continue;
        }

        // auto packets = std::vector<packet>(data, data + length);

        std::vector<packet> packets;

        for (auto it = data; (it + 4) <= (data + length); it += 4)
        {
            packet p = *(uint32_t*)it;
            packets.push_back(p);
        }

        client_retries = 0;

        for (auto& p : packets)
        {
            std::cout << "request: " << p << std::endl;

            auto route_id = p.route_id_get();
            // auto rw = p.rw.get();

            auto it = command_processor_routes_legacy.find(route_id);

            if (it == command_processor_routes_legacy.end())
            {
                printf("couldn't find route %04x\n", route_id);
                // ::write(socket_fd, (uint8_t *)&p, sizeof(p));
                continue;
            }

            auto& [id, route] = *it;

            switch (p.rw)
            {
            case 0:

                if (route.write == nullptr)
                    break;
                route.write(app, route, p, socket_fd);

                break;
            case 1:
                if (route.read == nullptr)
                    break;
                route.read(app, route, p, socket_fd);
                break;

            default:
                break;
            }
        }

        // ptr = data;
    }

    close(socket_fd);
}

struct __attribute__((packed)) metadata_process
{
    int power;
    int m00;
    int m01;
    int m10;
    int m11;
    int m02;
    int m20;
    int width;
    int track_number;
    int frame_max;
    int frame_number;
    int timestamp;
    int io_status;
    int t1;
    int t2;
};

struct __attribute__((packed)) metadata_frame
{
    int power;
    int m00;
    int m01;
    int m10;
    int m11;
    int m02;
    int m20;
    // int width;
    int track_number;
    int frame_max;
    int frame_number;
    int timestamp;
    int io_status;
    // int t1;
    // int t2;
};

struct __attribute__((packed)) mm_image_writer_ctrl
{
    int fifo_head;
    int intr;
};

void application::image_writer_legacy(int socket_fd)
{

    int retval = 0;
    const auto FIFO_LENGTH = 6;

    uint8_t* img_ptr = (uint8_t*)nit_framebuffer_core_get_memory_map(&nit_framebuffer_core_driver);
    uint8_t* pmeta_ptr = (uint8_t*)nit_process_core_get_virtual_metadata_shm_ptr(&nit_process_core_driver);

    int fifo_ctrl_fd = open("/dev/mem", O_RDWR | O_SYNC);
    int* mm_image_writer_ptr = (int*) mmap(NULL, _SC_PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fifo_ctrl_fd, 0x40003000);

    auto frame = [&](int tail) -> uint8_t* {
        return img_ptr + (tail * 0x2100);
    };

    auto frame_metadata = [&](int tail) -> metadata_frame* {
        return (metadata_frame*)((img_ptr + 0x2000 + tail * 0x2100));
    };

    auto process_metadata = [&] -> metadata_process* {
        return (metadata_process*) pmeta_ptr;
    };

    auto cleanup = [&](){
        munmap(mm_image_writer_ptr, _SC_PAGE_SIZE);
        close(fifo_ctrl_fd);
    };

    auto available = [](int head, int tail) -> bool {
        auto next_tail = 0;

        if (tail < (FIFO_LENGTH-1)) {
            next_tail = tail + 1;
        } else {
            next_tail = 0;
        }

        return (head < next_tail) || next_tail < head;

    };

    auto head = [&]() -> int {
        return *mm_image_writer_ptr;
    };

    auto fifo_tail = 0;
    auto fifo_head = 1;

    auto frame_idx_last = 0;

    while (!m_shutdown)
    {
        std::this_thread::yield();

        auto fifo_head = head();

        while (available(head(), fifo_tail)) {

            auto _frame_metadata = *frame_metadata(fifo_tail);

            if (_frame_metadata.frame_number - frame_idx_last > 1) {
                printf("missing frame %d -> %d (%d)\r\n", frame_idx_last, _frame_metadata.frame_number, _frame_metadata.frame_number - frame_idx_last);
            }

            frame_idx_last = _frame_metadata.frame_number;

            auto metadata = *process_metadata();
            if (write(socket_fd, &metadata, 60) < 0) {
                cleanup();
                return;
            }

            if (write(socket_fd, frame(fifo_tail), 8192) < 0) {
                cleanup();
                return;
            }

            if (fifo_tail < (FIFO_LENGTH-1)) {
                fifo_tail++;
            } else {
                fifo_tail=0;
            }

            std::this_thread::yield();
        }
    }
}
