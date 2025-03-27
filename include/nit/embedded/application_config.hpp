#ifndef _application_config_hpp_
#define _application_config_hpp_

#include <cinttypes>
#include <string>

// struct pid_config
// {
// 	uint16_t kp;
// 	uint16_t ki;
// 	uint16_t kd;
// 	uint16_t integral_limit;
// 	uint16_t slew_limit;
// };

// struct power_config
// {
// 	uint16_t power;
// 	uint16_t power_max;
// 	uint16_t power_min;
// 	uint16_t power_limit_max;
// 	uint16_t power_limit_min;
// };

// struct process_config
// {
// 	uint16_t mode;
// 	uint16_t automeasure_enable;
// 	uint16_t moment_track_start;
// 	uint16_t moment_track_stop;
// 	uint16_t reference_track;
// 	uint16_t reference_track_start;
// 	uint16_t track_length;
// 	uint16_t threshold;
// 	uint16_t manual_reference_width;
// 	uint16_t circular_buffer_size;
// 	uint16_t time_to_end_a_process;
// 	uint16_t external_laser_detection;
// };

// struct sensor_config
// {
// 	uint16_t pixel_mm_ratio;
// 	uint16_t black_level;
// 	uint16_t bias_voltage;
// 	uint16_t integration_time;
// };

// struct roi_config
// {
// 	uint16_t enable;
// 	uint16_t x1;
// 	uint16_t y1;
// 	uint16_t x2;
// 	uint16_t y2;
// 	uint16_t round;
// };

// struct alarm_config
// {
// 	bool enable;
// 	uint32_t max;
// 	uint32_t min;
// 	uint32_t time;
// };

// struct autoshutter_config
// {
// 	bool enable;
// 	bool enable_in_process;
// 	bool use_timer;
// 	bool use_temperature_drift;
// 	uint32_t interval;
// };

// struct preheating_config
// {
// 	bool enable;
// 	uint32_t time_us;
// 	uint32_t power_w;
// };

// struct drift_cancellation_config
// {
// 	bool enable;
// 	uint16_t cancellation_intensity_level;
// };

struct application_config
{

	std::string config_path;

	std::string version_major;
	std::string version_minor;
	std::string version_patch;
	std::string serial_number;

	std::string tcp_command_host_server_host;
	std::string tcp_command_host_server_port;

	std::string tcp_image_stream_host_server_host;
	std::string tcp_image_stream_host_server_port;

};


#endif