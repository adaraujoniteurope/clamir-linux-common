#include "application.hpp"

#include <iostream>
#include <thread>
#include <csignal>
#include <atomic>

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

#include "components/timer.hpp"

#include "networking/profinet/device.hpp"

#include "drivers/arm_core.h"
#include "drivers/bpc_table_core.h"
#include "drivers/control_unit_core.h"

#include "drivers/common.h"
#include "drivers/arm_core.h"
#include "drivers/control_unit_core.h"
#include "drivers/roi_core_field_table.h"
#include "drivers/gen_core_field_table.h"
#include "drivers/pwm_core_field_table.h"
#include "drivers/mom_core_field_table.h"

#define DRIVER_CALLBACK_INDEX_TABLE_ITEM(name, parameter, type, size, offset) nit_##name##_##parameter##_command_descriptor_offset,

typedef enum command_descriptor_index_enum
{
    NIT_ARM_CORE_FIELD_TABLE(DRIVER_CALLBACK_INDEX_TABLE_ITEM)
    NIT_CONTROL_UNIT_CORE_FIELD_TABLE(DRIVER_CALLBACK_INDEX_TABLE_ITEM)
    NIT_GEN_CORE_FIELD_TABLE(DRIVER_CALLBACK_INDEX_TABLE_ITEM)
    NIT_PWM_CORE_FIELD_TABLE(DRIVER_CALLBACK_INDEX_TABLE_ITEM)
    NIT_ROI_CORE_FIELD_TABLE(DRIVER_CALLBACK_INDEX_TABLE_ITEM)
    NIT_MOM_CORE_FIELD_TABLE(DRIVER_CALLBACK_INDEX_TABLE_ITEM)
    COMMAND_DESCRIPTOR_INDEX_ENUM_MAX
} command_descriptor_index_t;

namespace std
{
    bool operator==(const std::thread &a, std::thread &b)
    {
        return a.get_id() == b.get_id();
    }
}

std::map<size_t, driver_interface_descriptor> application::m_command_descriptor_table =
{
    {
        nit_arm_core_led_b_command_descriptor_offset,
        {
            (void*)&nit_arm_core_driver,
            (driver_interface_descriptor::setter_type) nit_arm_core_led_b_set,
            (driver_interface_descriptor::getter_type) nit_arm_core_led_b_get
        }
    },


    {
        nit_gen_core_digital_in_0_command_descriptor_offset,
        {
            (void*)&nit_mb_core_driver,
            (driver_interface_descriptor::setter_type) nit_gen_core_digital_in_0_set,
            (driver_interface_descriptor::getter_type) nit_gen_core_digital_in_0_get
        }
    },

    {
        nit_gen_core_digital_in_1_command_descriptor_offset,
        {
            (void*)&nit_mb_core_driver,
            (driver_interface_descriptor::setter_type) nit_gen_core_digital_in_1_set,
            (driver_interface_descriptor::getter_type) nit_gen_core_digital_in_1_get
        }
    },

    
    {
        nit_pwm_core_pwm_limit_max_command_descriptor_offset,
        {
            (void*)& nit_mb_core_driver,
            (driver_interface_descriptor::setter_type)  nit_pwm_core_pwm_limit_max_set,
            (driver_interface_descriptor::getter_type) nit_pwm_core_pwm_limit_max_get
        }
    },
    
    {
        nit_pwm_core_pwm_limit_min_command_descriptor_offset,
        {
            (void*)& nit_mb_core_driver,
            (driver_interface_descriptor::setter_type) nit_pwm_core_pwm_limit_min_set,
            (driver_interface_descriptor::getter_type) nit_pwm_core_pwm_limit_min_get
        }
    },

    
    {
        nit_mom_core_start_track_mom_t_command_descriptor_offset,
        {
            (void*)& nit_mb_core_driver,
            (driver_interface_descriptor::setter_type) nit_mom_core_start_track_mom_t_set,
            (driver_interface_descriptor::getter_type) nit_mom_core_start_track_mom_t_get
        }
    },

    
    {
        nit_mom_core_end_of_track_command_descriptor_offset,
        {
            (void*)& nit_mb_core_driver,
            (driver_interface_descriptor::setter_type) nit_mom_core_end_of_track_set,
            (driver_interface_descriptor::getter_type) nit_mom_core_end_of_track_get
        }
    },

    
    {
        nit_mom_core_mode_command_descriptor_offset,
        {
            (void*)& nit_mb_core_driver,
            (driver_interface_descriptor::setter_type) nit_mom_core_mode_set,
            (driver_interface_descriptor::getter_type) nit_mom_core_mode_get
        }
    },

    
    {
        nit_mom_core_reference_track_command_descriptor_offset,
        {
            (void*)& nit_mb_core_driver,
            (driver_interface_descriptor::setter_type) nit_mom_core_reference_track_set,
            (driver_interface_descriptor::getter_type) nit_mom_core_reference_track_get
        }
    },

    
    {
        nit_mom_core_time_track_low_command_descriptor_offset,
        {
            (void*)& nit_mb_core_driver,
            (driver_interface_descriptor::setter_type) nit_mom_core_time_track_low_set,
            (driver_interface_descriptor::getter_type) nit_mom_core_time_track_low_get
        }
    },

    
    {
        nit_mom_core_time_track_high_command_descriptor_offset,
        {
            (void*)& nit_mb_core_driver,
            (driver_interface_descriptor::setter_type) nit_mom_core_time_track_high_set,
            (driver_interface_descriptor::getter_type) nit_mom_core_time_track_high_get
        }
    },

    
    {
        nit_mom_core_threshold_command_descriptor_offset,
        {
            (void*)& nit_mb_core_driver,
            (driver_interface_descriptor::setter_type) nit_mom_core_threshold_set,
            (driver_interface_descriptor::getter_type) nit_mom_core_threshold_get
        }
    },

    
    {
        nit_roi_core_round_command_descriptor_offset,
        {
            (void*)& nit_mb_core_driver,
            (driver_interface_descriptor::setter_type) nit_roi_core_round_set,
            (driver_interface_descriptor::getter_type) nit_roi_core_round_get
        }
    },

    
    {
        nit_gen_core_enable_roi_command_descriptor_offset,
        {
            (void*)& nit_mb_core_driver,
            (driver_interface_descriptor::setter_type) nit_gen_core_enable_roi_set,
            (driver_interface_descriptor::getter_type) nit_gen_core_enable_roi_get
        }
    },

    
    {
        nit_roi_core_x1_command_descriptor_offset,
        {(void*)& nit_mb_core_driver,
        (driver_interface_descriptor::setter_type) nit_roi_core_x1_set,
        (driver_interface_descriptor::getter_type)  nit_roi_core_x1_get}
    },
    
    {
        nit_roi_core_x2_command_descriptor_offset,
        {
            (void*)& nit_mb_core_driver,
            (driver_interface_descriptor::setter_type) nit_roi_core_x2_set,
            (driver_interface_descriptor::getter_type) nit_roi_core_x2_get
        }
    },

    
    {
        nit_roi_core_y1_command_descriptor_offset,
        {
            (void*)& nit_mb_core_driver,
            (driver_interface_descriptor::setter_type) nit_roi_core_y1_set,
            (driver_interface_descriptor::getter_type) nit_roi_core_y1_get
        }
    },

    
    {
        nit_roi_core_y2_command_descriptor_offset,
        {
            (void*)& nit_mb_core_driver,
            (driver_interface_descriptor::setter_type) nit_roi_core_y2_set,
            (driver_interface_descriptor::getter_type) nit_roi_core_y2_get
        }
    },

    
    {
        nit_gen_core_digital_out_conf_command_descriptor_offset,
        {
            (void*)& nit_mb_core_driver,
            (driver_interface_descriptor::setter_type) nit_gen_core_digital_out_conf_set,
            (driver_interface_descriptor::getter_type) nit_gen_core_digital_out_conf_get
        }
    },

    
    {
        nit_control_unit_core_black_level_command_descriptor_offset,
        {
            (void*)& nit_control_unit_core_driver,
            (driver_interface_descriptor::setter_type) nit_control_unit_core_black_level_set,
            (driver_interface_descriptor::getter_type) nit_control_unit_core_black_level_get
        }
    },

    
    {
        nit_control_unit_core_bias_v_command_descriptor_offset,
        {
            (void*)& nit_control_unit_core_driver,
            (driver_interface_descriptor::setter_type) nit_control_unit_core_bias_v_set,
            (driver_interface_descriptor::getter_type) nit_control_unit_core_bias_v_get
        }
    },

    
    {
        nit_control_unit_core_int_time_command_descriptor_offset,
        {
            (void*)& nit_control_unit_core_driver,
            (driver_interface_descriptor::setter_type) nit_control_unit_core_int_time_set,
            (driver_interface_descriptor::getter_type) nit_control_unit_core_int_time_get
        }
    },

    
    {
        nit_control_unit_core_drift_enable_command_descriptor_offset,
        {
            (void*)& nit_control_unit_core_driver,
            (driver_interface_descriptor::setter_type) nit_control_unit_core_drift_enable_set,
            (driver_interface_descriptor::getter_type) nit_control_unit_core_drift_enable_get
        }
    },

    
    {
        nit_control_unit_core_drift_position_command_descriptor_offset,
        {
            (void*)& nit_control_unit_core_driver,
            (driver_interface_descriptor::setter_type) nit_control_unit_core_drift_position_set,
            (driver_interface_descriptor::getter_type) nit_control_unit_core_drift_position_get
        }
    },

    
    {
        nit_gen_core_change_op_mode_command_descriptor_offset,
        {
            (void*)& nit_mb_core_driver,
            (driver_interface_descriptor::setter_type) nit_gen_core_change_op_mode_set,
            (driver_interface_descriptor::getter_type) nit_gen_core_change_op_mode_get
        }
    },

    
    {
        nit_control_unit_core_bpc_en_command_descriptor_offset,
        {
            (void*)& nit_control_unit_core_driver,
            (driver_interface_descriptor::setter_type) nit_control_unit_core_bpc_en_set,
            (driver_interface_descriptor::getter_type) nit_control_unit_core_bpc_en_get
        }
    },

    {
        nit_control_unit_core_offset_update_command_descriptor_offset,
        {
             (void*)& nit_control_unit_core_driver,
             (driver_interface_descriptor::setter_type) nit_control_unit_core_offset_update_set,
             (driver_interface_descriptor::getter_type) nit_control_unit_core_offset_update_get
            
        }
    }
};

int application::initialize(int argc, char *argv[])
{

    {
        int retval = nit_arm_core_open(&arm_core_state);

        if (retval < 0)
        {
            return -1;
        }
    }

    {
        int retval = bpc_table_core_open(&bpc_core_state);
        if (retval < 0)
        {
            return -1;
        }
    }

    {
        int retval = nit_control_unit_core_open(&control_unit_core_state);
        if (retval < 0)
        {
            return -1;
        }
    }

    {
        int retval = framebuffer_core_open(&framebuffer_core_state);
        if (retval < 0)
        {
            return -1;
        }
    }

    {
        int retval = framebuffer_metadata_core_open(&framebuffer_metadata_core_state);
        if (retval < 0)
        {
            return -1;
        }
    }

    {
        int retval = mb_core_open(&mb_core_state);
        if (retval < 0)
        {
            return -1;
        }
    }

    /**
     * Configuration Defaults from Previous Releases
     */

    // nit_mb_core_pwm_limit_max_set: NIT_MB_CORE_PWM_LIMIT_MAX_OFFSET                 (0x0000C000): 1117
    nit_pwm_core_pwm_limit_max_set(&mb_core_state, 1117);

    // nit_mb_core_pwm_limit_min_set: NIT_MB_CORE_PWM_LIMIT_MIN_OFFSET                 (0x0000C003): 372
    nit_pwm_core_pwm_limit_min_set(&mb_core_state, 372);

    // nit_mb_core_start_track_mom_t_set: NIT_MB_CORE_START_TRACK_MOM_T_OFFSET         (0x00004009): 40
    nit_mom_core_start_track_mom_t_set(&mb_core_state, 40);

    // nit_mb_core_end_of_track_set: NIT_MB_CORE_END_OF_TRACK_OFFSET                   (0x00004010): 30
    nit_mom_core_end_of_track_set(&mb_core_state, 30);

    // nit_mb_core_mode_set: NIT_MB_CORE_MODE_OFFSET                                   (0x0000400B): 2
    nit_mom_core_mode_set(&mb_core_state, 2);

    // nit_mb_core_reference_track_set: NIT_MB_CORE_REFERENCE_TRACK_OFFSET             (0x0000400F): 3
    nit_mom_core_reference_track_set(&mb_core_state, 3);

    // nit_mb_core_time_track_low_set: NIT_MB_CORE_TIME_TRACK_LOW_OFFSET               (0x0000400C): 200000000
    nit_mom_core_time_track_low_set(&mb_core_state, 200000000);

    // nit_mb_core_time_track_high_set: NIT_MB_CORE_TIME_TRACK_HIGH_OFFSET             (0x0000400D): 0
    nit_mom_core_time_track_high_set(&mb_core_state, 0);

    // nit_mb_core_threshold_set: NIT_MB_CORE_THRESHOLD_OFFSET                         (0x00004000): 1182
    nit_mom_core_threshold_set(&mb_core_state, 1182);

    // nit_mb_core_roi_round_set: NIT_MB_CORE_ROI_ROUND_OFFSET                         (0x00008004): 0
    nit_roi_core_round_set(&mb_core_state, 0);

    // nit_mb_core_enable_roi_set: NIT_MB_CORE_ENABLE_ROI_OFFSET                       (0x00000002): 1
    nit_gen_core_enable_roi_set(&mb_core_state, 1);

    // nit_mb_core_roi_x1_set: NIT_MB_CORE_ROI_X1_OFFSET                               (0x00008000): 9
    nit_roi_core_x1_set(&mb_core_state, 9);

    // nit_mb_core_roi_x2_set: NIT_MB_CORE_ROI_X2_OFFSET                               (0x00008002): 53
    nit_roi_core_x2_set(&mb_core_state, 53);

    // nit_mb_core_roi_y1_set: NIT_MB_CORE_ROI_Y1_OFFSET                               (0x00008001): 8
    nit_roi_core_y1_set(&mb_core_state, 8);

    // nit_mb_core_roi_y2_set: NIT_MB_CORE_ROI_Y2_OFFSET                               (0x00008003): 52
    nit_roi_core_y2_set(&mb_core_state, 52);

    // nit_mb_core_digital_out_conf_set: NIT_MB_CORE_DIGITAL_OUT_CONF_OFFSET           (0x0000000B): 0
    nit_gen_core_digital_out_conf_set(&mb_core_state, 0);

    // control_unit_black_level_set: NIT_CONTROL_UNIT_BLACK_LEVEL_OFFSET               (0x00000024): 1000
    nit_control_unit_core_black_level_set(&control_unit_core_state, 1000);

    // control_unit_bias_v_set: NIT_CONTROL_UNIT_BIAS_V_OFFSET                         (0x00000001): 13652
    nit_control_unit_core_bias_v_set(&control_unit_core_state, 13652);

    // control_unit_int_time_set: NIT_CONTROL_UNIT_INT_TIME_OFFSET                     (0x00000000): 600
    nit_control_unit_core_int_time_set(&control_unit_core_state, 600);

    // control_unit_drift_enable_set: NIT_CONTROL_UNIT_DRIFT_ENABLE_OFFSET             (0x00000010): 1
    nit_control_unit_core_drift_enable_set(&control_unit_core_state, 1);

    // control_unit_drift_position_set: NIT_CONTROL_UNIT_DRIFT_POSITION_OFFSET         (0x00000011): 14
    nit_control_unit_core_drift_position_set(&control_unit_core_state, 14);

    // nit_mb_core_change_op_mode_set: NIT_MB_CORE_CHANGE_OP_MODE_OFFSET               (0x00000005): 1
    nit_gen_core_change_op_mode_set(&mb_core_state, 1);

    // control_unit_bpc_en_set: NIT_CONTROL_UNIT_BPC_EN_OFFSET                         (0x00000005): 1
    nit_control_unit_core_bpc_en_set(&control_unit_core_state, 1);

    nit_control_unit_core_offset_update_set(&control_unit_core_state, 1);

    return 0;
}

application::application()
    : m_shutdown(false)
      , m_command_server_router({{1, std::bind(&application::default_handler, this, std::placeholders::_1, std::placeholders::_2)}})
      , arm_core_state(nit_arm_core_driver)
      , mb_core_state(nit_mb_core_driver)
      , control_unit_core_state(nit_control_unit_core_driver)
      , bpc_core_state(nit_bpc_table_core_driver)
      , framebuffer_core_state(nit_framebuffer_core_driver)
      , framebuffer_metadata_core_state(nit_framebuffer_metadata_core_driver)
      , m_timer(1000, m_shutdown)
{

}

int application::default_handler(const unsigned char *buffer, int)
{
    return 0;
}

std::shared_ptr<application> application::create()
{
    return std::shared_ptr<application>(new application());
}

void application::command_router(int socket_fd)
{
}

void application::image_writer(int socket_fd)
{

    const uint8_t sync[] = {0x01, 0x00, 0x02, 0x00, 0x03, 0x00};
    const uint8_t metadata[60] = {0x00};
    uint8_t buffer[8192] = {0x00};

    uint8_t *image = (uint8_t *)framebuffer_core_get_memory_map(&framebuffer_core_state);
    int image_size = 8192;

    std::mutex socket_mutex;

    while (!m_shutdown)
    {

        m_timer.wait();

        std::unique_lock<std::mutex> lk(socket_mutex);

        if (write(socket_fd, sync, sizeof(sync)) < 0)
        {
            perror("Failed to write at socket when writing sync payload");
            break;
        }
        if (write(socket_fd, metadata, sizeof(metadata)) < 0)
        {
            perror("Failed to write at socket when writing metadata payload");
            break;
        }

        if (write(socket_fd, image, image_size) < 0)
        {
            perror("Failed to write at socket when writing frame payload");
            break;
        }
    }
}

void application::command_processor(int socket_fd)
{
    unsigned char data[1024];
    while (!m_shutdown)
    {

        int bytes_read = read(socket_fd, data, sizeof(data));

        if (bytes_read <= 0)
        {
            break;
        }
        
        std::stringstream ss((const char*) data);

        uint32_t index;

        ss >> index;

        try {

            std::stringstream ss;

            auto& descriptor = application::m_command_descriptor_table[index];

            if (descriptor.state != nullptr && descriptor.get != nullptr && descriptor.set != nullptr)
            {
                auto value = descriptor.get(descriptor.state);
                ss << "{ index: " << index << ", value: " << value << "}" << std::endl;
            } else {
                ss << "{ error: \"command not found\" }" << std::endl;
            }

            auto result = ss.str();

            if (write(socket_fd, result.c_str(), strlen(result.c_str())) < 0)
            {
                break;
            }


            
            
            


        } catch (std::exception& ex)
        {
            std::cout << "Requested command not found" << index << std::endl;
        }


    }
}

void application::run()
{
    std::vector<std::thread> threads;
    static std::atomic_bool shutdown = false;

    // Set up signal handling using standard C++ facilities
    std::signal(SIGINT, [](int)
                { shutdown.store(true); });
    std::signal(SIGTERM, [](int)
                { shutdown.store(true); });
    std::signal(SIGPIPE, SIG_IGN); // Ignore SIGPIPE

    /**
     * Creates the command server worker therad
     */
    auto command_server_worker = std::thread(tcp_server::create(4096, std::bind(&application::command_processor, this, std::placeholders::_1), shutdown));

    /**
     * Creates the image server worker therad
     */
    auto image_server_worker = std::thread(tcp_server::create(4097, std::bind(&application::image_writer, this, std::placeholders::_1), shutdown));

    /**
     * Create the shutter controller worker therad
     */

    // auto shutter_controller_worker = std::thread(std::bind(&timer::run, &timer::get_instance(), 1000, shutdown));
    auto timer_worker = std::thread(std::bind(&timer::run, &m_timer));

    auto profinet_service_worker = std::thread(device::create("end0", shutdown));

    m_server_threads.push_back(std::move(timer_worker));
    m_server_threads.push_back(std::move(command_server_worker));
    m_server_threads.push_back(std::move(image_server_worker));

    while (!shutdown.load())
    {
        /**
         * Corrected Bug:
         * When a signal like SIGTERM or SIGING it thrown during sleep, this function throws and kills
         * the main thread leading to a undefined state.
         *
         * When wrapping in a thread, the thread dies causing a return that's why it was working before.
         * but not here.
         *
         * Catching the exception, avoids this faulty behavior.
         */
        try
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        catch (std::exception &)
        {
        }
    }

    for (auto &thread : m_server_threads)
    {
        if (thread.joinable())
        {
            m_shutdown = true;
            thread.join();
        }
    }

    std::cout << "shutting down system" << std::endl;
}
void application::print_usage()
{
    const char usage[] =
        "clamir tcp server"
        "-h/--help: help\n"
        "--host: host\n"
        "--port: port\n";
}