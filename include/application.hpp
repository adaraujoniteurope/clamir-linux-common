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

#include <signal.h>

#include "common/defs.h"
#include "drivers/arm_core.h"
#include "drivers/gen_core.h"
#include "drivers/mom_core.h"
#include "drivers/pwm_core.h"
#include "drivers/roi_core.h"
#include "drivers/control_unit_core.h"
#include "drivers/bpc_table_core.h"
#include "drivers/framebuffer_core.h"
#include "drivers/framebuffer_metadata_core.h"

#include "networking/tcp/server.hpp"
#include "signals/event_emitter.hpp"
#include "components/timer.hpp"



// #define DRIVER_CALLBACK_DESCRIPTOR_TABLE_ITEM(name, parameter, type, size, offset) \
// { \
// 	.state = (void*)&nit_##name##_driver, \
// 	.set = (driver_interface_descriptor::setter_type) nit_##name##_##parameter##_set, \
// 	.get = (driver_interface_descriptor::getter_type)nit_##name##_##parameter##_get \
// }, \

// #define DRIVER_CALLBACK_DESCRIPTOR_TABLE_ITEM(driver) \
// { \
// 	.state = (void*)&driver, \
// 	.set = (driver_interface_descriptor::setter_type) nit_##name##_##parameter##_set, \
// 	.get = (driver_interface_descriptor::getter_type)nit_##name##_##parameter##_get \
// }, \

struct driver_interface_descriptor
{
	typedef void (*setter_type)(void *state, uint32_t value);	
	typedef uint32_t(*getter_type)(void *state);

    void* state;
    setter_type set;
    getter_type get;
};

struct application_config
{

	const char *config_path;

	uint8_t version_major;
	uint8_t version_minor;
	uint8_t version_patch;
	uint8_t serial_number;

	const char *roi_core_config_path;
	const char *pwm_core_config_path;
	const char *mom_core_config_path;
	const char *gen_core_config_path;
	const char *framebuffer_core_config_path;
	const char *control_unit_core_config_path;
	const char *bpc_table_core_config_path;
	const char *arm_core_config_path;

	const char *profinet_ethernet_device_name;

	const char *tcp_command_host_server_host;
	const char *tcp_command_host_server_port;

	const char *tcp_image_stream_host_server_host;
	const char *tcp_image_stream_host_server_port;
};

class application : std::enable_shared_from_this<application>
{
	application();

public:
	int default_handler(const unsigned char *buffer, int);
	static std::shared_ptr<application> create();
	int initialize(int argc, char *argv[]);
	void run();

private:

	// void timer(std::atomic_bool& shutdown);
	void command_router(int socket_fd);
	void image_writer(int socket_fd);
	void command_processor(int socket_fd);

	virtual void print_usage();

	application_config config;
	control_unit_core_state_t& control_unit_core_state;
	arm_core_state_t& arm_core_state;
	mb_core_state_t& mb_core_state;
	framebuffer_core_state_t& framebuffer_core_state;
	framebuffer_metadata_core_state_t& framebuffer_metadata_core_state;
	bpc_table_core_state_t& bpc_core_state;

	struct sigaction m_sigint_handler;
	struct sigaction m_pipe_handler;

	std::atomic_bool m_shutdown;
	timer m_timer;

	static std::map<size_t, driver_interface_descriptor> m_command_descriptor_table;

	std::list<std::thread> m_server_threads;
	std::unordered_map<int, std::function<int(const unsigned char *, int)>> m_command_server_router;
};

class application_connection_handler
{
private:
};

#endif