#ifndef _nit_embedded_server_ctrl_dbus_hpp_
#define _nit_embedded_server_ctrl_dbus_hpp_

#include <thread>
#include <list>
#include <unordered_map>
#include <map>
#include <memory>

#include <signal.h>

#include <syslog.h>

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
#include <nit/embedded/drivers/scc_core.h>

// #include <nit/embedded/networking/tcp/server.hpp>
#include <nit/embedded/signals/event_emitter.hpp>
#include <nit/embedded/components/timer.hpp>

#include <nit/embedded/math/control.hpp>
#include <nit/embedded/networking/tcp/protocol_legacy.hpp>

#include <nit/embedded/clamir_service_config.hpp>

#include <boost/signals2.hpp>

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

class clamir_service;

struct command_processor_route
{
	using write_callback_type = std::function<void(std::shared_ptr<clamir_service>, command_processor_route &, packet &)>;
	using read_callback_type = std::function<void(std::shared_ptr<clamir_service>, command_processor_route &, packet &)>;
	void* pdata;
	read_callback_type read;
	write_callback_type write;
};

class clamir_service : std::enable_shared_from_this<clamir_service>
{
	clamir_service();

public:


#ifdef NIT_CLAMIR_HOST_MOCKUP
	static const bool host_mockup = true;
	#else
	static const bool host_mockup = false;
#endif

	virtual ~clamir_service()
	{
		syslog(LOG_INFO, __func__);
	}

	int default_handler(const unsigned char *buffer, int);
	static std::shared_ptr<clamir_service> get_instance();
	int initialize(int argc, char *argv[]);
	void run(std::atomic_bool& shutdown);

	void save_all()
	{
		std::string CONFIGURATION_DIRECTORY = ".";

		if (std::getenv("CONFIGURATION_DIRECTORY") != nullptr)
		{
			CONFIGURATION_DIRECTORY = std::string(std::getenv("CONFIGURATION_DIRECTORY"));
		}

		nit_arm_core_config_save_to_file(&nit_arm_core_driver, (CONFIGURATION_DIRECTORY + "/nit_arm_core_config.xml").c_str());
		nit_control_unit_core_config_save_to_file(&nit_control_unit_core_driver, (CONFIGURATION_DIRECTORY + "/nit_control_unit_core_config.xml").c_str());
		nit_mb_core_config_save_to_file(&nit_mb_core_driver, (CONFIGURATION_DIRECTORY + "/nit_mb_core_config.xml").c_str());
		nit_process_core_config_save_to_file(&nit_process_core_driver, (CONFIGURATION_DIRECTORY + "/nit_process_core_config.xml").c_str());

		nit_scc_core_config_update_pull(&nit_scc_core_driver);
		nit_scc_core_config_save_to_file(&nit_scc_core_driver, (CONFIGURATION_DIRECTORY + "/nit_scc_core_config.xml").c_str());

	}

	clamir_service_config& config_get() { return config; }

	int command_processor_legacy(int request);

private:

	void legacy_control_function(volatile int *virtual_metadata_shm, volatile int *real_metadata_shm, volatile int *proc_var_shm, volatile int *gen_core_shm, volatile int *arm_core_shm, volatile int *control_unit_shm);

	utils::signal<std::shared_ptr<clamir_service>, uint8_t *, size_t, uint8_t *, size_t> image_read;
	
	static std::shared_ptr<clamir_service> instance;

	clamir_service_config config_default = {
		.version_major = 6,
		.version_minor = 0,
		.version_patch = 5,
		.serial_number = "189J611",
		.tcp_command_host_server_host = "0.0.0.0",
		.tcp_command_host_server_port = 4097,
		.tcp_image_stream_host_server_host = "0.0.0.0",
		.tcp_image_stream_host_server_port = 4096,
		.sensor_update_interval_us = 50000,
		.global_timer_update_interval_us = 1000,
	};
	clamir_service_config config;

	std::atomic_bool m_shutdown = false;
	std::shared_ptr<abstract_timer> m_timer;

	static std::map<uint16_t, command_processor_route> command_processor_routes_legacy;

	std::list<std::thread> m_server_threads;
	std::unordered_map<int, std::function<int(const unsigned char *, int)>> m_command_server_router;
};

class application_connection_handler
{
private:
};

#endif