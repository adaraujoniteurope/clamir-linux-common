#ifndef _nit_embedded_clamir_service_config_hpp_
#define _nit_embedded_clamir_service_config_hpp_

#include <cinttypes>
#include <string>

#include <boost/serialization/nvp.hpp>

struct clamir_service_config
{

	std::string config_path;

	int version_major;
	int version_minor;
	int version_patch;
	std::string serial_number;

	std::string tcp_command_host_server_host;
	int tcp_command_host_server_port;

	std::string tcp_image_stream_host_server_host;
	int tcp_image_stream_host_server_port;

	int sensor_update_interval_us;
	int global_timer_update_interval_us;

	bool sensor_calibation_disable = true;
};

namespace boost::serialization
{
	template<typename archiver>
	void serialize(archiver& ar, clamir_service_config& obj, unsigned)
	{
		ar & boost::make_nvp("version_major", obj.version_major);
		ar & boost::make_nvp("version_minor", obj.version_minor);
		ar & boost::make_nvp("version_patch", obj.version_patch);
		ar & boost::make_nvp("serial_number", obj.serial_number);
		ar & boost::make_nvp("tcp_command_host_server_host", obj.tcp_command_host_server_host);
		ar & boost::make_nvp("tcp_command_host_server_port", obj.tcp_command_host_server_port);
		ar & boost::make_nvp("tcp_image_stream_host_server_host", obj.tcp_command_host_server_host);
		ar & boost::make_nvp("tcp_image_stream_host_server_port", obj.tcp_command_host_server_port);
		ar & boost::make_nvp("sensor_update_interval_ms", obj.sensor_update_interval_us);
		ar & boost::make_nvp("global_timer_update_interval_ms", obj.global_timer_update_interval_us);
		ar & boost::make_nvp("sensor_calibation_disable", obj.sensor_calibation_disable);
	}
}


#endif