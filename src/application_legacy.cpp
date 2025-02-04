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

#include "application_legacy_control_loop.hpp"

int command_target_pid_controller_core_kp_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[KP]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_pid_controller_core_kp_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[KP] = req.value.get();
    req.value.set(ptr[KP]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_pid_controller_core_ki_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[KI]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_pid_controller_core_ki_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[KI] = req.value.get();
    req.value.set(ptr[KI]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_pid_controller_core_kd_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[KD]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_pid_controller_core_kd_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[KD] = req.value.get();
    req.value.set((uint32_t)ptr[KD]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_max_power_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[MAX_POWER]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_max_power_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[MAX_POWER] = req.value.get();
    req.value.set(ptr[MAX_POWER]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_min_power_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[MIN_POWER]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_min_power_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[MIN_POWER] = req.value.get();
    req.value.set(ptr[MIN_POWER]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_power_man_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[POWER_MAN]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_power_man_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[POWER_MAN] = req.value.get();
    req.value.set(ptr[POWER_MAN]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_auto_shutter_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[AUTO_SHUTTER]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_auto_shutter_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
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

int command_target_nit_gen_core_set_ref_width_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[SET_REF_WIDTH]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_set_ref_width_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[SET_REF_WIDTH] = req.value.get();
    req.value.set(ptr[SET_REF_WIDTH]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_power_limit_max_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[POWER_LIMIT_MAX]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_power_limit_max_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[POWER_LIMIT_MAX] = req.value.get();
    req.value.set(ptr[POWER_LIMIT_MAX]);
    nit_pwm_core_pwm_limit_max_set(&nit_mb_core_driver, ptr[POWER_LIMIT_MAX]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_power_limit_min_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[POWER_LIMIT_MIN]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_power_limit_min_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[POWER_LIMIT_MIN] = req.value.get();
    nit_pwm_core_pwm_limit_max_set(&nit_mb_core_driver, ptr[POWER_LIMIT_MIN]);
    req.value.set(ptr[POWER_LIMIT_MIN]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_width_ref_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[WIDTH_REF]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_width_ref_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[WIDTH_REF] = req.value.get();
    req.value.set(ptr[WIDTH_REF]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_pixel_mm_ratio_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[PIXEL_MM_RATIO]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_pixel_mm_ratio_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[PIXEL_MM_RATIO] = req.value.get();
    req.value.set(ptr[PIXEL_MM_RATIO]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_pid_error_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[PID_ERROR]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_pid_error_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[PID_ERROR] = req.value.get();
    req.value.set(ptr[PID_ERROR]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_end_of_process_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[END_OF_PROCESS]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_end_of_process_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[END_OF_PROCESS] = req.value.get();
    req.value.set(ptr[END_OF_PROCESS]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_limit_integral_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[LIMIT_INTEGRAL]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_limit_integral_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[LIMIT_INTEGRAL] = req.value.get();
    req.value.set(ptr[LIMIT_INTEGRAL]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_limit_slew_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[LIMIT_SLEW]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_limit_slew_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[LIMIT_SLEW] = req.value.get();
    req.value.set(ptr[LIMIT_SLEW]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_buff_size_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[BUFF_SIZE]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_buff_size_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[BUFF_SIZE] = req.value.get();
    req.value.set(ptr[BUFF_SIZE]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_enable_alarm_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[ENABLE_ALARM]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_enable_alarm_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[ENABLE_ALARM] = req.value.get();
    req.value.set(ptr[ENABLE_ALARM]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_alarm_max_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[ALARM_MAX]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_alarm_max_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[ALARM_MAX] = req.value.get();
    req.value.set(ptr[ALARM_MAX]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_alarm_min_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[ALARM_MIN]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_alarm_min_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[ALARM_MIN] = req.value.get();
    req.value.set(ptr[ALARM_MIN]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_alarm_time_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[ALARM_TIME]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_alarm_time_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[ALARM_TIME] = req.value.get();
    req.value.set(ptr[ALARM_TIME]);
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

int command_target_nit_gen_core_serial_number_low_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_automeasure_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[AUTOMEASURE]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_automeasure_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[AUTOMEASURE] = req.value.get();
    req.value.set(ptr[AUTOMEASURE]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_autoshutter_config_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[AUTOSHUTTER_CONFIG]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_autoshutter_config_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[AUTOSHUTTER_CONFIG] = req.value.get();
    req.value.set(ptr[AUTOSHUTTER_CONFIG]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_autoshutter_temperature_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[AUTOSHUTTER_TEMP]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_autoshutter_temperature_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[AUTOSHUTTER_TEMP] = req.value.get();
    req.value.set(ptr[AUTOSHUTTER_TEMP]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_autoshutter_timer_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[AUTOSHUTTER_TIMER]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_autoshutter_timer_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[AUTOSHUTTER_TIMER] = req.value.get();
    req.value.set(ptr[AUTOSHUTTER_TIMER]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_track_reference_start_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[TRACK_REF_START]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_track_reference_start_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[TRACK_REF_START] = req.value.get();
    req.value.set(ptr[TRACK_REF_START]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_laser_external_control_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[LASER_EXTERAL_CONTROL]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_laser_external_control_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[LASER_EXTERAL_CONTROL] = req.value.get();
    req.value.set(ptr[LASER_EXTERAL_CONTROL]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_delay_laser_on_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[DELAY_LASER_ON]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_delay_laser_on_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[DELAY_LASER_ON] = req.value.get();
    req.value.set(ptr[DELAY_LASER_ON]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_preheating_enable_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[PREHEATING_ENA]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_preheating_enable_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[PREHEATING_ENA] = req.value.get();
    req.value.set(ptr[PREHEATING_ENA]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_preheating_time_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[PREHEATING_TIME]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_preheating_time_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[PREHEATING_TIME] = req.value.get();
    req.value.set(ptr[PREHEATING_TIME]);
    send_response(socket_fd, req);
    return 0;
}

int command_target_nit_gen_core_preheating_power_read(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    req.value.set((uint32_t)ptr[PREHEATING_POWER]);
    std::cout << __func__ << std::endl;
    send_response(socket_fd, req);

    return 0;
}

int command_target_nit_gen_core_preheating_power_write(std::shared_ptr<application> app, command_processor_route &route, packet &req, int socket_fd)
{
    auto ptr = app->get_process_variables_shm_ptr();
    ptr[PREHEATING_POWER] = req.value.get();
    req.value.set(ptr[PREHEATING_POWER]);
    send_response(socket_fd, req);
    return 0;
}

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

    {0x0402, {.pdata = nullptr, .read = command_target_pid_controller_core_kp_read, .write = command_target_pid_controller_core_kp_write}},
    {0x0401, {.pdata = nullptr, .read = command_target_pid_controller_core_ki_read, .write = command_target_pid_controller_core_ki_write}},
    {0x0403, {.pdata = nullptr, .read = command_target_pid_controller_core_kd_read, .write = command_target_pid_controller_core_kd_write}},

    {0x0404, {.pdata = nullptr, .read = command_target_nit_gen_core_max_power_read, .write = command_target_nit_gen_core_max_power_write}},
    {0x0405, {.pdata = nullptr, .read = command_target_nit_gen_core_min_power_read, .write = command_target_nit_gen_core_min_power_write}},
    {0x0407, {.pdata = nullptr, .read = command_target_nit_gen_core_power_man_read, .write = command_target_nit_gen_core_power_man_write}},
    {0x0409, {.pdata = nullptr, .read = command_target_nit_gen_core_auto_shutter_read, .write = command_target_nit_gen_core_auto_shutter_write}},

    {0x040D, {.pdata = nullptr, .read = command_target_nit_gen_core_set_ref_width_read, .write = command_target_nit_gen_core_set_ref_width_write}},
    {0x0418, {.pdata = nullptr, .read = command_target_nit_gen_core_power_limit_max_read, .write = command_target_nit_gen_core_power_limit_max_write}},
    {0x0419, {.pdata = nullptr, .read = command_target_nit_gen_core_power_limit_min_read, .write = command_target_nit_gen_core_power_limit_min_write}},
    {0x041F, {.pdata = nullptr, .read = command_target_nit_gen_core_width_ref_read, .write = command_target_nit_gen_core_width_ref_write}},
    {0x0420, {.pdata = nullptr, .read = command_target_nit_gen_core_pixel_mm_ratio_read, .write = command_target_nit_gen_core_pixel_mm_ratio_write}},
    {0x0421, {.pdata = nullptr, .read = command_target_nit_gen_core_pid_error_read, .write = command_target_nit_gen_core_pid_error_write}},
    {0x0422, {.pdata = nullptr, .read = command_target_nit_gen_core_end_of_process_read, .write = command_target_nit_gen_core_end_of_process_write}},
    {0x0423, {.pdata = nullptr, .read = command_target_nit_gen_core_limit_integral_read, .write = command_target_nit_gen_core_limit_integral_write}},
    {0x0424, {.pdata = nullptr, .read = command_target_nit_gen_core_limit_slew_read, .write = command_target_nit_gen_core_limit_slew_write}},
    {0x0425, {.pdata = nullptr, .read = command_target_nit_gen_core_buff_size_read, .write = command_target_nit_gen_core_buff_size_write}},
    {0x04A5, {.pdata = nullptr, .read = command_target_nit_gen_core_buff_size_read, .write = command_target_nit_gen_core_buff_size_write}},
    {0x0426, {.pdata = nullptr, .read = command_target_nit_gen_core_enable_alarm_read, .write = command_target_nit_gen_core_enable_alarm_write}},
    {0x0427, {.pdata = nullptr, .read = command_target_nit_gen_core_alarm_max_read, .write = command_target_nit_gen_core_alarm_max_write}},
    {0x0428, {.pdata = nullptr, .read = command_target_nit_gen_core_alarm_min_read, .write = command_target_nit_gen_core_alarm_min_write}},
    {0x0429, {.pdata = nullptr, .read = command_target_nit_gen_core_alarm_time_read, .write = command_target_nit_gen_core_alarm_time_write}},
    {0x042A, {.pdata = nullptr, .read = command_target_nit_gen_core_serial_number_low_read, .write = command_target_nit_gen_core_serial_number_low_write}},
    {0x042E, {.pdata = nullptr, .read = command_target_nit_gen_core_automeasure_read, .write = command_target_nit_gen_core_automeasure_write}},
    {0x042F, {.pdata = nullptr, .read = command_target_nit_gen_core_autoshutter_config_read, .write = command_target_nit_gen_core_autoshutter_config_write}},
    {0x0431, {.pdata = nullptr, .read = command_target_nit_gen_core_autoshutter_temperature_read, .write = command_target_nit_gen_core_autoshutter_temperature_write}},
    {0x0432, {.pdata = nullptr, .read = command_target_nit_gen_core_autoshutter_timer_read, .write = command_target_nit_gen_core_autoshutter_timer_write}},
    {0x0433, {.pdata = nullptr, .read = command_target_nit_gen_core_track_reference_start_read, .write = command_target_nit_gen_core_track_reference_start_write}},
    {0x0434, {.pdata = nullptr, .read = command_target_nit_gen_core_laser_external_control_read, .write = command_target_nit_gen_core_laser_external_control_write}},
    {0x0435, {.pdata = nullptr, .read = command_target_nit_gen_core_delay_laser_on_read, .write = command_target_nit_gen_core_delay_laser_on_write}},
    {0x0436, {.pdata = nullptr, .read = command_target_nit_gen_core_preheating_enable_read, .write = command_target_nit_gen_core_preheating_enable_write}},
    {0x0437, {.pdata = nullptr, .read = command_target_nit_gen_core_preheating_time_read, .write = command_target_nit_gen_core_preheating_time_write}},
    {0x0438, {.pdata = nullptr, .read = command_target_nit_gen_core_preheating_power_read, .write = command_target_nit_gen_core_preheating_power_write}},
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

    while (!m_shutdown & (client_retries++ < 2))
    {

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
                auto _ = std::async(std::launch::async, &application::save_all, application::get_instance());
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

void application::image_writer_legacy(int socket_fd)
{

    int retval = 0;

    uint8_t *image_shm_ptr = (uint8_t *)nit_framebuffer_core_get_memory_map(&nit_framebuffer_core_driver);

    std::mutex socket_mutex;

    while (!m_shutdown)
    {

        m_timer->wait();

        std::unique_lock<std::mutex> lk(socket_mutex);

        memcpy(m_image_buffer, image_shm_ptr, sizeof(m_image_buffer));
        memcpy(m_metadata_buffer, image_shm_ptr + sizeof(m_image_buffer), sizeof(m_metadata_buffer));

        {
            /** because of speed we ignore driver access assertions */
            auto voltage = unsafe_get<nit_control_unit_core_state_t, uint16_t>(&nit_control_unit_core_driver, nit_control_unit_core_temp1_offset);
            ((uint32_t*)virtual_metadata_shm_ptr)[13] = nit_control_unit_core_temp_to_degc(voltage);
        }

        {
            /** because of speed we ignore driver access assertions */
            auto voltage = unsafe_get<nit_control_unit_core_state_t, uint16_t>(&nit_control_unit_core_driver, nit_control_unit_core_temp2_offset);
            ((uint32_t*)virtual_metadata_shm_ptr)[14] = nit_control_unit_core_temp_to_degc(voltage);
        }

        image_read.emit(application::get_instance(), m_image_buffer, sizeof(m_image_buffer), m_metadata_buffer, sizeof(m_metadata_buffer));

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
    }
}