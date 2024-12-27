#ifndef PROCESS_VARIABLES_H_
#define PROCESS_VARIABLES_H_

#ifdef __cplusplus
extern "C" {
#endif
// Power, MOM00, MOM01, MOM10, MOM11, MOM02, MOM20, Width, Track Nmbr, Frame Max, Frame Number, Timestamp, IO Status, T1, T2

typedef struct __attribute__((packed)) process_variables_struct
{
    int ki;
    int kp;
    int kd;
    int max_power;
    int min_power;
    int power_man;
    int power_limit_max;
    int power_limit_min;
    int set_ref_width;
    int width_ref;
    int pixel_mm_ratio;
    int pid_error;
    int end_of_process;
    int limit_integral;
    int limit_slew;
    int buff_size;
    int auto_shutter;
    int enable_alarm;
    int alarm_max;
    int alarm_min;
    int alarm_time;
    int serial_number_low;
    int serial_number_high;
    int automeasure;
    int autoshutter_config;
    int autoshutter_temp;
    int autoshutter_timer;
    int track_ref_start;
    int laser_exteral_control;
    int delay_laser_on;
    int preheating_ena;
    int preheating_time;
    int preheating_power;
} process_variables_t;

#ifdef __cplusplus
}
#endif

#endif