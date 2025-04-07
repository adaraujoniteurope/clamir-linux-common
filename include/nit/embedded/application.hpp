#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <chrono>
#include <thread>
#include <iomanip>
#include <filesystem>
#include <mutex>
#include <list>
#include <unordered_map>
#include <map>
#include <iostream>
#include <memory>

#include <signal.h>

#include <nit/embedded/common/defs.h>
#include <nit/embedded/drivers/arm_core.h>
#include <nit/embedded/drivers/gen_core.h>
#include <nit/embedded/drivers/mom_core.h>
#include <nit/embedded/drivers/pwm_core.h>
#include <nit/embedded/drivers/roi_core.h>
#include <nit/embedded/drivers/process_core.h>
#include <nit/embedded/drivers/control_unit_core.h>
#include <nit/embedded/drivers/bpc_table_core.h>
#include <nit/embedded/drivers/framebuffer_core.h>

#include <nit/embedded/networking/tcp/server.hpp>
#include <nit/embedded/signals/event_emitter.hpp>
#include <nit/embedded/components/timer.hpp>

#include <nit/embedded/math/control.hpp>
#include <nit/embedded/networking/tcp/protocol_legacy.hpp>

#include <nit/embedded/application_config.hpp>

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

#undef DRIVER_CALLBACK_INDEX_TABLE_ITEM

class application;

struct command_processor_route
{
	using write_callback_type = std::function<void(std::shared_ptr<application>, command_processor_route &, packet &, int)>;
	using read_callback_type = std::function<void(std::shared_ptr<application>, command_processor_route &, packet &, int)>;
	void* pdata;
	read_callback_type read;
	write_callback_type write;
};

class application : std::enable_shared_from_this<application>
{
	application();

public:


#ifdef NIT_CLAMIR_HOST_MOCKUP
	static const bool host_mockup = true;
	#else
	static const bool host_mockup = false;
#endif

	virtual ~application()
	{
		std::cout << __func__ << std::endl;
	}

	int default_handler(const unsigned char *buffer, int);
	static std::shared_ptr<application> get_instance();
	int initialize(int argc, char *argv[]);
	void run();

	void save_all()
	{
		std::string CONFIGURATION_DIRECTORY = ".";
		
		try {
			CONFIGURATION_DIRECTORY = std::string(std::getenv("CONFIGURATION_DIRECTORY"));
		} catch (std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
		}

		nit_arm_core_config_save_to_file(&nit_arm_core_driver, (CONFIGURATION_DIRECTORY + "/nit_arm_core_config.xml").c_str());
		nit_control_unit_core_config_save_to_file(&nit_control_unit_core_driver, (CONFIGURATION_DIRECTORY + "/nit_control_unit_core_config.xml").c_str());
		nit_mb_core_config_save_to_file(&nit_mb_core_driver, (CONFIGURATION_DIRECTORY + "/nit_mb_core_config.xml").c_str());
		nit_process_core_config_save_to_file(&nit_process_core_driver, (CONFIGURATION_DIRECTORY + "/nit_process_core_config.xml").c_str());
	}

	// constexpr volatile int* get_process_variables_shm_ptr() { return process_variables_shm_ptr; }

	// math::control::src_pad<double> controller0_input;
	// pwm_core_sink_pad controller0_output;

	math::control::pid_controller<double> m_controller;

private:

	int sensor_calibrate();

	void legacy_control_function(volatile int *virtual_metadata_shm, volatile int *real_metadata_shm, volatile int *proc_var_shm, volatile int *gen_core_shm, volatile int *arm_core_shm, volatile int *control_unit_shm);

	void image_writer(int socket_fd);
	void image_writer_legacy(int socket_fd);

	void command_processor_legacy(int socket_fd);
	void command_processor(int socket_fd);

	utils::signal<std::shared_ptr<application>, uint8_t *, size_t, uint8_t *, size_t> image_read;

	virtual void print_usage();

	static std::shared_ptr<application> instance;

	application_config config_default = {
		.version_major = "0",
		.version_minor = "2",
		.version_patch = "0",
		.serial_number = "0",
		.tcp_command_host_server_host = "0.0.0.0",
		.tcp_command_host_server_port = 4097,
		.tcp_image_stream_host_server_host = "0.0.0.0",
		.tcp_image_stream_host_server_port = 4096,
		.sensor_update_interval_us = 50000,
		.global_timer_update_interval_us = 1000,
	};
	application_config config;

	struct sigaction m_sigint_handler;
	struct sigaction m_pipe_handler;

	std::atomic_bool m_shutdown = false;
	std::shared_ptr<abstract_timer> m_timer;

	static std::map<uint16_t, command_processor_route> command_processor_routes_legacy;

	std::list<std::thread> m_server_threads;
	std::unordered_map<int, std::function<int(const unsigned char *, int)>> m_command_server_router;

	// volatile int *process_variables_shm_ptr = NULL;
	// volatile int *virtual_metadata_shm_ptr = NULL;

	uint8_t m_image_buffer[8192];
	uint8_t m_metadata_buffer[60];
};

class application_connection_handler
{
private:
};

#endif