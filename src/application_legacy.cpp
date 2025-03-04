#include <application.hpp>

#include <map>
#include <iostream>
#include <thread>
#include <csignal>
#include <atomic>
#include <cmath>

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

#include <components/timer.hpp>

#include <networking/profinet/device.hpp>

#include <drivers/arm_core.h>
#include <drivers/bpc_table_core.h>
#include <drivers/control_unit_core.h>

#include <drivers/common.h>
#include <drivers/arm_core.h>
#include <drivers/control_unit_core.h>
#include <drivers/roi_core_field_table.h>
#include <drivers/gen_core_field_table.h>
#include <drivers/pwm_core_field_table.h>
#include <drivers/mom_core_field_table.h>

#include <drivers/process_core_field_table.h>
#include <drivers/process_core.h>

#include <networking/tcp/protocol_legacy.hpp>

#include <math/algorithm.hpp>

int send_response(int fd, packet &req)
{
    std::cout << "response:" << std::endl;
    std::cout << req << std::endl;
    return ::write(fd, &req.data, sizeof(req.data));
}

#define DEFINE_COMMAND_TARGET_WRITE_CALLBACK(driver_type, prefix, var_type, var)                                                              \
    int command_target_##prefix##_##var##_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd) \
    {                                                                                                                                         \
        std::cout << __func__ << std::endl;                                                                                                   \
                                                                                                                                              \
        prefix##_##var##_set((driver_type##_state_t *)route.pdata, req.value.get());                                                          \
                                                                                                                                              \
        var_type value = req.value.get();                                                                                                     \
        prefix##_##var##_get((driver_type##_state_t *)route.pdata, &value);                                                                   \
        req.value.set(value);                                                                                                                 \
                                                                                                                                              \
        send_response(socket_fd, req);                                                                                                        \
                                                                                                                                              \
        return 0;                                                                                                                             \
    }

#define DEFINE_COMMAND_TARGET_READ_CALLBACK(driver_type, prefix, var_type, var)                                                              \
    int command_target_##prefix##_##var##_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd) \
    {                                                                                                                                        \
        std::cout << __func__ << std::endl;                                                                                                  \
                                                                                                                                             \
        var_type value = req.value.get();                                                                                                    \
        prefix##_##var##_get((driver_type##_state_t *)route.pdata, &value);                                                                  \
        req.value.set(value);                                                                                                                \
                                                                                                                                             \
        send_response(socket_fd, req);                                                                                                       \
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

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, save_embedded_conf)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, save_embedded_conf)
int command_target_nit_control_unit_core_arm_sw_version_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_control_unit_core_arm_sw_version_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    req.value.set(0x0002);
    send_response(socket_fd, req);
    return 0;
}

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, drift_enable)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, drift_enable)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, drift_position)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, drift_position)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, drift_level)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, drift_level)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, fpga_version)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core, nit_control_unit_core, uint16_t, fpga_version)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, start_track_mom_t)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, start_track_mom_t)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, end_of_track)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, end_of_track)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, mode)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, mode)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, reference_track)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, reference_track)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_mom_core, uint32_t, time_track_low)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_mom_core, uint32_t, time_track_low)
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

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core, uint32_t, auto_shutter)
// DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core, uint32_t, auto_shutter)

int command_target_nit_process_core_auto_shutter_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{

    // uint8_t *image = (uint8_t *)nit_framebuffer_core_get_memory_map(&nit_framebuffer_core_driver);
    // nit_bpc_table_core_bpc_table_write(&nit_bpc_table_core_driver, image, 8192);

    nit_control_unit_core_offset_en_set(&nit_control_unit_core_driver, 1);
    nit_control_unit_core_offset_update_set(&nit_control_unit_core_driver, 1);

    nit_control_unit_core_shutter_set(&nit_control_unit_core_driver, 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    nit_control_unit_core_shutter_set(&nit_control_unit_core_driver, 0);
    nit_control_unit_core_offset_update_set(&nit_control_unit_core_driver, 0);

    send_response(socket_fd, req);

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

int command_target_nit_gen_core_serial_number_low_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    req.value.set(0x00000);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_serial_number_low_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{

    req.route_id_set(0x04aa);
    req.value.set(0x3231);

    send_response(socket_fd, req);

    req.route_id_set(0x04ab);
    req.value.set(0x4541);

    send_response(socket_fd, req);

    req.route_id_set(0x04ac);
    req.value.set(0x3434);

    send_response(socket_fd, req);

    req.route_id_set(0x04ad);
    req.value.set(0x0037);

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

    {0x0501, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_int_time_read, .write = command_target_nit_control_unit_core_int_time_write}},
    {0x0502, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_bias_v_read, .write = command_target_nit_control_unit_core_bias_v_write}},
    {0x0503, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_offset_en_read, .write = command_target_nit_control_unit_core_offset_en_write}},
    {0x0504, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_offset_update_read, .write = command_target_nit_control_unit_core_offset_update_write}},
    {0x0505, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_shutter_read, .write = command_target_nit_control_unit_core_shutter_write}},
    {0x0506, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_bpc_en_read, .write = command_target_nit_control_unit_core_bpc_en_write}},
    {0x0507, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_bpc_mem_write_read, .write = command_target_nit_control_unit_core_bpc_mem_write_write}},
    {0x0508, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_bpc_identify_read, .write = command_target_nit_control_unit_core_bpc_identify_write}},
    {0x0509, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_temp1_read, .write = command_target_nit_control_unit_core_temp1_write}},
    {0x050A, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_temp2_read, .write = command_target_nit_control_unit_core_temp2_write}},
    {0x050B, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_temp3_read, .write = command_target_nit_control_unit_core_temp3_write}},
    {0x050C, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_temp4_read, .write = command_target_nit_control_unit_core_temp4_write}},
    {0x0520, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_trigger_usec_read, .write = command_target_nit_control_unit_core_trigger_usec_write}},
    {0x0521, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_black_level_read, .write = command_target_nit_control_unit_core_black_level_write}},
    {0x0522, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_sincronization_read, .write = command_target_nit_control_unit_core_sincronization_write}},
    {0x0523, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_save_embedded_conf_read, .write = command_target_nit_control_unit_core_save_embedded_conf_write}},
    {0x0524, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_arm_sw_version_read, .write = command_target_nit_control_unit_core_arm_sw_version_write}},
    {0x050D, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_drift_enable_read, .write = command_target_nit_control_unit_core_drift_enable_write}},
    {0x050E, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_drift_position_read, .write = command_target_nit_control_unit_core_drift_position_write}},
    {0x050F, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_drift_level_read, .write = command_target_nit_control_unit_core_drift_level_write}},
    {0x05A5, {.pdata = (void *)&nit_control_unit_core_driver, .read = command_target_nit_control_unit_core_fpga_version_read, .write = command_target_nit_control_unit_core_fpga_version_write}},

    {0x0402, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_kp_read, .write = command_target_nit_process_core_kp_write}},
    {0x0401, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_ki_read, .write = command_target_nit_process_core_ki_write}},
    {0x0403, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_kd_read, .write = command_target_nit_process_core_kd_write}},

    {0x0404, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_max_power_read, .write = command_target_nit_process_core_max_power_write}},
    {0x0405, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_min_power_read, .write = command_target_nit_process_core_min_power_write}},
    {0x0407, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_power_man_read, .write = command_target_nit_process_core_power_man_write}},
    {0x0409, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_auto_shutter_read, .write = command_target_nit_process_core_auto_shutter_write}},

    {0x040D, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_set_ref_width_read, .write = command_target_nit_process_core_set_ref_width_write}},
    {0x0418, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_power_limit_max_read, .write = command_target_nit_process_core_power_limit_max_write}},
    {0x0419, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_power_limit_min_read, .write = command_target_nit_process_core_power_limit_min_write}},
    {0x041F, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_width_ref_read, .write = command_target_nit_process_core_width_ref_write}},
    {0x0420, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_pixel_mm_ratio_read, .write = command_target_nit_process_core_pixel_mm_ratio_write}},
    {0x0421, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_pid_error_read, .write = command_target_nit_process_core_pid_error_write}},
    {0x0422, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_end_of_process_read, .write = command_target_nit_process_core_end_of_process_write}},
    {0x0423, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_limit_integral_read, .write = command_target_nit_process_core_limit_integral_write}},
    {0x0424, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_limit_slew_read, .write = command_target_nit_process_core_limit_slew_write}},
    {0x0425, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_buff_size_read, .write = command_target_nit_process_core_buff_size_write}},
    {0x04A5, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_buff_size_read, .write = command_target_nit_process_core_buff_size_write}},
    {0x0426, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_enable_alarm_read, .write = command_target_nit_process_core_enable_alarm_write}},
    {0x0427, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_alarm_max_read, .write = command_target_nit_process_core_alarm_max_write}},
    {0x0428, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_alarm_min_read, .write = command_target_nit_process_core_alarm_min_write}},
    {0x0429, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_alarm_time_read, .write = command_target_nit_process_core_alarm_time_write}},
    {0x042A, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_gen_core_serial_number_low_read, .write = command_target_nit_gen_core_serial_number_low_write}},
    {0x042E, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_automeasure_read, .write = command_target_nit_process_core_automeasure_write}},
    {0x042F, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_autoshutter_config_read, .write = command_target_nit_process_core_autoshutter_config_write}},
    {0x0431, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_autoshutter_temp_read, .write = command_target_nit_process_core_autoshutter_temp_write}},
    {0x0432, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_autoshutter_timer_read, .write = command_target_nit_process_core_autoshutter_timer_write}},
    {0x0433, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_track_ref_start_read, .write = command_target_nit_process_core_track_ref_start_write}},
    {0x0434, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_laser_external_control_read, .write = command_target_nit_process_core_laser_external_control_write}},
    {0x0435, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_delay_laser_on_read, .write = command_target_nit_process_core_delay_laser_on_write}},
    {0x0436, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_preheating_ena_read, .write = command_target_nit_process_core_preheating_ena_write}},
    {0x0437, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_preheating_time_read, .write = command_target_nit_process_core_preheating_time_write}},
    {0x0438, {.pdata = (void *)&nit_process_core_driver, .read = command_target_nit_process_core_preheating_power_read, .write = command_target_nit_process_core_preheating_power_write}},
    {0x0406, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_mom_core_start_track_mom_t_read, .write = command_target_nit_mom_core_start_track_mom_t_write}},
    {0x040a, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_mom_core_mode_read, .write = command_target_nit_mom_core_mode_write}},
    {0x0408, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_mom_core_end_of_track_read, .write = command_target_nit_mom_core_end_of_track_write}},
    // {0x048A, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_mom_core_mode_read, .write = command_target_nit_mom_core_mode_write}},
    {0x040B, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_mom_core_reference_track_read, .write = command_target_nit_mom_core_reference_track_write}},
    {0x040C, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_mom_core_time_track_low_read, .write = command_target_nit_mom_core_time_track_low_write}},
    {0x040E, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_arm_core_soft_reset_read, .write = command_target_nit_arm_core_soft_reset_write}},
    {0x040F, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_mom_core_threshold_read, .write = command_target_nit_mom_core_threshold_write}},
    // {0x0412, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_roi_core_x1_read, .write = command_target_nit_roi_core_x1_write}},
    {0x0410, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_roi_core_round_read, .write = command_target_nit_roi_core_round_write}},
    {0x0411, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_gen_core_enable_roi_read, .write = command_target_nit_gen_core_enable_roi_write}},
    {0x0412, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_roi_core_x1_read, .write = command_target_nit_roi_core_x1_write}},
    {0x0413, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_roi_core_y1_read, .write = command_target_nit_roi_core_y1_write}},
    {0x0414, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_roi_core_x2_read, .write = command_target_nit_roi_core_x2_write}},
    {0x0415, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_roi_core_y2_read, .write = command_target_nit_roi_core_y2_write}},
    {0x0416, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_mom_core_mode_read, .write = command_target_nit_mom_core_mode_write}},
    {0x041A, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_arm_core_led_r_read, .write = command_target_nit_arm_core_led_r_write}},
    {0x041B, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_arm_core_led_g_read, .write = command_target_nit_arm_core_led_g_write}},
    {0x041C, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_arm_core_led_b_read, .write = command_target_nit_arm_core_led_b_write}},
    {0x0417, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_pwm_core_pwm_read, .write = command_target_nit_pwm_core_pwm_write}},
    {0x041D, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_pwm_core_pwm_limit_max_read, .write = command_target_nit_pwm_core_pwm_limit_max_write}},
    {0x041E, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_pwm_core_pwm_limit_min_read, .write = command_target_nit_pwm_core_pwm_limit_min_write}},
    {0x0430, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_gen_core_digital_out_conf_read, .write = command_target_nit_gen_core_digital_out_conf_write}},
    {0x0439, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_gen_core_digital_out_0_read, .write = command_target_nit_gen_core_digital_out_0_write}},
    {0x043A, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_gen_core_digital_out_1_read, .write = command_target_nit_gen_core_digital_out_1_write}},
    {0x043B, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_gen_core_digital_out_2_read, .write = command_target_nit_gen_core_digital_out_2_write}},
    {0x043C, {.pdata = (void *)&nit_mb_core_driver, .read = command_target_nit_gen_core_digital_out_3_read, .write = command_target_nit_gen_core_digital_out_3_write}}};

void application::command_processor_legacy(int socket_fd)
{
    unsigned char data[1024];
    unsigned char *ptr = data;
    int length = 0;
    int client_retries = 0;

    std::mutex mutex;

    while (!m_shutdown & (client_retries++ < 2))
    {
        std::unique_lock<std::mutex> lk(mutex);
        length = read(socket_fd, ptr, sizeof(data));

        if (length <= 0)
        {
            continue;
        }

        client_retries = 0;

        // auto length = (size_t)(ptr - data);
        auto npackets = length / 4;

        // parse all in a list of requests
        for (int i = 0; i < npackets; i++)
        {

            auto packet_ptr_begin = data + 4 * i;
            auto packet_ptr_end = packet_ptr_begin + 4;
            auto packet_ptr_end_offset = packet_ptr_end - data;

            if (packet_ptr_end_offset > length)
            {
                std::cout << std::format("invalid packet found by offset of {}\n", packet_ptr_end_offset - length);
                break;
            }

            packet req(*(uint32_t *)packet_ptr_begin);

            std::cout << "request:" << std::endl;
            std::cout << req << std::endl;

            auto route_id = req.route_id_get();
            auto target = command_processor_routes_legacy.find(route_id);

            if (target == command_processor_routes_legacy.end())
            {
                printf("couldn't find route %04x\n", route_id);
                ::write(socket_fd, (uint8_t *)&req, sizeof(req));
                continue;
            }

            /**
             * if rout is found and request is of type write
             * call the route write function
             */
            if ((req.rw.get() == 0) & (command_processor_routes_legacy[route_id].write != nullptr))
            {
                auto &_route = command_processor_routes_legacy[route_id];
                _route.write(application::get_instance(), _route, req, socket_fd);

                /** TODO: this parameter save should be dissociated from network protocol */
                // auto _ = std::async(std::launch::async, &application::save_all, application::get_instance());
                save_all();
                continue;
            }

            /**
             * if route is found and request is of type read
             * call the read function
             */
            else if ((req.rw.get() == 1) & (command_processor_routes_legacy[route_id].read != nullptr))
            {
                auto &_route = command_processor_routes_legacy[route_id];
                _route.read(application::get_instance(), _route, req, socket_fd);
                continue;
            }

            /**
             * the competence of handling connection is of the route
             * the connetion will remain open to the next request.
             */
        }
    }

    close(socket_fd);
}


struct __attribute__((packed)) metadata
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

void application::image_writer_legacy(int socket_fd)
{

    int retval = 0;

    uint8_t *image_shm_ptr = (uint8_t *)nit_framebuffer_core_get_memory_map(&nit_framebuffer_core_driver);

    std::mutex socket_mutex;

    volatile int *virtual_metadata_shm_ptr = nit_process_core_get_virtual_metadata_shm_ptr(&nit_process_core_driver);

    metadata* ptr = (metadata*) virtual_metadata_shm_ptr;
    int last_frame_index = ptr->frame_number;

    int missing_frames_counter = 0;
    int first_frame = ptr->frame_number;

    while (!m_shutdown)
    {
        std::unique_lock<std::mutex> lk(socket_mutex);

        std::this_thread::sleep_for(std::chrono::microseconds(50));

        if ((last_frame_index - ptr->frame_number) == 0) {
            continue;
        }

        memcpy(m_image_buffer, image_shm_ptr, sizeof(m_image_buffer));
        memcpy(m_metadata_buffer, image_shm_ptr + sizeof(m_image_buffer), sizeof(m_metadata_buffer));

        {
            /** because of speed we ignore driver access assertions */
            auto voltage = unsafe_get<nit_control_unit_core_state_t, uint16_t>(&nit_control_unit_core_driver, nit_control_unit_core_temp1_offset);

            ((uint32_t *)virtual_metadata_shm_ptr)[13] = nit_control_unit_core_temp_to_degc(voltage);
        }

        {
            /** because of speed we ignore driver access assertions */
            auto voltage = unsafe_get<nit_control_unit_core_state_t, uint16_t>(&nit_control_unit_core_driver, nit_control_unit_core_temp2_offset);
            ((uint32_t *)virtual_metadata_shm_ptr)[14] = nit_control_unit_core_temp_to_degc(voltage);
        }

        if ((ptr->frame_number - last_frame_index) > 1)
        {
            printf("missed frame %d -> %d\n", last_frame_index, ptr->frame_number);
        }

        if ((retval = write(socket_fd, (void *)virtual_metadata_shm_ptr, 60)) < 0)
        {
            std::cout << "Failed to write at socket when writing metadata packet with error:" << strerror(retval) << std::endl;
            break;
        }

        if ((retval = write(socket_fd, m_image_buffer, sizeof(m_image_buffer))) < 0)
        {
            std::cout << "Failed to write at socket when writing frame packet with error:" << strerror(retval) << std::endl;
            break;
        }
        
        last_frame_index = ptr->frame_number;
    }

    printf("missed frames: %d\n", missing_frames_counter);
    printf("total frames: %d\n", ptr->frame_number - first_frame);
    
}