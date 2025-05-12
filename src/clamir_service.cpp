#include <arpa/inet.h>
#include <atomic>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/signals2.hpp>
#include <fcntl.h>
#include <filesystem>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

#include <nit/embedded/clamir_service.hpp>
#include <nit/embedded/components/filter.hpp>
#include <nit/embedded/components/timer.hpp>
#include <nit/embedded/drivers/arm_core.h>
#include <nit/embedded/drivers/bpc_table_core.h>
#include <nit/embedded/drivers/common.h>
#include <nit/embedded/drivers/control_unit_core.h>
#include <nit/embedded/drivers/framebuffer_core.h>
#include <nit/embedded/drivers/gen_core_field_table.h>
#include <nit/embedded/drivers/mom_core_field_table.h>
#include <nit/embedded/drivers/process_core.h>
#include <nit/embedded/drivers/process_core_field_table.h>
#include <nit/embedded/drivers/pwm_core_field_table.h>
#include <nit/embedded/drivers/roi_core_field_table.h>
#include <nit/embedded/drivers/scc_core.h>
#include <nit/embedded/math/algorithm.hpp>
#include <nit/embedded/math/control.hpp>
#include <nit/embedded/networking/tcp/protocol_legacy.hpp>
#include <nit/embedded/utils/runnable_worker.hpp>
#include <nit/embedded/utils/time.hpp>
#include <nit/embedded/vision/frame_generator.hpp>

#define DEFINE_COMMAND_TARGET_WRITE_CALLBACK(driver_type, prefix, var_type,    \
                                             var)                              \
  int command_target_##prefix##_##var##_write(                                 \
      std::shared_ptr<clamir_service> app, command_processor_route &route,     \
      packet &req) {                                                           \
    syslog(LOG_INFO, __func__);                                                \
                                                                               \
    prefix##_##var##_set((driver_type##_state_t *)route.pdata, req.value);     \
                                                                               \
    var_type value = req.value;                                                \
    prefix##_##var##_get((driver_type##_state_t *)route.pdata, &value);        \
                                                                               \
    return 0;                                                                  \
  }

#define DEFINE_COMMAND_TARGET_READ_CALLBACK(driver_type, prefix, var_type,     \
                                            var)                               \
  int command_target_##prefix##_##var##_read(                                  \
      std::shared_ptr<clamir_service> app, command_processor_route &route,     \
      packet &req) {                                                           \
    syslog(LOG_INFO, __func__);                                                \
                                                                               \
    var_type value = req.value;                                                \
    prefix##_##var##_get((driver_type##_state_t *)route.pdata, &value);        \
    req.value = value;                                                         \
                                                                               \
    return 0;                                                                  \
  }

using namespace math::control;
using namespace utils::time;
using namespace utils::numeric;

// namespace po = boost::program_options;

namespace std {
bool operator==(const std::thread &a, std::thread &b) {
  return a.get_id() == b.get_id();
}
} // namespace std

int sensor_calibrate() {
  nit_control_unit_core_offset_en_set(&nit_control_unit_core_driver, 1);
  nit_control_unit_core_offset_update_set(&nit_control_unit_core_driver, 1);

  nit_scc_core_calibration_bypass_set(&nit_scc_core_driver, 1);
  nit_scc_core_opmode_set(&nit_scc_core_driver, NIT_SCC_CORE_OPMODE_CALIBRATE);

  /**
   * in the case we want to use the FPGA to do the procedure instead of software
   * 1. Design Rationale: it's not necessary to use the FPGA for calibrating the
   *      sensor and improves complexity of the design.
   */
  nit_scc_core_calibration_mode_set(
      &nit_scc_core_driver, NIT_SCC_CORE_CALIBRATION_STATUS_ACQUIRING_MIN);

  nit_control_unit_core_shutter_set(&nit_control_unit_core_driver, 1);

  std::this_thread::sleep_for(std::chrono::milliseconds(2500));
  nit_scc_core_stub_eval_calibrate_acquire_min(&nit_scc_core_driver);
  std::this_thread::sleep_for(std::chrono::milliseconds(2500));

  /**
   * in the case we want to use the FPGA to do the procedure instead of software
   * 1. Design Rationale: it's not necessary to use the FPGA for calibrating the
   *      sensor and improves complexity of the design.
   */
  nit_scc_core_calibration_mode_set(
      &nit_scc_core_driver, NIT_SCC_CORE_CALIBRATION_STATUS_ACQUIRING_MAX);

  nit_control_unit_core_shutter_set(&nit_control_unit_core_driver, 0);

  std::this_thread::sleep_for(std::chrono::milliseconds(2500));
  nit_scc_core_stub_eval_calibrate_acquire_max(&nit_scc_core_driver);
  std::this_thread::sleep_for(std::chrono::milliseconds(2500));

  // nit_scc_core_stub_eval_calibrate_update(&nit_scc_core_driver);

  // in the case we use FPGA for calibrating the sensor...
  // nit_scc_core_calibration_mode_wait_idle(&nit_scc_core_driver);

  nit_scc_core_opmode_set(&nit_scc_core_driver, NIT_SCC_CORE_OPMODE_PROCESS);

  if (!nit_scc_core_driver.config.calibration_bypass) {
    nit_scc_core_calibration_bypass_set(&nit_scc_core_driver, 0);
  }

  /**
   * older offset core processing disable offset update
   */
  nit_control_unit_core_offset_update_set(&nit_control_unit_core_driver, 0);

  return 0;
}

int clamir_service::initialize(int argc, char *argv[]) {

  const char *CONFIGURATION_DIRECTORY = std::getenv("CONFIGURATION_DIRECTORY");

  if (CONFIGURATION_DIRECTORY != nullptr) {
    syslog(LOG_INFO, "CONFIGURATION_DIRECTORY: %s", CONFIGURATION_DIRECTORY);
  } else {
    syslog(LOG_INFO,
           "CONFIGURATION_DIRECTORY environment variable is not set.");
    CONFIGURATION_DIRECTORY = std::filesystem::current_path().c_str();
  }

  std::filesystem::path configuration_path = CONFIGURATION_DIRECTORY;

  try {
    std::filesystem::create_directory(configuration_path);
  } catch (std::exception &ex) {
    syslog(LOG_INFO, "%s", ex.what());
  }

  auto app_config_path = std::string(CONFIGURATION_DIRECTORY) + "/clamir.xml";

  if (!std::filesystem::exists(app_config_path)) {
    try {
      std::ofstream ofs(app_config_path);
      boost::archive::xml_oarchive xoa(ofs);
      xoa &boost::make_nvp("config", this->config_default);
    } catch (std::exception &ex) {
      syslog(LOG_INFO, "%s", ex.what());
      return -1;
    }
  }

  try {
    std::ifstream ifs(app_config_path);
    boost::archive::xml_iarchive xia(ifs);
    xia &boost::make_nvp("config", this->config);
  } catch (std::exception &ex) {
    syslog(LOG_INFO, "%s", ex.what());
    return -1;
  }

  {
    int retval = nit_arm_core_open(&nit_arm_core_driver);

    if (retval < 0) {
      return -1;
    }
  }

  {
    int retval = nit_bpc_table_core_open(&nit_bpc_table_core_driver);
    if (retval < 0) {
      return -1;
    }
  }

  {
    int retval = nit_control_unit_core_open(&nit_control_unit_core_driver);
    if (retval < 0) {
      return -1;
    }
  }

  {

    nit_framebuffer_core_config_t config = {
        .operating_mode = NIT_FRAMEBUFFER_CORE_OPERATING_MODE_NORMAL};

    if (host_mockup) {
      config.operating_mode =
          NIT_FRAMEBUFFER_CORE_OPERATING_MODE_TEST_PATTERN_BALL;
    }

    int retval =
        nit_framebuffer_core_open(&nit_framebuffer_core_driver, &config);

    if (retval < 0) {
      return -1;
    }
  }

  {
    int retval = nit_mb_core_open(&nit_mb_core_driver);
    if (retval < 0) {
      return -1;
    }
  }

  {
    auto save_path = configuration_path;
    save_path += "/nit_scc_core_config.xml";

    syslog(LOG_INFO, "loading configuration from: %s", save_path.c_str());

    if (nit_scc_core_config_load_from_file(&nit_scc_core_driver,
                                           save_path.c_str()) < 0) {
      if (nit_scc_core_config_save_to_file(&nit_scc_core_driver,
                                           save_path.c_str()) < 0) {
        syslog(LOG_INFO, "loading configuration failed!");
        return -1;
      }
    }

    if (nit_scc_core_config_load_from_file(&nit_scc_core_driver,
                                           save_path.c_str()) < 0) {
      syslog(LOG_INFO, "loading configuration failed!");
      return -1;
    }
  }

  {
    int retval =
        nit_scc_core_open(&nit_scc_core_driver, &nit_framebuffer_core_driver);
    if (retval < 0) {
      return -1;
    }
  }

  /**
   * Configuration Defaults from Previous Releases
   */

  // nit_mb_core_pwm_limit_max_set: NIT_MB_CORE_PWM_LIMIT_MAX_OFFSET
  // (0x0000C000): 1117
  nit_pwm_core_pwm_limit_max_set(&nit_mb_core_driver, 1117);

  // nit_mb_core_pwm_limit_min_set: NIT_MB_CORE_PWM_LIMIT_MIN_OFFSET
  // (0x0000C003): 372
  nit_pwm_core_pwm_limit_min_set(&nit_mb_core_driver, 372);

  // nit_mb_core_start_track_mom_t_set: NIT_MB_CORE_START_TRACK_MOM_T_OFFSET
  // (0x00004009): 40
  nit_mom_core_start_track_mom_t_set(&nit_mb_core_driver, 40);

  // nit_mb_core_end_of_track_set: NIT_MB_CORE_END_OF_TRACK_OFFSET (0x00004010):
  // 30
  nit_mom_core_end_of_track_set(&nit_mb_core_driver, 30);

  // nit_mb_core_mode_set: NIT_MB_CORE_MODE_OFFSET (0x0000400B): 2
  nit_mom_core_mode_set(&nit_mb_core_driver, 2);

  // nit_mb_core_reference_track_set: NIT_MB_CORE_REFERENCE_TRACK_OFFSET
  // (0x0000400F): 3
  nit_mom_core_reference_track_set(&nit_mb_core_driver, 3);

  // nit_mb_core_time_track_low_set: NIT_MB_CORE_TIME_TRACK_LOW_OFFSET
  // (0x0000400C): 200000000
  nit_mom_core_time_track_low_set(&nit_mb_core_driver, 200000000);

  // nit_mb_core_time_track_high_set: NIT_MB_CORE_TIME_TRACK_HIGH_OFFSET
  // (0x0000400D): 0
  nit_mom_core_time_track_high_set(&nit_mb_core_driver, 0);

  // nit_mb_core_threshold_set: NIT_MB_CORE_THRESHOLD_OFFSET (0x00004000): 1182
  nit_mom_core_threshold_set(&nit_mb_core_driver, 1182);

  // nit_mb_core_roi_round_set: NIT_MB_CORE_ROI_ROUND_OFFSET (0x00008004): 0
  nit_roi_core_round_set(&nit_mb_core_driver, 0);

  // nit_mb_core_enable_roi_set: NIT_MB_CORE_ENABLE_ROI_OFFSET (0x00000002): 1
  nit_gen_core_enable_roi_set(&nit_mb_core_driver, 0);

  // nit_mb_core_roi_x1_set: NIT_MB_CORE_ROI_X1_OFFSET (0x00008000): 9
  nit_roi_core_x1_set(&nit_mb_core_driver, 15);

  // nit_mb_core_roi_x2_set: NIT_MB_CORE_ROI_X2_OFFSET (0x00008002): 53
  nit_roi_core_x2_set(&nit_mb_core_driver, 50);

  // nit_mb_core_roi_y1_set: NIT_MB_CORE_ROI_Y1_OFFSET (0x00008001): 8
  nit_roi_core_y1_set(&nit_mb_core_driver, 15);

  // nit_mb_core_roi_y2_set: NIT_MB_CORE_ROI_Y2_OFFSET (0x00008003): 52
  nit_roi_core_y2_set(&nit_mb_core_driver, 50);

  // nit_mb_core_digital_out_conf_set: NIT_MB_CORE_DIGITAL_OUT_CONF_OFFSET
  // (0x0000000B): 0
  nit_gen_core_digital_out_conf_set(&nit_mb_core_driver, 0);

  // control_unit_black_level_set: NIT_CONTROL_UNIT_BLACK_LEVEL_OFFSET
  // (0x00000024): 1000
  nit_control_unit_core_black_level_set(&nit_control_unit_core_driver, 1000);

  // control_unit_bias_v_set: NIT_CONTROL_UNIT_BIAS_V_OFFSET (0x00000001): 13652
  nit_control_unit_core_bias_v_set(&nit_control_unit_core_driver, 13652);

  // control_unit_int_time_set: NIT_CONTROL_UNIT_INT_TIME_OFFSET (0x00000000):
  // 600
  nit_control_unit_core_int_time_set(&nit_control_unit_core_driver, 600);

  // control_unit_drift_enable_set: NIT_CONTROL_UNIT_DRIFT_ENABLE_OFFSET
  // (0x00000010): 1
  nit_control_unit_core_drift_enable_set(&nit_control_unit_core_driver, 1);

  // control_unit_drift_position_set: NIT_CONTROL_UNIT_DRIFT_POSITION_OFFSET
  // (0x00000011): 14
  nit_control_unit_core_drift_position_set(&nit_control_unit_core_driver, 14);

  // nit_mb_core_change_op_mode_set: NIT_MB_CORE_CHANGE_OP_MODE_OFFSET
  // (0x00000005): 1
  nit_gen_core_change_op_mode_set(&nit_mb_core_driver, 1);

  // control_unit_bpc_en_set: NIT_CONTROL_UNIT_BPC_EN_OFFSET (0x00000005): 1
  nit_control_unit_core_bpc_en_set(&nit_control_unit_core_driver, 1);

  nit_control_unit_core_offset_update_set(&nit_control_unit_core_driver, 1);

  {
    auto save_path = configuration_path;
    save_path += "/nit_arm_core_config.xml";

    syslog(LOG_INFO, "loading configuration from: %s", save_path.c_str());

    if (nit_arm_core_config_load_from_file(&nit_arm_core_driver,
                                           save_path.c_str()) < 0) {
      if (nit_arm_core_config_save_to_file(&nit_arm_core_driver,
                                           save_path.c_str()) < 0) {
        syslog(LOG_INFO, "loading configuration failed!");
        return -1;
      }
    }
  }

  {
    auto save_path = configuration_path;
    save_path += "/nit_control_unit_core_config.xml";

    syslog(LOG_INFO, "loading configuration from: %s", save_path.c_str());

    if (nit_control_unit_core_config_load_from_file(
            &nit_control_unit_core_driver, save_path.c_str()) < 0) {
      if (nit_control_unit_core_config_save_to_file(
              &nit_control_unit_core_driver, save_path.c_str()) < 0) {
        syslog(LOG_INFO, "loading configuration failed!");
        return -1;
      }
    }
  }

  {
    auto save_path = configuration_path;
    save_path += "/nit_mb_core_config.xml";

    syslog(LOG_INFO, "loading configuration from: %s", save_path.c_str());

    if (nit_mb_core_config_load_from_file(&nit_mb_core_driver,
                                          save_path.c_str()) < 0) {
      if (nit_mb_core_config_save_to_file(&nit_mb_core_driver,
                                          save_path.c_str()) < 0) {
        syslog(LOG_INFO, "loading configuration failed!");
        return -1;
      }
    }
  }

  {
    int retval = nit_process_core_open(
        &nit_process_core_driver, &nit_arm_core_driver,
        &nit_control_unit_core_driver, &nit_mb_core_driver,
        &nit_framebuffer_core_driver);
    if (retval < 0) {
      return -1;
    }
  }

  {
    auto save_path = configuration_path;
    save_path += "/nit_process_core_config.xml";

    syslog(LOG_INFO, "loading configuration from: %s", save_path.c_str());

    if (nit_process_core_config_load_from_file(&nit_process_core_driver,
                                               save_path.c_str()) < 0) {
      if (nit_process_core_config_save_to_file(&nit_process_core_driver,
                                               save_path.c_str()) < 0) {
        syslog(LOG_INFO, "loading configuration failed!");
        return -1;
      }
    }
  }

  try {
    nit_bpc_table_core_bpc_table_load(&nit_bpc_table_core_driver,
                                      "/mnt/mmc/sys/bpcc.sys");
  } catch (std::exception &e) {
    syslog(LOG_INFO, "%s", e.what());
  }

  /**
   * load serial number from BPC file
   * (old behaviour)
   */
  const char *bpcc_path = "/mnt/mmc/sys/bpcc.sys";
  if (std::filesystem::exists(bpcc_path)) {
    std::fstream fs(bpcc_path);
    fs >> config.serial_number;
  }

  return 0;
}

clamir_service::clamir_service()
    : m_shutdown(false),
      m_command_server_router(
          {{1, std::bind(&clamir_service::default_handler, this,
                         std::placeholders::_1, std::placeholders::_2)}}) {
  m_timer = std::make_shared<linux_generic_timer>(
      this->config.global_timer_update_interval_us, m_shutdown);
}

int clamir_service::default_handler(const unsigned char *buffer, int) {
  return 0;
}

std::shared_ptr<clamir_service> clamir_service::instance = nullptr;
std::shared_ptr<clamir_service> clamir_service::get_instance() {
  if (instance == nullptr) {
    instance = std::shared_ptr<clamir_service>(new clamir_service());
  }

  return instance;
}

void clamir_service::run(std::atomic_bool &shutdown) {

  if (!std::filesystem::exists("/run/lock/clamir/ctrl")) {
    std::filesystem::create_directories("/run/lock/clamir/ctrl");
  }

  if (!std::filesystem::exists("/run/lock/clamir/ctrl/process.lock")) {
    system("touch /run/lock/clamir/ctrl/process.lock");
    m_server_threads.push_back(std::thread([this]() -> void {
      nit_process_core_run(&nit_process_core_driver, m_timer, m_shutdown);
    }));
  }

  std::shared_ptr<linux_generic_timer> image_timer = nullptr;

  while (!shutdown) {
    try {
      std::this_thread::sleep_for(std::chrono::milliseconds(  1000));
    } catch (std::exception &) {
    }
  }

  for (auto &thread : m_server_threads) {
    if (thread.joinable()) {
      m_shutdown = true;
      thread.join();
    }
  }
  system("rm /run/lock/clamir/ctrl/process.lock");
  syslog(LOG_INFO, "shutting down system");
}

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core,
                                     nit_control_unit_core, uint16_t, int_time)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core,
                                    nit_control_unit_core, uint16_t, int_time)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core,
                                     nit_control_unit_core, uint16_t, bias_v)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core,
                                    nit_control_unit_core, uint16_t, bias_v)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core,
                                     nit_control_unit_core, uint16_t, offset_en)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core,
                                    nit_control_unit_core, uint16_t, offset_en)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core,
                                     nit_control_unit_core, uint16_t,
                                     offset_update)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core,
                                    nit_control_unit_core, uint16_t,
                                    offset_update)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core,
                                     nit_control_unit_core, uint16_t, shutter)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core,
                                    nit_control_unit_core, uint16_t, shutter)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core,
                                     nit_control_unit_core, uint16_t, bpc_en)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core,
                                    nit_control_unit_core, uint16_t, bpc_en)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core,
                                     nit_control_unit_core, uint16_t,
                                     bpc_mem_write)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core,
                                    nit_control_unit_core, uint16_t,
                                    bpc_mem_write)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core,
                                     nit_control_unit_core, uint16_t,
                                     bpc_identify)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core,
                                    nit_control_unit_core, uint16_t,
                                    bpc_identify)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core,
                                     nit_control_unit_core, uint16_t, temp1)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core,
                                    nit_control_unit_core, uint16_t, temp1)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core,
                                     nit_control_unit_core, uint16_t, temp2)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core,
                                    nit_control_unit_core, uint16_t, temp2)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core,
                                     nit_control_unit_core, uint16_t, temp3)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core,
                                    nit_control_unit_core, uint16_t, temp3)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core,
                                     nit_control_unit_core, uint16_t, temp4)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core,
                                    nit_control_unit_core, uint16_t, temp4)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core,
                                     nit_control_unit_core, uint16_t,
                                     trigger_usec)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core,
                                    nit_control_unit_core, uint16_t,
                                    trigger_usec)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core,
                                     nit_control_unit_core, uint16_t,
                                     black_level)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core,
                                    nit_control_unit_core, uint16_t,
                                    black_level)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core,
                                     nit_control_unit_core, uint16_t,
                                     sincronization)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core,
                                    nit_control_unit_core, uint16_t,
                                    sincronization)

int command_target_nit_control_unit_core_save_embedded_conf_write(
    std::shared_ptr<clamir_service> app, command_processor_route &route,
    packet &req) {
  syslog(LOG_INFO, __func__);
  app->save_all();
  return 0;
}

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core,
                                    nit_control_unit_core, uint16_t,
                                    save_embedded_conf)

int command_target_nit_control_unit_core_arm_sw_version_write(
    std::shared_ptr<clamir_service> app, command_processor_route &route,
    packet &req) {
  syslog(LOG_INFO, __func__);
  return 0;
}

int command_target_nit_control_unit_core_arm_sw_version_read(
    std::shared_ptr<clamir_service> app, command_processor_route &route,
    packet &req) {
  syslog(LOG_INFO, __func__);
  req.value = app->config_get().version_major;
  return 0;
}

int command_target_nit_control_unit_core_drift_enable_write(
    std::shared_ptr<clamir_service> app, command_processor_route &route,
    packet &req) {
  syslog(LOG_INFO, __func__);
  nit_control_unit_core_drift_enable_set(
      (nit_control_unit_core_state_t *)route.pdata, req.value);
  uint16_t value = req.value;
  nit_control_unit_core_drift_enable_get(
      (nit_control_unit_core_state_t *)route.pdata, &value);
  return 0;
}

int command_target_nit_control_unit_core_drift_enable_read(
    std::shared_ptr<clamir_service> app, command_processor_route &route,
    packet &req) {
  syslog(LOG_INFO, __func__);
  uint16_t value = req.value;
  nit_control_unit_core_drift_enable_get(
      (nit_control_unit_core_state_t *)route.pdata, &value);
  req.value = value;
  return 0;
}

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core,
                                     nit_control_unit_core, uint16_t,
                                     drift_position)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core,
                                    nit_control_unit_core, uint16_t,
                                    drift_position)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_control_unit_core,
                                     nit_control_unit_core, uint16_t,
                                     drift_level)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_control_unit_core,
                                    nit_control_unit_core, uint16_t,
                                    drift_level)

int command_target_nit_control_unit_core_magic_id_write(
    std::shared_ptr<clamir_service> app, command_processor_route &route,
    packet &req) {
  syslog(LOG_INFO, __func__);
  return 0;
}

int command_target_nit_control_unit_core_magic_id_read(
    std::shared_ptr<clamir_service> app, command_processor_route &route,
    packet &req) {
  syslog(LOG_INFO, __func__);
  req.value = 0x0700;
  return 0;
}

int command_target_nit_control_unit_core_fpga_version_write(
    std::shared_ptr<clamir_service> app, command_processor_route &route,
    packet &req) {
  syslog(LOG_INFO, __func__);
  return 0;
}

int command_target_nit_control_unit_core_fpga_version_read(
    std::shared_ptr<clamir_service> app, command_processor_route &route,
    packet &req) {
  syslog(LOG_INFO, __func__);
  req.value = 0x0002;
  return 0;
}

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_mom_core, uint16_t,
                                     start_track_mom_t)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_mom_core, uint16_t,
                                    start_track_mom_t)

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_mom_core, uint16_t,
                                     end_of_track)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_mom_core, uint16_t,
                                    end_of_track)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, mode)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_mom_core, uint16_t, mode)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_mom_core, uint16_t,
                                     reference_track)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_mom_core, uint16_t,
                                    reference_track)

// DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_mom_core, uint32_t,
// time_track_low) DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core,
// nit_mom_core, uint32_t, time_track_low)
#include <nit/embedded/drivers/common.h>

int command_target_nit_mom_core_time_track_low_read(
    std::shared_ptr<clamir_service> app, command_processor_route &route,
    packet &req) {
  syslog(LOG_INFO, __func__);

  uint32_t high = 0;
  uint32_t low = 0;

  if (nit_mom_core_time_track_high_get(&nit_mb_core_driver, &high) < 0) {
    return -1;
  }

  if (nit_mom_core_time_track_low_get(&nit_mb_core_driver, &low) < 0) {
    return -1;
  }

  uint16_t value = ((((uint64_t)high << 32) | ((uint64_t)low << 0)) / 100e5);

  req.value = value;

  return 0;
}

int command_target_nit_mom_core_time_track_low_write(
    std::shared_ptr<clamir_service> app, command_processor_route &route,
    packet &req) {
  syslog(LOG_INFO, __func__);

  uint64_t value = 0;
  value = req.value;
  value *= 100e5;

  if (nit_mom_core_time_track_high_set(
          &nit_mb_core_driver, (value & 0xFFFFFFFF00000000) >> 32) < 0) {
    return -1;
  }

  if (nit_mom_core_time_track_low_set(&nit_mb_core_driver,
                                      (value & 0x00000000FFFFFFFF) >> 0) < 0) {
    return -2;
  }

  return 0;
}

DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_arm_core, nit_arm_core, uint16_t,
                                     soft_reset)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_arm_core, nit_arm_core, uint16_t,
                                    soft_reset)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_mom_core, uint16_t,
                                     threshold)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_mom_core, uint16_t,
                                    threshold)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_pwm_core, uint16_t, pwm)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_pwm_core, uint16_t, pwm)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, round)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, round)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_gen_core, uint16_t,
                                    enable_roi)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_gen_core, uint16_t,
                                     enable_roi)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, x1)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, x1)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, y1)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, y1)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, x2)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, x2)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, y2)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_roi_core, uint16_t, y2)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_arm_core, nit_arm_core, uint16_t, led_r)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_arm_core, nit_arm_core, uint16_t,
                                     led_r)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_arm_core, nit_arm_core, uint16_t, led_g)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_arm_core, nit_arm_core, uint16_t,
                                     led_g)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_arm_core, nit_arm_core, uint16_t, led_b)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_arm_core, nit_arm_core, uint16_t,
                                     led_b)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_pwm_core, uint16_t,
                                    pwm_limit_max)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_pwm_core, uint16_t,
                                     pwm_limit_max)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_pwm_core, uint16_t,
                                    pwm_limit_min)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_pwm_core, uint16_t,
                                     pwm_limit_min)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_gen_core, uint16_t,
                                    digital_out_conf)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_gen_core, uint16_t,
                                     digital_out_conf)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_gen_core, uint16_t,
                                    digital_out_0)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_gen_core, uint16_t,
                                     digital_out_0)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_gen_core, uint16_t,
                                    digital_out_1)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_gen_core, uint16_t,
                                     digital_out_1)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_gen_core, uint16_t,
                                    digital_out_2)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_gen_core, uint16_t,
                                     digital_out_2)
DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_mb_core, nit_gen_core, uint16_t,
                                    digital_out_3)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_mb_core, nit_gen_core, uint16_t,
                                     digital_out_3)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, kp)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, kp)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, ki)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, ki)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, kd)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, kd)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, max_power)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, max_power)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, min_power)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, min_power)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, power_man)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, power_man)

int command_target_scc_core_calibrate_read(std::shared_ptr<clamir_service> app,
                                           command_processor_route &route,
                                           packet &req) {
  syslog(LOG_INFO, __func__);
  uint32_t value = req.value;
  req.value = value;
  return 0;
}

int command_target_scc_core_calibrate_write(std::shared_ptr<clamir_service> app,
                                            command_processor_route &route,
                                            packet &req) {
  nit_scc_core_calibrate(&nit_scc_core_driver);
  return 0;
}

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, background_remove)
// DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
// uint32_t, background_remove)

int command_target_nit_process_core_background_remove_write(
    std::shared_ptr<clamir_service> app, command_processor_route &route,
    packet &req) {

  if (!app->config_get().sensor_calibation_disable) {
    sensor_calibrate();
  }

  nit_control_unit_core_shutter_set(&nit_control_unit_core_driver, 1);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  nit_control_unit_core_offset_en_set(&nit_control_unit_core_driver, 1);
  nit_control_unit_core_offset_update_set(&nit_control_unit_core_driver, 1);

  std::this_thread::sleep_for(std::chrono::milliseconds(250));

  nit_control_unit_core_shutter_set(&nit_control_unit_core_driver, 0);

  nit_control_unit_core_offset_update_set(&nit_control_unit_core_driver, 0);

  return 0;
}

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, set_ref_width)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, set_ref_width)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, power_limit_max)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, power_limit_max)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, power_limit_min)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, power_limit_min)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, width_ref)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, width_ref)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, pixel_mm_ratio)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, pixel_mm_ratio)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, pid_error)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, pid_error)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, end_of_process)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, end_of_process)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, limit_integral)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, limit_integral)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, limit_slew)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, limit_slew)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, buff_size)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, buff_size)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, enable_alarm)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, enable_alarm)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, alarm_max)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, alarm_max)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, alarm_min)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, alarm_min)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, alarm_time)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, alarm_time)

int command_target_nit_gen_core_serial_number_low_write(
    std::shared_ptr<clamir_service> app, command_processor_route &route,
    packet &req) {
  syslog(LOG_INFO, __func__);
  return 0;
}

int command_target_nit_gen_core_serial_number_low_read(
    std::shared_ptr<clamir_service> app, command_processor_route &route,
    packet &req) {
  syslog(LOG_INFO, __func__);

  const char *serial_number = app->config_get().serial_number.c_str();

  req.route_id_set(0x04aa);
  req.value = serial_number[1] << 8 | serial_number[0];

  req.route_id_set(0x04ab);
  req.value = serial_number[3] << 8 | serial_number[2];

  req.route_id_set(0x04ac);
  req.value = serial_number[5] << 8 | serial_number[4];

  req.route_id_set(0x04ad);
  req.value = serial_number[7] << 8 | serial_number[6];

  return 0;
}

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, serial_number_low)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, serial_number_low)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, automeasure)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, automeasure)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, autoshutter_config)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, autoshutter_config)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, autoshutter_temp)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, autoshutter_temp)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, autoshutter_timer)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, autoshutter_timer)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, track_ref_start)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, track_ref_start)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, laser_external_control)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, laser_external_control)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, delay_laser_on)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, delay_laser_on)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, preheating_ena)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, preheating_ena)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, preheating_time)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, preheating_time)

DEFINE_COMMAND_TARGET_READ_CALLBACK(nit_process_core, nit_process_core,
                                    uint32_t, preheating_power)
DEFINE_COMMAND_TARGET_WRITE_CALLBACK(nit_process_core, nit_process_core,
                                     uint32_t, preheating_power)

std::map<uint16_t, command_processor_route>
    clamir_service::command_processor_routes_legacy = {

        {0x0501,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_int_time_read,
          .write = command_target_nit_control_unit_core_int_time_write}},
        {0x0502,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_bias_v_read,
          .write = command_target_nit_control_unit_core_bias_v_write}},
        {0x0503,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_offset_en_read,
          .write = command_target_nit_control_unit_core_offset_en_write}},
        {0x0504,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_offset_update_read,
          .write = command_target_nit_control_unit_core_offset_update_write}},
        {0x0505,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_shutter_read,
          .write = command_target_nit_control_unit_core_shutter_write}},
        {0x0506,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_bpc_en_read,
          .write = command_target_nit_control_unit_core_bpc_en_write}},
        {0x0507,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_bpc_mem_write_read,
          .write = command_target_nit_control_unit_core_bpc_mem_write_write}},
        {0x0508,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_bpc_identify_read,
          .write = command_target_nit_control_unit_core_bpc_identify_write}},
        {0x0509,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_temp1_read,
          .write = command_target_nit_control_unit_core_temp1_write}},
        {0x050A,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_temp2_read,
          .write = command_target_nit_control_unit_core_temp2_write}},
        {0x050B,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_temp3_read,
          .write = command_target_nit_control_unit_core_temp3_write}},
        {0x050C,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_temp4_read,
          .write = command_target_nit_control_unit_core_temp4_write}},

        {0x050D,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_drift_enable_read,
          .write = command_target_nit_control_unit_core_drift_enable_write}},
        {0x050E,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_drift_position_read,
          .write = command_target_nit_control_unit_core_drift_position_write}},
        {0x050F,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_drift_level_read,
          .write = command_target_nit_control_unit_core_drift_level_write}},

        {0x0520,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_trigger_usec_read,
          .write = command_target_nit_control_unit_core_trigger_usec_write}},
        {0x0521,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_black_level_read,
          .write = command_target_nit_control_unit_core_black_level_write}},
        {0x0522,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_sincronization_read,
          .write = command_target_nit_control_unit_core_sincronization_write}},
        {0x0523,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_save_embedded_conf_read,
          .write =
              command_target_nit_control_unit_core_save_embedded_conf_write}},

        {0x0524,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_arm_sw_version_read,
          .write = command_target_nit_control_unit_core_arm_sw_version_write}},
        {0x0525,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_magic_id_read,
          .write = command_target_nit_control_unit_core_magic_id_write}},
        {0x0526,
         {.pdata = (void *)&nit_control_unit_core_driver,
          .read = command_target_nit_control_unit_core_fpga_version_read,
          .write = command_target_nit_control_unit_core_fpga_version_write}},

        {0x0402,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_kp_read,
          .write = command_target_nit_process_core_kp_write}},
        {0x0401,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_ki_read,
          .write = command_target_nit_process_core_ki_write}},
        {0x0403,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_kd_read,
          .write = command_target_nit_process_core_kd_write}},

        {0x0404,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_max_power_read,
          .write = command_target_nit_process_core_max_power_write}},
        {0x0405,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_min_power_read,
          .write = command_target_nit_process_core_min_power_write}},
        {0x0407,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_power_man_read,
          .write = command_target_nit_process_core_power_man_write}},
        {0x0409,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_background_remove_read,
          .write = command_target_nit_process_core_background_remove_write}},

        {0x040D,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_set_ref_width_read,
          .write = command_target_nit_process_core_set_ref_width_write}},
        {0x0418,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_power_limit_max_read,
          .write = command_target_nit_process_core_power_limit_max_write}},
        {0x0419,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_power_limit_min_read,
          .write = command_target_nit_process_core_power_limit_min_write}},
        {0x041F,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_width_ref_read,
          .write = command_target_nit_process_core_width_ref_write}},
        {0x0420,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_pixel_mm_ratio_read,
          .write = command_target_nit_process_core_pixel_mm_ratio_write}},
        {0x0421,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_pid_error_read,
          .write = command_target_nit_process_core_pid_error_write}},
        {0x0422,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_end_of_process_read,
          .write = command_target_nit_process_core_end_of_process_write}},
        {0x0423,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_limit_integral_read,
          .write = command_target_nit_process_core_limit_integral_write}},
        {0x0424,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_limit_slew_read,
          .write = command_target_nit_process_core_limit_slew_write}},
        {0x0425,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_buff_size_read,
          .write = command_target_nit_process_core_buff_size_write}},
        {0x04A5,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_buff_size_read,
          .write = command_target_nit_process_core_buff_size_write}},
        {0x0426,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_enable_alarm_read,
          .write = command_target_nit_process_core_enable_alarm_write}},
        {0x0427,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_alarm_max_read,
          .write = command_target_nit_process_core_alarm_max_write}},
        {0x0428,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_alarm_min_read,
          .write = command_target_nit_process_core_alarm_min_write}},
        {0x0429,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_alarm_time_read,
          .write = command_target_nit_process_core_alarm_time_write}},
        {0x042A,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_gen_core_serial_number_low_read,
          .write = command_target_nit_gen_core_serial_number_low_write}},
        {0x042E,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_automeasure_read,
          .write = command_target_nit_process_core_automeasure_write}},
        {0x042F,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_autoshutter_config_read,
          .write = command_target_nit_process_core_autoshutter_config_write}},
        {0x0431,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_autoshutter_temp_read,
          .write = command_target_nit_process_core_autoshutter_temp_write}},
        {0x0432,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_autoshutter_timer_read,
          .write = command_target_nit_process_core_autoshutter_timer_write}},
        {0x0433,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_track_ref_start_read,
          .write = command_target_nit_process_core_track_ref_start_write}},
        {0x0434,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_laser_external_control_read,
          .write =
              command_target_nit_process_core_laser_external_control_write}},
        {0x0435,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_delay_laser_on_read,
          .write = command_target_nit_process_core_delay_laser_on_write}},
        {0x0436,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_preheating_ena_read,
          .write = command_target_nit_process_core_preheating_ena_write}},
        {0x0437,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_preheating_time_read,
          .write = command_target_nit_process_core_preheating_time_write}},
        {0x0438,
         {.pdata = (void *)&nit_process_core_driver,
          .read = command_target_nit_process_core_preheating_power_read,
          .write = command_target_nit_process_core_preheating_power_write}},
        {0x0406,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_mom_core_start_track_mom_t_read,
          .write = command_target_nit_mom_core_start_track_mom_t_write}},
        {0x040a,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_mom_core_mode_read,
          .write = command_target_nit_mom_core_mode_write}},
        {0x0408,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_mom_core_end_of_track_read,
          .write = command_target_nit_mom_core_end_of_track_write}},

        {0x040B,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_mom_core_reference_track_read,
          .write = command_target_nit_mom_core_reference_track_write}},
        {0x040C,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_mom_core_time_track_low_read,
          .write = command_target_nit_mom_core_time_track_low_write}},
        {0x040E,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_arm_core_soft_reset_read,
          .write = command_target_nit_arm_core_soft_reset_write}},
        {0x040F,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_mom_core_threshold_read,
          .write = command_target_nit_mom_core_threshold_write}},

        {0x0410,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_roi_core_round_read,
          .write = command_target_nit_roi_core_round_write}},
        {0x0411,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_gen_core_enable_roi_read,
          .write = command_target_nit_gen_core_enable_roi_write}},
        {0x0412,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_roi_core_x1_read,
          .write = command_target_nit_roi_core_x1_write}},
        {0x0413,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_roi_core_y1_read,
          .write = command_target_nit_roi_core_y1_write}},
        {0x0414,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_roi_core_x2_read,
          .write = command_target_nit_roi_core_x2_write}},
        {0x0415,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_roi_core_y2_read,
          .write = command_target_nit_roi_core_y2_write}},
        {0x0416,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_mom_core_mode_read,
          .write = command_target_nit_mom_core_mode_write}},
        {0x041A,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_arm_core_led_r_read,
          .write = command_target_nit_arm_core_led_r_write}},
        {0x041B,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_arm_core_led_g_read,
          .write = command_target_nit_arm_core_led_g_write}},
        {0x041C,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_arm_core_led_b_read,
          .write = command_target_nit_arm_core_led_b_write}},
        {0x0417,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_pwm_core_pwm_read,
          .write = command_target_nit_pwm_core_pwm_write}},
        {0x041D,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_pwm_core_pwm_limit_max_read,
          .write = command_target_nit_pwm_core_pwm_limit_max_write}},
        {0x041E,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_pwm_core_pwm_limit_min_read,
          .write = command_target_nit_pwm_core_pwm_limit_min_write}},
        {0x0430,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_gen_core_digital_out_conf_read,
          .write = command_target_nit_gen_core_digital_out_conf_write}},
        {0x0439,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_gen_core_digital_out_0_read,
          .write = command_target_nit_gen_core_digital_out_0_write}},
        {0x043A,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_gen_core_digital_out_1_read,
          .write = command_target_nit_gen_core_digital_out_1_write}},
        {0x043B,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_gen_core_digital_out_2_read,
          .write = command_target_nit_gen_core_digital_out_2_write}},
        {0x043C,
         {.pdata = (void *)&nit_mb_core_driver,
          .read = command_target_nit_gen_core_digital_out_3_read,
          .write = command_target_nit_gen_core_digital_out_3_write}}};

int32_t clamir_service::command_processor_legacy(int32_t request) {

  auto app = clamir_service::get_instance();

  packet p(request);

  auto route_id = p.route_id_get();
  // auto rw = p.rw.get();

  auto it = command_processor_routes_legacy.find(route_id);

  if (it == command_processor_routes_legacy.end()) {
    printf("couldn't find route %04x\n", route_id);
    return p.value;
  }

  auto &[id, route] = *it;

  switch (p.rw) {
  case 0:

    if (route.write == nullptr)
      break;
    route.write(app, route, p);

    break;
  case 1:
    if (route.read == nullptr)
      break;
    route.read(app, route, p);
    break;

  default:
    break;
  }

  return p.value;
}