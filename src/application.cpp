#include <nit/embedded/application.hpp>

#include <iostream>
#include <thread>
#include <csignal>
#include <atomic>

#include <sys/mman.h>

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

#include <nit/embedded/utils/runnable_worker.hpp>
#include <nit/embedded/components/timer.hpp>
#include <nit/embedded/components/filter.hpp>
#include <nit/embedded/math/control.hpp>
#include <nit/embedded/utils/time.hpp>

#include <nit/embedded/components/timer.hpp>

#include <nit/embedded/drivers/arm_core.h>
#include <nit/embedded/drivers/bpc_table_core.h>
#include <nit/embedded/drivers/scc_core.h>
#include <nit/embedded/drivers/control_unit_core.h>

#include <nit/embedded/drivers/common.h>
#include <nit/embedded/drivers/arm_core.h>
#include <nit/embedded/drivers/control_unit_core.h>
#include <nit/embedded/drivers/process_core.h>

#include <nit/embedded/drivers/roi_core_field_table.h>
#include <nit/embedded/drivers/gen_core_field_table.h>
#include <nit/embedded/drivers/pwm_core_field_table.h>
#include <nit/embedded/drivers/mom_core_field_table.h>

#include <nit/embedded/drivers/framebuffer_core.h>

#include <nit/embedded/math/algorithm.hpp>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

using namespace math::control;
using namespace utils::time;
using namespace utils::numeric;

// namespace po = boost::program_options;

namespace std
{
    bool operator==(const std::thread& a, std::thread& b)
    {
        return a.get_id() == b.get_id();
    }
}

int application::sensor_calibrate()
{
    nit_control_unit_core_offset_en_set(&nit_control_unit_core_driver, 1);
    nit_control_unit_core_offset_update_set(&nit_control_unit_core_driver, 1);

    nit_scc_core_calibration_bypass_set(&nit_scc_core_driver, 1);
    nit_scc_core_opmode_set(&nit_scc_core_driver, NIT_SCC_CORE_OPMODE_CALIBRATE);

    /**
     * in the case we want to use the FPGA to do the procedure instead of software
     * 1. Design Rationale: it's not necessary to use the FPGA for calibrating the
     *      sensor and improves complexity of the design.
     */
    nit_scc_core_calibration_mode_set(&nit_scc_core_driver, NIT_SCC_CORE_CALIBRATION_STATUS_ACQUIRING_MIN);

    nit_control_unit_core_shutter_set(&nit_control_unit_core_driver, 1);
    if (host_mockup) {
        nit_framebuffer_core_operating_mode_set(&nit_framebuffer_core_driver, NIT_FRAMEBUFFER_CORE_OPERATING_MODE_TEST_UNIFORM_SHUTTER_CLOSED);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    nit_scc_core_stub_eval_calibrate_acquire_min(&nit_scc_core_driver);
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));

    /**
     * in the case we want to use the FPGA to do the procedure instead of software
     * 1. Design Rationale: it's not necessary to use the FPGA for calibrating the
     *      sensor and improves complexity of the design.
     */
    nit_scc_core_calibration_mode_set(&nit_scc_core_driver, NIT_SCC_CORE_CALIBRATION_STATUS_ACQUIRING_MAX);

    if (host_mockup) {
        nit_framebuffer_core_operating_mode_set(&nit_framebuffer_core_driver, NIT_FRAMEBUFFER_CORE_OPERATING_MODE_TEST_UNIFORM_SHUTTER_OPEN);
    }
    nit_control_unit_core_shutter_set(&nit_control_unit_core_driver, 0);

    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    nit_scc_core_stub_eval_calibrate_acquire_max(&nit_scc_core_driver);
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));

    // nit_scc_core_stub_eval_calibrate_update(&nit_scc_core_driver);

    // in the case we use FPGA for calibrating the sensor...
    // nit_scc_core_calibration_mode_wait_idle(&nit_scc_core_driver);

    nit_scc_core_opmode_set(&nit_scc_core_driver, NIT_SCC_CORE_OPMODE_PROCESS);

    if (host_mockup) {
        nit_framebuffer_core_operating_mode_set(&nit_framebuffer_core_driver, NIT_FRAMEBUFFER_CORE_OPERATING_MODE_TEST_PATTERN_BALL);
    }

    if (!nit_scc_core_driver.config.calibration_bypass)
    {
        nit_scc_core_calibration_bypass_set(&nit_scc_core_driver, 0);
    }

    /**
     * older offset core processing disable offset update
     */
    nit_control_unit_core_offset_update_set(&nit_control_unit_core_driver, 0);

    return 0;
}

int application::initialize(int argc, char* argv[])
{

    const char* CONFIGURATION_DIRECTORY = std::getenv("CONFIGURATION_DIRECTORY");

    if (CONFIGURATION_DIRECTORY != nullptr) {
        std::cout << "CONFIGURATION_DIRECTORY: " << CONFIGURATION_DIRECTORY << std::endl;
    }
    else {
        std::cout << "CONFIGURATION_DIRECTORY environment variable is not set." << std::endl;
        CONFIGURATION_DIRECTORY = std::filesystem::current_path().c_str();
    }

    std::filesystem::path configuration_path = CONFIGURATION_DIRECTORY;

    try {
        std::filesystem::create_directory(configuration_path);
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }

    auto app_config_path = std::string(CONFIGURATION_DIRECTORY) + "/clamir.xml";

    if (!std::filesystem::exists(app_config_path))
    {
        try {
            std::ofstream ofs(app_config_path);
            boost::archive::xml_oarchive xoa(ofs);
            xoa& boost::make_nvp("config", this->config_default);
        }
        catch (std::exception& ex) {
            std::cout << ex.what() << std::endl;
            return -1;
        }
    }

    try {
        std::ifstream ifs(app_config_path);
        boost::archive::xml_iarchive xia(ifs);
        xia& boost::make_nvp("config", this->config);
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
        return -1;
    }

    {
        int retval = nit_arm_core_open(&nit_arm_core_driver);

        if (retval < 0)
        {
            return -1;
        }
    }

    {
        int retval = nit_bpc_table_core_open(&nit_bpc_table_core_driver);
        if (retval < 0)
        {
            return -1;
        }
    }

    {
        int retval = nit_control_unit_core_open(&nit_control_unit_core_driver);
        if (retval < 0)
        {
            return -1;
        }
    }

    {

        nit_framebuffer_core_config_t config = {
            .operating_mode = NIT_FRAMEBUFFER_CORE_OPERATING_MODE_NORMAL
        };

        if (host_mockup) {
            config.operating_mode = NIT_FRAMEBUFFER_CORE_OPERATING_MODE_TEST_PATTERN_BALL;
        }

        int retval = nit_framebuffer_core_open(&nit_framebuffer_core_driver, &config);

        if (retval < 0)
        {
            return -1;
        }
    }

    {
        int retval = nit_mb_core_open(&nit_mb_core_driver);
        if (retval < 0)
        {
            return -1;
        }
    }

    {
        auto save_path = configuration_path;
        save_path += "/nit_scc_core_config.xml";

        std::cout << "loading configuration from: " << save_path << std::endl;

        if (nit_scc_core_config_load_from_file(&nit_scc_core_driver, save_path.c_str()) < 0)
        {
            if (nit_scc_core_config_save_to_file(&nit_scc_core_driver, save_path.c_str()) < 0)
            {
                std::cout << "loading configuration failed!" << std::endl;
                return -1;
            }
        }

        if (nit_scc_core_config_load_from_file(&nit_scc_core_driver, save_path.c_str()) < 0)
        {
            std::cout << "loading configuration failed!" << std::endl;
            return -1;
        }
    }

    {
        int retval = nit_scc_core_open(&nit_scc_core_driver, &nit_framebuffer_core_driver);
        if (retval < 0)
        {
            return -1;
        }
    }

    /**
     * Configuration Defaults from Previous Releases
     */

     // nit_mb_core_pwm_limit_max_set: NIT_MB_CORE_PWM_LIMIT_MAX_OFFSET                 (0x0000C000): 1117
    nit_pwm_core_pwm_limit_max_set(&nit_mb_core_driver, 1117);

    // nit_mb_core_pwm_limit_min_set: NIT_MB_CORE_PWM_LIMIT_MIN_OFFSET                 (0x0000C003): 372
    nit_pwm_core_pwm_limit_min_set(&nit_mb_core_driver, 372);

    // nit_mb_core_start_track_mom_t_set: NIT_MB_CORE_START_TRACK_MOM_T_OFFSET         (0x00004009): 40
    nit_mom_core_start_track_mom_t_set(&nit_mb_core_driver, 40);

    // nit_mb_core_end_of_track_set: NIT_MB_CORE_END_OF_TRACK_OFFSET                   (0x00004010): 30
    nit_mom_core_end_of_track_set(&nit_mb_core_driver, 30);

    // nit_mb_core_mode_set: NIT_MB_CORE_MODE_OFFSET                                   (0x0000400B): 2
    nit_mom_core_mode_set(&nit_mb_core_driver, 2);

    // nit_mb_core_reference_track_set: NIT_MB_CORE_REFERENCE_TRACK_OFFSET             (0x0000400F): 3
    nit_mom_core_reference_track_set(&nit_mb_core_driver, 3);

    // nit_mb_core_time_track_low_set: NIT_MB_CORE_TIME_TRACK_LOW_OFFSET               (0x0000400C): 200000000
    nit_mom_core_time_track_low_set(&nit_mb_core_driver, 200000000);

    // nit_mb_core_time_track_high_set: NIT_MB_CORE_TIME_TRACK_HIGH_OFFSET             (0x0000400D): 0
    nit_mom_core_time_track_high_set(&nit_mb_core_driver, 0);

    // nit_mb_core_threshold_set: NIT_MB_CORE_THRESHOLD_OFFSET                         (0x00004000): 1182
    nit_mom_core_threshold_set(&nit_mb_core_driver, 1182);

    // nit_mb_core_roi_round_set: NIT_MB_CORE_ROI_ROUND_OFFSET                         (0x00008004): 0
    nit_roi_core_round_set(&nit_mb_core_driver, 0);

    // nit_mb_core_enable_roi_set: NIT_MB_CORE_ENABLE_ROI_OFFSET                       (0x00000002): 1
    nit_gen_core_enable_roi_set(&nit_mb_core_driver, 0);

    // nit_mb_core_roi_x1_set: NIT_MB_CORE_ROI_X1_OFFSET                               (0x00008000): 9
    nit_roi_core_x1_set(&nit_mb_core_driver, 15);

    // nit_mb_core_roi_x2_set: NIT_MB_CORE_ROI_X2_OFFSET                               (0x00008002): 53
    nit_roi_core_x2_set(&nit_mb_core_driver, 50);

    // nit_mb_core_roi_y1_set: NIT_MB_CORE_ROI_Y1_OFFSET                               (0x00008001): 8
    nit_roi_core_y1_set(&nit_mb_core_driver, 15);

    // nit_mb_core_roi_y2_set: NIT_MB_CORE_ROI_Y2_OFFSET                               (0x00008003): 52
    nit_roi_core_y2_set(&nit_mb_core_driver, 50);

    // nit_mb_core_digital_out_conf_set: NIT_MB_CORE_DIGITAL_OUT_CONF_OFFSET           (0x0000000B): 0
    nit_gen_core_digital_out_conf_set(&nit_mb_core_driver, 0);

    // control_unit_black_level_set: NIT_CONTROL_UNIT_BLACK_LEVEL_OFFSET               (0x00000024): 1000
    nit_control_unit_core_black_level_set(&nit_control_unit_core_driver, 1000);

    // control_unit_bias_v_set: NIT_CONTROL_UNIT_BIAS_V_OFFSET                         (0x00000001): 13652
    nit_control_unit_core_bias_v_set(&nit_control_unit_core_driver, 13652);

    // control_unit_int_time_set: NIT_CONTROL_UNIT_INT_TIME_OFFSET                     (0x00000000): 600
    nit_control_unit_core_int_time_set(&nit_control_unit_core_driver, 600);

    // control_unit_drift_enable_set: NIT_CONTROL_UNIT_DRIFT_ENABLE_OFFSET             (0x00000010): 1
    nit_control_unit_core_drift_enable_set(&nit_control_unit_core_driver, 1);

    // control_unit_drift_position_set: NIT_CONTROL_UNIT_DRIFT_POSITION_OFFSET         (0x00000011): 14
    nit_control_unit_core_drift_position_set(&nit_control_unit_core_driver, 14);

    // nit_mb_core_change_op_mode_set: NIT_MB_CORE_CHANGE_OP_MODE_OFFSET               (0x00000005): 1
    nit_gen_core_change_op_mode_set(&nit_mb_core_driver, 1);

    // control_unit_bpc_en_set: NIT_CONTROL_UNIT_BPC_EN_OFFSET                         (0x00000005): 1
    nit_control_unit_core_bpc_en_set(&nit_control_unit_core_driver, 1);

    nit_control_unit_core_offset_update_set(&nit_control_unit_core_driver, 1);

    {
        auto save_path = configuration_path;
        save_path += "/nit_arm_core_config.xml";

        std::cout << "loading configuration from: " << save_path << std::endl;

        if (nit_arm_core_config_load_from_file(&nit_arm_core_driver, save_path.c_str()) < 0)
        {
            if (nit_arm_core_config_save_to_file(&nit_arm_core_driver, save_path.c_str()) < 0)
            {
                std::cout << "loading configuration failed!" << std::endl;
                return -1;
            }
        }

    }

    {
        auto save_path = configuration_path;
        save_path += "/nit_control_unit_core_config.xml";

        std::cout << "loading configuration from: " << save_path << std::endl;

        if (nit_control_unit_core_config_load_from_file(&nit_control_unit_core_driver, save_path.c_str()) < 0)
        {
            if (nit_control_unit_core_config_save_to_file(&nit_control_unit_core_driver, save_path.c_str()) < 0)
            {
                std::cout << "loading configuration failed!" << std::endl;
                return -1;
            }
        }

    }

    {
        auto save_path = configuration_path;
        save_path += "/nit_mb_core_config.xml";

        std::cout << "loading configuration from: " << save_path << std::endl;

        if (nit_mb_core_config_load_from_file(&nit_mb_core_driver, save_path.c_str()) < 0)
        {
            if (nit_mb_core_config_save_to_file(&nit_mb_core_driver, save_path.c_str()) < 0)
            {
                std::cout << "loading configuration failed!" << std::endl;
                return -1;
            }
        }

    }

    {
        int retval = nit_process_core_open(&nit_process_core_driver, &nit_arm_core_driver, &nit_control_unit_core_driver, &nit_mb_core_driver, &nit_framebuffer_core_driver);
        if (retval < 0)
        {
            return -1;
        }
    }

    {
        auto save_path = configuration_path;
        save_path += "/nit_process_core_config.xml";

        std::cout << "loading configuration from: " << save_path << std::endl;

        if (nit_process_core_config_load_from_file(&nit_process_core_driver, save_path.c_str()) < 0)
        {
            if (nit_process_core_config_save_to_file(&nit_process_core_driver, save_path.c_str()) < 0)
            {
                std::cout << "loading configuration failed!" << std::endl;
                return -1;
            }
        }

    }

    try {
        nit_bpc_table_core_bpc_table_load(&nit_bpc_table_core_driver, "/mnt/mmc/sys/bpcc.sys");
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    /**
     * load serial number from BPC file
     * (old behaviour)
     */
    const char* bpcc_path = "/mnt/mmc/sys/bpcc.sys";
    if (std::filesystem::exists(bpcc_path)) {
        std::fstream fs(bpcc_path);
        fs >> config.serial_number;
    }

    return 0;
}

application::application()
    : m_shutdown(false), m_command_server_router({ {1, std::bind(&application::default_handler, this, std::placeholders::_1, std::placeholders::_2)} })
{
    m_timer = std::make_shared<linux_generic_timer>(this->config.global_timer_update_interval_us, m_shutdown);
}

int application::default_handler(const unsigned char* buffer, int)
{
    return 0;
}

std::shared_ptr<application> application::instance = nullptr;
std::shared_ptr<application> application::get_instance()
{
    if (instance == nullptr)
    {
        instance = std::shared_ptr<application>(new application());
    }

    return instance;
}

void application::run()
{

    static std::atomic_bool shutdown = false;

    // Set up signal handling using standard C++ facilities
    std::signal(SIGINT, [](int)
        { shutdown.store(true); });
    std::signal(SIGTERM, [](int)
        { shutdown.store(true); });
    std::signal(SIGPIPE, SIG_IGN);

    auto legacy_command_server_worker = std::thread(tcp_server::create(4097, std::bind(&application::command_processor_legacy, this, std::placeholders::_1), shutdown));
    auto legacy_image_server_worker = std::thread(tcp_server::create(4096, std::bind(&application::image_writer_legacy, this, std::placeholders::_1), shutdown));
    auto legacy_image_reader_worker = std::thread(std::bind(&application::image_reader, this));

    // m_server_threads.push_back(std::move(system_timer_thread));

    m_server_threads.push_back(std::move(legacy_command_server_worker));
    m_server_threads.push_back(std::move(legacy_image_server_worker));
    m_server_threads.push_back(std::move(legacy_image_reader_worker));

    m_server_threads.push_back(std::move(std::thread([this]() -> void {
        nit_process_core_run(&nit_process_core_driver, m_timer, m_shutdown);
        })));

    std::shared_ptr<linux_generic_timer> image_timer = nullptr;

    while (!shutdown.load())
    {
        try
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        catch (std::exception&)
        {
        }
    }

    for (auto& thread : m_server_threads)
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
