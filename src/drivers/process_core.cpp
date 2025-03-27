#include "common/defs.h"
#include "utils/config_file.h"
#include <utils/waitable.hpp>

#include <drivers/arm_core.h>
#include <drivers/mb_core.h>
#include <drivers/control_unit_core.h>
#include <drivers/bpc_table_core.h>
#include <drivers/framebuffer_core.h>
#include <drivers/process_core.h>

#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <math.h>
#include <string.h>

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define KI 0
#define KP 1
#define KD 2
#define MAX_POWER 3
#define MIN_POWER 4
#define POWER_MAN 5
#define POWER_LIMIT_MAX 6
#define POWER_LIMIT_MIN 7
#define SET_REF_WIDTH 8
#define WIDTH_REF 9
#define PIXEL_MM_RATIO 10 // llegan en micras, pasar a mm
#define PID_ERROR 11
#define END_OF_PROCESS 12 // imagenes sin se�al para considerar un proceso finalizado
#define LIMIT_INTEGRAL 13
#define LIMIT_SLEW 14
#define BUFF_SIZE 15
#define AUTO_SHUTTER 16
#define ENABLE_ALARM 17
#define ALARM_MAX 18
#define ALARM_MIN 19
#define ALARM_TIME 20
#define SERIAL_NUMBER_LOW 21
#define SERIAL_NUMBER_HIGH 22
#define AUTOMEASURE 23
#define AUTOSHUTTER_CONFIG 24
#define AUTOSHUTTER_TEMP 25
#define AUTOSHUTTER_TIMER 26
#define TRACK_REF_START 27
#define LASER_EXTERAL_CONTROL 28
#define DELAY_LASER_ON 29
#define PREHEATING_ENA 30
#define PREHEATING_TIME 31
#define PREHEATING_POWER 32

#define MANUAL 0
#define IDLE 8
#define MIDIENDO 9
#define CONTROL 10
#define PREHEATING 11

#define THRESHOLD 0x4000 + 0
#define INTENSITY_MIN 0x4000 + 1
#define INTENSITY_MAX 0x4000 + 2
#define MOM_00 0x4000 + 3
#define MOM_01 0x4000 + 4
#define MOM_10 0x4000 + 5
#define MOM_11 0x4000 + 6
#define MOM_02 0x4000 + 7
#define MOM_20 0x4000 + 8
#define START_TRACK_MOM_T 0x4000 + 9
#define TRACK_NBR 0x4000 + 10
#define MODE 0x4000 + 11
#define TIME_TRACK_LOW 0x4000 + 12
#define TIME_TRACK_HIGH 0x4000 + 13
#define FRAME_MAX 0x4000 + 14
#define REFERENCE_TRACK 0x4000 + 15
#define END_OF_TRACK 0x4000 + 16

#define VERSION_ID 0x0000 + 0
#define ENABLE_MOM 0x0000 + 1
#define ENABLE_ROI 0x0000 + 2
#define ENABLE_PWM 0x0000 + 3
#define ENABLE_IRQ 0x0000 + 4
#define CHANGE_OP_MODE 0x0000 + 5
#define IRQ_MICROBLAZE 0x0000 + 6
#define DIGITAL_IN_0 0x0000 + 7
#define DIGITAL_IN_1 0x0000 + 8
#define DIGITAL_OUT_0 0x0000 + 9
#define DIGITAL_OUT_1 0x0000 + 10
#define DIGITAL_OUT_CONF 0x0000 + 11
#define DIGITAL_IN_2 0x0000 + 12
#define DIGITAL_IN_3 0x0000 + 13
#define DIGITAL_OUT_2 0x0000 + 14
#define DIGITAL_OUT_3 0x0000 + 15

#define LMAX_KI 30000
#define LMAX_KP 30000
#define LMAX_KD 30000
#define LMAX_MAX_POWER 32000
#define LMAX_MIN_POWER 30000
#define LMAX_START_TRACK_MOM 2000
#define LMAX_POWER 32000
#define LMAX_STOP_TRACK_MOM 1000
#define LMAX_MODE 2
#define LMAX_TRACK_REFERENCE 100
#define LMAX_TRACK_LENGTH 10000
#define LMAX_WIDTH_MANUAL 65
#define LMAX_THRESHOLD 5000
#define LMAX_ROI_ROUND 3
#define LMAX_X2_PIXEL 63
#define LMAX_X2_PIXEL 63
#define LMAX_X1_PIXEL 62
#define LMAX_Y2_PIXEL 63
#define LMAX_Y1_PIXEL 62
#define LMAX_MAX_POWER_LIMIT 32000
#define LMAX_MIN_POWER_LIMIT 30000
#define LMAX_END_OF_PROCESS 30000
#define LMAX_PIXEL_MM_RATIO 10
#define LMAX_LIMIT_SLEW 300
#define LMAX_BLACK_LEVEL 10000
#define LMAX_ALARM_MAX 320
#define LMAX_ALARM_MIN 320
#define LMAX_ALARM_TIME 10000
#define LMAX_LIMIT_INTEGRAL 10000
#define LMAX_CIRCULAR_BUFFER_SIZE 512
#define LMAX_INTEGRATION_TIME 800
#define LMAX_CONF_DIGITALIO 2
#define LMAX_TRACK_REF_START 100
#define LMAX_DRIFT_TEMP_AUTOSHUTTER 50
#define LMAX_TIMER_AUTOSHUTTER 320000
#define LMAX_BIAS_VOLTAGE 2.5
#define LMAX_DELAY_LASER_ON 1000
#define LMAX_PREHEATING_TIME 30000
#define LMAX_PREHEATING_POWER 32000
#define LMAX_DRIFT_INTENSITY 15

#define PWM_LIMIT_MAX 0xC000 + 0
#define PWM 0xC000 + 1
#define POWER 0xC000 + 2
#define PWM_LIMIT_MIN 0xC000 + 3

#define WAIT_START_CALIBRATION 150
#define WAIT_STOP_CALIBRATION 100 // 30
#define WAIT_SECOND_APERTURE 400
#define NAP_DURATION 750 // 300

#define LED_R 0x4000 + 0
#define LED_B 0x4000 + 1
#define LED_G 0x4000 + 2

#define NIT_INT_TIME 0            // 0
#define NIT_BIAS_V 1              // 4
#define NIT_OFFSET_EN 2           // 8
#define NIT_OFFSET_UPDATE 3       // 12
#define NIT_SHUTTER 4             // 16
#define NIT_BPC_EN 5              // 20
#define NIT_BPC_MEM_WRITE 6       // 24
#define NIT_BPC_IDENTIFY 7        // 28
#define NIT_TEMP1 8               // 32
#define NIT_TEMP2 9               // 36
#define NIT_TEMP3 10              // 40
#define NIT_TEMP4 11              // 44
#define NIT_SHUTTER_RESET 12      // 48
#define NIT_SINCRONIZATION 13     // 52
#define NIT_SAVE_EMBEDDED_CONF 14 // 56
#define NIT_ARM_SW_VERSION 15     // 60
#define NIT_DRIFT_ENABLE 16       // 64
#define NIT_DRIFT_POSITION 17     // 68
#define NIT_DRIFT_LEVEL 18        // 72
#define NIT_FPGA_VERSION 19       // 76
#define NIT_9 20                  // 80
#define NIT_10 21                 // 84
#define NIT_11 22                 // 88
#define NIT_12 23                 // 92
#define NIT_13 24                 // 96
#define NIT_14 25                 // 100
#define NIT_15 26                 // 104
#define NIT_16 27                 // 108
#define NIT_17 28                 // 112
#define NIT_18 29                 // 116
#define NIT_19 30                 // 120
#define NIT_20 31                 // 124
#define NIT_TRIGGER_USEC 35       // 140
#define NIT_BLACK_LEVEL 36        // 144
#define NIT_21 37                 // 152

#define BRAM_IMG_METADATOS 0x42002000

#define SEM_NAME "semaforo"

struct nit_process_core_private_state
{
    int estadoAutomata = 0;
    int valor1 = 0;
    int valor2 = 0;
    int contadorFramesMidiendo = 0;

    pthread_mutex_t lock;

    // Variables calculo potencia
    double width_ref = 1;
    double error_t0 = 0;
    double error_t1 = 0;
    double potencia_t0 = 0;
    double potencia_t1 = 0;
    double dt = 0.001;
    double potenciaMax, potenciaMin, limPotenciaMax, limPotenciaMin;
    double ki, kp, kd;
    double duty;
    double pixel_mm_ratio;
    double integral = 0;
    double derivative = 0, limiteIntegral, limit_integral_divided, limiteSlew;
    int mode = 0;
    int laser_status = 0; // 0 OFF, 1 ON
    int contadorFramesStop = 0;
    int endP = 1000;
    double delta_potencia = 0;
    uint64_t track_cnt = 0;
    int track = 0;
    uint64_t track_length = 2000;

    int stop_logging;

    sem_t* semaforo;

    // File descriptor de UIO
    int pending = 0;
    int enable = 1;

    // Habilita interrupciones

    // int fd_int = open("/dev/uio0", O_RDWR);
    // if (fd_int < 0)
    // 	printf("No se puede abrir el descriptor de uio0 para copntrol\n");

    // int fdMetadatos;
    // fdMetadatos = open("/dev/mem", O_RDWR | O_SYNC);
    // volatile int *metaPtr = real_metadata_shm;
    // metaPtr = (volatile int *)mmap(NULL, 64, PROT_READ | PROT_WRITE, MAP_SHARED, fdMetadatos, BRAM_IMG_METADATOS);

    int metadatos[12];
    double W = 0;
    double width_aux = 0;
    double W_circ_buff_aux = 0;
    double w_circ_buffer[512];

    int control_NAP = 0;
    int measurement_NAP = 0;
    int cont_calibration = 0;
    uint32_t alarm_enable; // 1 alarm ON, 0 alarm OFF
    double alarm_min;
    double alarm_max;
    uint16_t cnt_aux_alarm;
    uint16_t alarm;
    uint16_t automeasure_flag, autoshutter_config;
    uint32_t autoshutter_time_target;
    float autoshutter_temp_target;
    int resultado = 0;
    int cont = 0;
    int z = 0;
    int circ_buffer_size = 1;

    char auto_auto_shutter_enable;
    char auto_auto_shutter_enable_inprocess;
    char auto_auto_shutter_time_xtemp;
    float current_temperature;
    float temperature_last_autoshutter = ((float)virtual_metadata_shm[13]) / 10;
    uint32_t cnt_last_autoshutter = 0;
    char pending_autoshutter = 0;
    uint16_t laser_external = 0;
    int last_laser_status = 0;
    int delay_laser_on = 0;
    int cont_preheating = 0;

    volatile int* proc_var_shm;
    volatile int* virtual_metadata_shm;
    volatile int* arm_core_shm;
    volatile int* real_metadata_shm;
    volatile int* control_unit_shm;
    volatile int* mb_core_shm;
};

nit_process_core_state_t nit_process_core_driver;

double nit_process_core_calculate_width(int metadatos[12]);
nit_process_core_private_state* nit_process_core_process_data_ptr_get(nit_process_core_state_t* state);

int nit_process_core_open(nit_process_core_state_t* state, nit_arm_core_state_t* nit_arm_core_state, nit_control_unit_core_state_t* nit_control_unit_core_state, nit_mb_core_state_t* nit_mb_core_state, nit_framebuffer_core_state_t* nit_framebuffer_core_state)
{
    if (state == NULL)
    {
        return -1;
    }

    if (state->is_open == true)
    {
        return 0;
    }

    state->is_open = false;

    state->fd = -1;

    if (!NIT_CLAMIR_HOST_MOCKUP) {

        state->fd = open("/dev/mem", O_RDWR | O_SYNC);
        if (state->fd < 0)
        {
            return -2;
        }

    }

    state->priv = (void*)malloc(sizeof(nit_process_core_private_state));
    memset((void*)state->priv, 0, sizeof(nit_process_core_private_state));

    if (state->priv == NULL)
    {
        return -3;
    }

    auto process = (nit_process_core_private_state*)state->priv;

    process->semaforo = sem_open(SEM_NAME, O_CREAT, 0644, 0);

    process->proc_var_shm = (volatile int*)mmap(NULL, 512, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    process->virtual_metadata_shm = (volatile int*)mmap(NULL, 256, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (process->virtual_metadata_shm == nullptr)
    {
        return -5;
    }

    if (nit_arm_core_assert(nit_arm_core_state) < 0)
    {
        return -6;
    }

    process->arm_core_shm = (volatile int*)nit_arm_core_state->priv;

    if (nit_control_unit_core_assert(nit_control_unit_core_state) < 0)
    {
        return -7;
    }

    process->control_unit_shm = (volatile int*)nit_control_unit_core_state->priv;

    if (nit_mb_core_state_assert(nit_mb_core_state) < 0)
    {
        return -8;
    }

    process->mb_core_shm = (volatile int*)nit_mb_core_state->priv;

    if (nit_framebuffer_core_state_assert(nit_framebuffer_core_state) < 0)
    {
        return -9;
    }

    process->real_metadata_shm = (volatile int*)nit_framebuffer_core_metadata(nit_framebuffer_core_state);

    if (state->priv == NULL)
    {
        close(state->fd);
        return -10;
    }

    state->is_open = true;

    return 0;
}

int nit_process_core_close(nit_process_core_state_t* state)
{
    if (state == NULL)
    {
        return -1;
    }

    if (state->is_open == false)
    {
        return 0;
    }

    state->is_open = false;

    if (state->priv != NULL)
    {
        munmap((void*)state->priv, NIT_PROCESS_CORE_SIZE);
        state->priv = NULL;
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }

    return 0;
}

int nit_process_core_assert(nit_process_core_state_t* state)
{

    if (state == NULL)
    {
        return -1;
    }

    if (state->priv == NULL)
    {
        return -2;
    }

    if (state->is_open == false)
    {
        return -3;
    }

    return 0;
}

volatile int* nit_process_core_get_virtual_metadata_shm_ptr(nit_process_core_state_t* state)
{
    return nit_process_core_process_data_ptr_get(state)->virtual_metadata_shm;
}

nit_process_core_private_state* nit_process_core_process_data_ptr_get(nit_process_core_state_t* state)
{
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return nullptr;
    }

    return (nit_process_core_private_state*)state->priv;
}

int nit_process_core_config_save_to_file(nit_process_core_state_t* state, const char* path)
{

    if (nit_process_core_ki_get(state, &state->config.ki) < 0)
    {
        return -1;
    }
    if (nit_process_core_kp_get(state, &state->config.kp) < 0)
    {
        return -2;
    }
    if (nit_process_core_kd_get(state, &state->config.kd) < 0)
    {
        return -3;
    }
    if (nit_process_core_max_power_get(state, &state->config.max_power) < 0)
    {
        return -4;
    }
    if (nit_process_core_min_power_get(state, &state->config.min_power) < 0)
    {
        return -5;
    }
    if (nit_process_core_power_man_get(state, &state->config.power_man) < 0)
    {
        return -6;
    }
    if (nit_process_core_power_limit_max_get(state, &state->config.power_limit_max) < 0)
    {
        return -7;
    }
    if (nit_process_core_power_limit_min_get(state, &state->config.power_limit_min) < 0)
    {
        return -8;
    }
    if (nit_process_core_set_ref_width_get(state, &state->config.set_ref_width) < 0)
    {
        return -9;
    }
    if (nit_process_core_width_ref_get(state, &state->config.width_ref) < 0)
    {
        return -10;
    }
    if (nit_process_core_pixel_mm_ratio_get(state, &state->config.pixel_mm_ratio) < 0)
    {
        return -11;
    }
    if (nit_process_core_pid_error_get(state, &state->config.pid_error) < 0)
    {
        return -12;
    }
    if (nit_process_core_end_of_process_get(state, &state->config.end_of_process) < 0)
    {
        return -13;
    }
    if (nit_process_core_limit_integral_get(state, &state->config.limit_integral) < 0)
    {
        return -14;
    }
    if (nit_process_core_limit_slew_get(state, &state->config.limit_slew) < 0)
    {
        return -15;
    }
    if (nit_process_core_buff_size_get(state, &state->config.buff_size) < 0)
    {
        return -16;
    }
    if (nit_process_core_auto_shutter_get(state, &state->config.auto_shutter) < 0)
    {
        return -17;
    }
    if (nit_process_core_enable_alarm_get(state, &state->config.enable_alarm) < 0)
    {
        return -18;
    }
    if (nit_process_core_alarm_max_get(state, &state->config.alarm_max) < 0)
    {
        return -19;
    }
    if (nit_process_core_alarm_min_get(state, &state->config.alarm_min) < 0)
    {
        return -20;
    }
    if (nit_process_core_alarm_time_get(state, &state->config.alarm_time) < 0)
    {
        return -21;
    }
    if (nit_process_core_serial_number_low_get(state, &state->config.serial_number_low) < 0)
    {
        return -22;
    }
    if (nit_process_core_serial_number_high_get(state, &state->config.serial_number_high) < 0)
    {
        return -23;
    }
    if (nit_process_core_automeasure_get(state, &state->config.automeasure) < 0)
    {
        return -24;
    }
    if (nit_process_core_autoshutter_config_get(state, &state->config.autoshutter_config) < 0)
    {
        return -25;
    }
    if (nit_process_core_autoshutter_temp_get(state, &state->config.autoshutter_temp) < 0)
    {
        return -26;
    }
    if (nit_process_core_autoshutter_timer_get(state, &state->config.autoshutter_timer) < 0)
    {
        return -27;
    }
    if (nit_process_core_track_ref_start_get(state, &state->config.track_ref_start) < 0)
    {
        return -28;
    }
    if (nit_process_core_laser_external_control_get(state, &state->config.laser_external_control) < 0)
    {
        return -29;
    }
    if (nit_process_core_delay_laser_on_get(state, &state->config.delay_laser_on) < 0)
    {
        return -30;
    }
    if (nit_process_core_preheating_ena_get(state, &state->config.preheating_ena) < 0)
    {
        return -31;
    }
    if (nit_process_core_preheating_time_get(state, &state->config.preheating_time) < 0)
    {
        return -32;
    }
    if (nit_process_core_preheating_power_get(state, &state->config.preheating_power) < 0)
    {
        return -33;
    }

    return config_file_save_to_file(state, path);
}

int nit_process_core_config_load_from_file(nit_process_core_state_t* state, const char* path)
{

    if (config_file_load_from_file(state, path) < 0)
    {
        return -1;
    }

    if (nit_process_core_ki_set(state, state->config.ki) < 0)
    {
        return -1;
    }
    if (nit_process_core_kp_set(state, state->config.kp) < 0)
    {
        return -2;
    }
    if (nit_process_core_kd_set(state, state->config.kd) < 0)
    {
        return -3;
    }
    if (nit_process_core_max_power_set(state, state->config.max_power) < 0)
    {
        return -4;
    }
    if (nit_process_core_min_power_set(state, state->config.min_power) < 0)
    {
        return -5;
    }
    if (nit_process_core_power_man_set(state, state->config.power_man) < 0)
    {
        return -6;
    }
    if (nit_process_core_power_limit_max_set(state, state->config.power_limit_max) < 0)
    {
        return -7;
    }
    if (nit_process_core_power_limit_min_set(state, state->config.power_limit_min) < 0)
    {
        return -8;
    }
    if (nit_process_core_set_ref_width_set(state, state->config.set_ref_width) < 0)
    {
        return -9;
    }
    if (nit_process_core_width_ref_set(state, state->config.width_ref) < 0)
    {
        return -10;
    }
    if (nit_process_core_pixel_mm_ratio_set(state, state->config.pixel_mm_ratio) < 0)
    {
        return -11;
    }
    if (nit_process_core_pid_error_set(state, state->config.pid_error) < 0)
    {
        return -12;
    }
    if (nit_process_core_end_of_process_set(state, state->config.end_of_process) < 0)
    {
        return -13;
    }
    if (nit_process_core_limit_integral_set(state, state->config.limit_integral) < 0)
    {
        return -14;
    }
    if (nit_process_core_limit_slew_set(state, state->config.limit_slew) < 0)
    {
        return -15;
    }
    if (nit_process_core_buff_size_set(state, state->config.buff_size) < 0)
    {
        return -16;
    }
    if (nit_process_core_auto_shutter_set(state, state->config.auto_shutter) < 0)
    {
        return -17;
    }
    if (nit_process_core_enable_alarm_set(state, state->config.enable_alarm) < 0)
    {
        return -18;
    }
    if (nit_process_core_alarm_max_set(state, state->config.alarm_max) < 0)
    {
        return -19;
    }
    if (nit_process_core_alarm_min_set(state, state->config.alarm_min) < 0)
    {
        return -20;
    }
    if (nit_process_core_alarm_time_set(state, state->config.alarm_time) < 0)
    {
        return -21;
    }
    if (nit_process_core_serial_number_low_set(state, state->config.serial_number_low) < 0)
    {
        return -22;
    }
    if (nit_process_core_serial_number_high_set(state, state->config.serial_number_high) < 0)
    {
        return -23;
    }
    if (nit_process_core_automeasure_set(state, state->config.automeasure) < 0)
    {
        return -24;
    }
    if (nit_process_core_autoshutter_config_set(state, state->config.autoshutter_config) < 0)
    {
        return -25;
    }
    if (nit_process_core_autoshutter_temp_set(state, state->config.autoshutter_temp) < 0)
    {
        return -26;
    }
    if (nit_process_core_autoshutter_timer_set(state, state->config.autoshutter_timer) < 0)
    {
        return -27;
    }
    if (nit_process_core_track_ref_start_set(state, state->config.track_ref_start) < 0)
    {
        return -28;
    }
    if (nit_process_core_laser_external_control_set(state, state->config.laser_external_control) < 0)
    {
        return -29;
    }
    if (nit_process_core_delay_laser_on_set(state, state->config.delay_laser_on) < 0)
    {
        return -30;
    }
    if (nit_process_core_preheating_ena_set(state, state->config.preheating_ena) < 0)
    {
        return -31;
    }
    if (nit_process_core_preheating_time_set(state, state->config.preheating_time) < 0)
    {
        return -32;
    }
    if (nit_process_core_preheating_power_set(state, state->config.preheating_power) < 0)
    {
        return -33;
    }

    return 0;
}

int nit_process_core_run(nit_process_core_state_t* state, std::shared_ptr<utils::waitable> timer, std::atomic_bool& shutdown)
{
    auto priv = nit_process_core_process_data_ptr_get(state);

    priv->estadoAutomata = 0;
    priv->valor1 = 0;
    priv->valor2 = 0;
    priv->contadorFramesMidiendo = 0;

    // Variables calculo potencia
    priv->width_ref = 1;
    priv->error_t0 = 0;
    priv->error_t1 = 0;
    priv->potencia_t0 = 0;
    priv->potencia_t1 = 0;
    priv->dt = 0.001;
    priv->integral = 0;
    priv->mode = 0;
    priv->laser_status = 0; // 0 OFF, 1 ON
    priv->contadorFramesStop = 0;
    priv->endP = 1000;
    priv->delta_potencia = 0;
    priv->track_cnt = 0;
    priv->track = 0;
    priv->track_length = 2000;

    priv->semaforo = sem_open(SEM_NAME, O_CREAT, 0644, 0);

    priv->W = 0;
    priv->width_aux = 0;
    priv->W_circ_buff_aux = 0;

    for (int i = 0; i < 512; i++)
    {
        priv->w_circ_buffer[i] = 0;
    }
    priv->control_NAP = 0;
    priv->measurement_NAP = 0;
    priv->cont_calibration = 0;

    priv->cnt_aux_alarm = priv->proc_var_shm[ALARM_TIME];

    priv->resultado = 0;
    priv->cont = 0;
    priv->z = 0;
    priv->circ_buffer_size = 1;
    priv->derivative = 0;

    priv->temperature_last_autoshutter = ((float)priv->virtual_metadata_shm[13]) / 10;
    priv->cnt_last_autoshutter = 0;
    priv->pending_autoshutter = 0;
    priv->laser_external = 0;
    priv->last_laser_status = 0;
    priv->delay_laser_on = 0;
    priv->cont_preheating = 0;
    while (!shutdown)
    {
        priv->ki = priv->proc_var_shm[KI];
        priv->kp = priv->proc_var_shm[KP];
        priv->kd = priv->proc_var_shm[KD];
        priv->potenciaMax = (double)priv->proc_var_shm[MAX_POWER];
        priv->potenciaMin = (double)priv->proc_var_shm[MIN_POWER];

        if (priv->potenciaMin > 32767)
        {
            priv->potenciaMin = priv->potenciaMin - 65536;
        }

        priv->limPotenciaMax = priv->proc_var_shm[POWER_LIMIT_MAX];
        priv->limPotenciaMin = priv->proc_var_shm[POWER_LIMIT_MIN];

        priv->pixel_mm_ratio = ((double)priv->proc_var_shm[PIXEL_MM_RATIO] / 1000); // se reciben en micras
        priv->proc_var_shm[PID_ERROR] = (int)100 * priv->error_t1;
        priv->limiteIntegral = (double)priv->proc_var_shm[LIMIT_INTEGRAL];
        priv->limiteSlew = ((double)priv->proc_var_shm[LIMIT_SLEW] / 100);
        priv->circ_buffer_size = priv->proc_var_shm[BUFF_SIZE];

        priv->alarm_enable = priv->proc_var_shm[ENABLE_ALARM];
        priv->alarm_max = ((double)priv->proc_var_shm[ALARM_MAX]) / 100;
        priv->alarm_min = ((double)priv->proc_var_shm[ALARM_MIN]) / 100;
        priv->automeasure_flag = priv->proc_var_shm[AUTOMEASURE];

        priv->autoshutter_config = priv->proc_var_shm[AUTOSHUTTER_CONFIG]; // booleano, comprobar flags
        // decode_config
        priv->auto_auto_shutter_enable = priv->autoshutter_config & 0x01;
        priv->auto_auto_shutter_enable_inprocess = (priv->autoshutter_config >> 1) & 0x01;
        priv->auto_auto_shutter_time_xtemp = (priv->autoshutter_config >> 3) & 0x01;
        // decodee config end
        priv->autoshutter_temp_target = ((float)priv->proc_var_shm[AUTOSHUTTER_TEMP]) / 10; // de int16 recibido a double o float, dividir entre 10
        priv->autoshutter_time_target = priv->proc_var_shm[AUTOSHUTTER_TIMER] * 10000;      //	de int16 recibido a int32, multiplicar por 10

        priv->laser_external = priv->proc_var_shm[LASER_EXTERAL_CONTROL];

        if (priv->circ_buffer_size < 1)
        {
            priv->circ_buffer_size = 1;
        }
        else if (priv->circ_buffer_size > LMAX_CIRCULAR_BUFFER_SIZE)
        {
            priv->circ_buffer_size = LMAX_CIRCULAR_BUFFER_SIZE;
        }
        if (priv->mode != priv->mb_core_shm[MODE])
        {
            priv->mode = priv->mb_core_shm[MODE];
            if (priv->mode == 2)
            {
                priv->estadoAutomata = MANUAL;
                printf("\nCambio a estado MANUAL\n");
                // if (logging == 1)
                // {
                // 	stop_logging = 1;
                // 	cont = 0;
                // 	pthread_mutex_unlock(&lock);
                // }
            }
            else
            {
                priv->estadoAutomata = IDLE;
                printf("\nCambio a estado IDLE\n");
                priv->mb_core_shm[CHANGE_OP_MODE] = 1;
            }
        }

        timer->wait();
        // read(fd_int, (int *)&pending, sizeof(int)); // Se bloquea hasta que sucede una interrupcion de uio0
        memcpy(priv->metadatos, (void*)priv->real_metadata_shm, 48);

        // Laser status
        if (priv->laser_external)
        {
            priv->laser_status = priv->mb_core_shm[DIGITAL_IN_0];
            // laser_status = mb_core_shm[DIGITAL_IN_1];
        }
        else
        {
            if (priv->laser_status)
            {
                if (priv->metadatos[1] < priv->mb_core_shm[END_OF_TRACK])
                    priv->laser_status = 0;
            }
            else
            {
                if (priv->metadatos[1] >= priv->mb_core_shm[START_TRACK_MOM_T])
                    priv->laser_status = 1;
            }
        }

        // write(fd_int, (void *)&enable, sizeof(int)); // Habilita la interrupcion de uio0

        /*
         * Cambio a lectura buffer circular
         */
        if (priv->proc_var_shm[AUTO_SHUTTER])
        {

            priv->proc_var_shm[AUTO_SHUTTER] = 0;
            if (!priv->measurement_NAP)
            {
                priv->measurement_NAP = NAP_DURATION;
                priv->control_NAP = NAP_DURATION;
                // proc_var_shm[AUTO_SHUTTER] = 0;
                priv->cont_calibration = WAIT_START_CALIBRATION + 1 + WAIT_STOP_CALIBRATION + WAIT_SECOND_APERTURE;
                // if(control_unit_shm[NIT_SHUTTER] == 0)
                //{
                priv->control_unit_shm[NIT_SHUTTER] = 1; // cerrar shutter
                //}
                priv->cnt_last_autoshutter = 0;
                priv->temperature_last_autoshutter = priv->current_temperature;
            }
            // control_unit_shm[NIT_SHUTTER_RESET]=1;
        }
        if (!priv->measurement_NAP)
        {
            priv->mb_core_shm[DIGITAL_OUT_3] = 0;
            if (priv->delay_laser_on > 0)
            {
                priv->delay_laser_on--;
            }
            priv->z += 1;
            if (priv->z >= priv->circ_buffer_size)
            {
                priv->z = 0;
            }
            priv->w_circ_buffer[priv->z] = priv->pixel_mm_ratio * nit_process_core_calculate_width(priv->metadatos);
            priv->W_circ_buff_aux = 0;
            for (int y = 0; y < priv->circ_buffer_size; y++)
            {
                priv->W_circ_buff_aux += priv->w_circ_buffer[y];
            }
            priv->W = priv->W_circ_buff_aux / (priv->circ_buffer_size);
        }
        else
        { // siesta
            priv->mb_core_shm[DIGITAL_OUT_3] = 1;
            priv->W = priv->W;
            if ((priv->cont_calibration <= 0) && (priv->control_unit_shm[NIT_SHUTTER] != 0))
            {
                priv->control_unit_shm[NIT_SHUTTER] = 0; // open shutter
            }
            else
            {
                if (priv->cont_calibration == (WAIT_SECOND_APERTURE))
                {
                    //	control_unit_shm[NIT_SHUTTER] = 0;
                }
                if (priv->cont_calibration == (1 + WAIT_STOP_CALIBRATION + WAIT_SECOND_APERTURE))
                {
                    priv->control_unit_shm[NIT_OFFSET_UPDATE] = 1;
                    // control_unit_shm[NIT_SHUTTER_RESET]=1;
                }
                // cont_calibration--;
            }
            priv->cont_calibration--;
            priv->measurement_NAP--;
            priv->control_NAP--;
        }

        if (priv->proc_var_shm[SET_REF_WIDTH] > 0)
        {
            priv->width_ref = ((double)priv->proc_var_shm[WIDTH_REF]) / 100;
            priv->proc_var_shm[SET_REF_WIDTH] = 0;
        }

        switch (priv->estadoAutomata)
        {
        case MANUAL:
            priv->potencia_t0 = priv->proc_var_shm[POWER_MAN];
            if (priv->mode != 2)
            {
                priv->estadoAutomata = IDLE;
                printf("\nCambio a estado IDLE\n");
                priv->mb_core_shm[CHANGE_OP_MODE] = 1;
            }
            priv->track_cnt = 0;
            priv->track = 0;
            priv->delay_laser_on = 0;
            break;

        case IDLE:
            priv->mb_core_shm[CHANGE_OP_MODE] = 1;
            priv->width_aux = 0;
            priv->contadorFramesStop = 0;
            if (priv->laser_status)
            {
                if (priv->proc_var_shm[PREHEATING_ENA])
                {
                    priv->cont_preheating = 0;
                    priv->estadoAutomata = PREHEATING;
                    printf("\nCambio a estado PREHEATING\n");
                }
                else
                {
                    priv->estadoAutomata = MIDIENDO;
                    printf("\nCambio a estado MIDIENDO\n");
                    priv->contadorFramesMidiendo = 0;
                    // pthread_mutex_unlock(&lock);
                }
                priv->track_length = ((unsigned int)priv->mb_core_shm[TIME_TRACK_HIGH]);
                priv->track_length = (((priv->track_length << 32) | ((unsigned int)priv->mb_core_shm[TIME_TRACK_LOW])) * 1000) / CLK_100MHZ;
            }
            else
            {
                priv->potencia_t0 = priv->proc_var_shm[POWER_MAN];
                priv->potencia_t1 = priv->potencia_t0;
            }
            priv->track_cnt = 0;
            priv->track = 0;
            priv->delay_laser_on = 0;
            break;

        case PREHEATING:
            priv->potencia_t0 = priv->proc_var_shm[PREHEATING_POWER];
            if (priv->cont_preheating++ >= priv->proc_var_shm[PREHEATING_TIME])
            {
                priv->estadoAutomata = MIDIENDO;
                priv->contadorFramesMidiendo = 0;
                // pthread_mutex_unlock(&lock);
                printf("\nCambio a estado MIDIENDO\n");
            }
            break;

        case MIDIENDO:
            priv->cont++;
            if (priv->mode == 0)
            {
                if (priv->track_cnt >= priv->track_length)
                {
                    priv->track_cnt = 0;
                    priv->track++;
                }
                else
                {
                    priv->track_cnt++;
                }
            }
            else if (priv->mode == 1)
            {
                if ((priv->laser_status > 0) && (priv->laser_status != priv->last_laser_status))
                {
                    if (priv->laser_external)
                    {
                        priv->track++;
                        priv->delay_laser_on = priv->proc_var_shm[DELAY_LASER_ON];
                    }
                    else
                    {
                        if (!priv->measurement_NAP)
                        {
                            priv->track++;
                            priv->delay_laser_on = priv->proc_var_shm[DELAY_LASER_ON];
                        }
                    }
                }
            }
            // if ((laser_status) & (metadatos[7] >= proc_var_shm[TRACK_REF_START])) //sustituido por cuenta de tracks en el ARM
            if ((priv->laser_status) & (priv->track >= priv->proc_var_shm[TRACK_REF_START]))
            {
                priv->width_aux = priv->width_aux + priv->W;
                priv->contadorFramesMidiendo++;
            }
            priv->potencia_t0 = priv->proc_var_shm[POWER_MAN];
            priv->potencia_t1 = priv->potencia_t0;
            // if (metadatos[7] >= mb_core_shm[REFERENCE_TRACK]){
            if (priv->track >= priv->mb_core_shm[REFERENCE_TRACK])
            {
                priv->estadoAutomata = CONTROL;
                printf("\nCambio a estado CONTROL\n");
                if (priv->contadorFramesMidiendo == 0)
                {
                    priv->contadorFramesMidiendo = 1;
                }
                if (priv->automeasure_flag == 1)
                {
                    priv->width_ref = priv->width_aux / priv->contadorFramesMidiendo;
                }
                else
                {
                    priv->width_ref = ((double)priv->proc_var_shm[WIDTH_REF]) / 100;
                }
                priv->width_aux = 0;
                priv->error_t0 = 0;
                priv->error_t1 = 0;
                priv->integral = 0;
                priv->derivative = 0;
                priv->contadorFramesMidiendo = 0;
            }
            break;

        case CONTROL:
            priv->endP = priv->proc_var_shm[END_OF_PROCESS];
            priv->cont++;

            if (priv->mode == 0)
            {
                if (priv->track_cnt >= priv->track_length)
                {
                    priv->track_cnt = 0;
                    priv->track++;
                }
                else
                {
                    priv->track_cnt++;
                }
            }
            else if (priv->mode == 1)
            {
                if ((priv->laser_status > 0) && (priv->laser_status != priv->last_laser_status))
                {
                    if (priv->laser_external)
                    {
                        priv->track++;
                        priv->delay_laser_on = priv->proc_var_shm[DELAY_LASER_ON];
                    }
                    else
                    {
                        if (!priv->measurement_NAP)
                        {
                            priv->track++;
                            priv->delay_laser_on = priv->proc_var_shm[DELAY_LASER_ON];
                        }
                    }
                }
            }
            if (priv->delay_laser_on > 0)
            {
                priv->delay_laser_on--;
            }
            else
            {
                if ((!priv->laser_status) || (priv->control_NAP))
                {
                    if (priv->contadorFramesStop++ > priv->endP)
                    {
                        priv->estadoAutomata = IDLE;
                        printf("\nCambio a estado IDLE\n");
                        priv->contadorFramesStop = 0;
                        priv->cont = 0;
                    }
                }
                else
                {
                    priv->contadorFramesStop = 0;
                    // Comienza el c�clculo de la potencia
                    priv->error_t0 = priv->width_ref - priv->W;
                    priv->integral = priv->integral + (priv->error_t0 * priv->dt);

                    if (priv->ki > 0)
                    {
                        priv->limit_integral_divided = priv->limiteIntegral / priv->ki;
                    }
                    else
                    {
                        priv->limit_integral_divided = 1000;
                    }
                    if (fabs(priv->integral) >= (priv->limit_integral_divided))
                    {
                        if (priv->integral < 0)
                        {
                            priv->integral = 0 - priv->limit_integral_divided;
                        }
                        else
                        {
                            priv->integral = priv->limit_integral_divided;
                        }
                    }

                    //				derivative = (error_t0 - error_t1)/dt; //comentada por posibilidad de valor muy grande
                    priv->derivative = (priv->error_t0 - priv->error_t1);
                    priv->potencia_t0 = priv->proc_var_shm[POWER_MAN] + (priv->kp * priv->error_t0) + (priv->ki * priv->integral) + (priv->kd * priv->derivative);
                    priv->error_t1 = priv->error_t0;

                    priv->delta_potencia = priv->potencia_t1 - priv->potencia_t0;
                    if (abs(priv->delta_potencia) > priv->limiteSlew)
                    {
                        // printf("potencia_t0 : %f", potencia_t0);
                        if (priv->potencia_t0 < priv->potencia_t1)
                        {
                            priv->potencia_t0 = priv->potencia_t1 - priv->limiteSlew;
                        }
                        else
                        {
                            priv->potencia_t0 = priv->potencia_t1 + priv->limiteSlew;
                        }
                        // printf(" potencia_t0 despues de limit : %f\n", potencia_t0);
                    }
                }
            }

            break;
        default:
            break;
        }

        priv->resultado = (((int)(100 * priv->width_ref) << 16)) | ((int)(priv->W * 100));

        if (priv->potencia_t0 > priv->limPotenciaMax)
        {
            priv->potencia_t0 = priv->limPotenciaMax;
        }
        else
        {
            if (priv->potencia_t0 < priv->limPotenciaMin)
            {
                priv->potencia_t0 = priv->limPotenciaMin;
            }
        }
        priv->potencia_t1 = priv->potencia_t0;
        priv->metadatos[0] = priv->potencia_t0;
        priv->metadatos[7] = priv->track;
        priv->metadatos[11] = (priv->estadoAutomata << 24) | (priv->laser_status << 16) | priv->metadatos[11];

        priv->duty = (priv->potencia_t0 - priv->potenciaMin) * (16383 / (priv->potenciaMax - priv->potenciaMin));
        priv->mb_core_shm[PWM] = (unsigned int)priv->duty;
        priv->last_laser_status = priv->laser_status;

        memcpy((void*)&priv->virtual_metadata_shm[0], &priv->metadatos, 28);    // Power, MOM00, MOM01, MOM10, MOM11, MOM02, MOM20
        priv->virtual_metadata_shm[7] = priv->resultado;                         // Width
        memcpy((void*)&priv->virtual_metadata_shm[8], &priv->metadatos[7], 20); // Track Nmbr, Frame Max, Frame Number, Timestamp, IO Status

        // gestion de la alarma

        if ((priv->alarm_enable == 1) && (priv->laser_status))
        {
            if ((priv->W > priv->alarm_max) || (priv->W < priv->alarm_min))
            {
                if (priv->cnt_aux_alarm > 0)
                {
                    priv->cnt_aux_alarm--;
                    priv->alarm = 0;
                }
                else
                {
                    priv->alarm = 1;
                }
            }
            else
            {
                priv->cnt_aux_alarm = priv->proc_var_shm[ALARM_TIME];
                priv->alarm = 0;
            }
        }
        else
        {
            priv->cnt_aux_alarm = priv->proc_var_shm[ALARM_TIME];
            priv->alarm = 0;
        }

        // LEDS y autoshutter
        priv->cnt_last_autoshutter++;
        priv->current_temperature = ((float)priv->virtual_metadata_shm[13]) / 10;
        if (priv->auto_auto_shutter_time_xtemp)
        {
            if (priv->autoshutter_time_target < priv->cnt_last_autoshutter)
            {
                priv->pending_autoshutter = 1;
                priv->cnt_last_autoshutter = 0;
            }
        }
        else
        {
            if (priv->autoshutter_temp_target < (fabs(priv->temperature_last_autoshutter - priv->current_temperature)))
            {
                priv->pending_autoshutter = 1;
                priv->cnt_last_autoshutter = 0;
                priv->temperature_last_autoshutter = priv->current_temperature;
            }
        }

        switch (priv->estadoAutomata)
        {
        case MANUAL:
            if (priv->auto_auto_shutter_enable && priv->pending_autoshutter)
            {
                priv->proc_var_shm[AUTO_SHUTTER] = 1;
                priv->pending_autoshutter = 0;
            }
            break;
        case IDLE:
            if (priv->auto_auto_shutter_enable && priv->pending_autoshutter)
            {
                priv->proc_var_shm[AUTO_SHUTTER] = 1;
                priv->pending_autoshutter = 0;
            }
            break;
        case MIDIENDO:
            if (priv->auto_auto_shutter_enable && priv->auto_auto_shutter_enable_inprocess && priv->pending_autoshutter)
            {
                priv->proc_var_shm[AUTO_SHUTTER] = 1;
                priv->pending_autoshutter = 0;
            }

            break;

        case CONTROL:
            if (priv->auto_auto_shutter_enable && priv->auto_auto_shutter_enable_inprocess && priv->pending_autoshutter)
            {
                priv->proc_var_shm[AUTO_SHUTTER] = 1;
                priv->pending_autoshutter = 0;
            }
            break;

        default:
            break;
        }

        if (priv->alarm == 1)
        {
            // ROJO
            priv->arm_core_shm[LED_R] = 0;
            priv->arm_core_shm[LED_G] = 1;
            priv->arm_core_shm[LED_B] = 1;
        }
        else
        {

            switch (priv->estadoAutomata)
            {
            case MANUAL: // AMARILLO
                priv->arm_core_shm[LED_R] = 0;
                priv->arm_core_shm[LED_G] = 0;
                priv->arm_core_shm[LED_B] = 1;

                break;
            case IDLE: // VERDE
                priv->arm_core_shm[LED_R] = 1;
                priv->arm_core_shm[LED_G] = 0;
                priv->arm_core_shm[LED_B] = 1;

                break;

            case MIDIENDO: // MORADO
                priv->arm_core_shm[LED_R] = 0;
                priv->arm_core_shm[LED_G] = 1;
                priv->arm_core_shm[LED_B] = 0;

                break;

            case CONTROL: // AZUL
                priv->arm_core_shm[LED_R] = 1;
                priv->arm_core_shm[LED_G] = 1;
                priv->arm_core_shm[LED_B] = 0;

                break;
            default:
                break;
            }
        }

        if (priv->alarm_enable == 1)
        { // Solo si la alarma est� habilitada
            priv->mb_core_shm[DIGITAL_OUT_0] = priv->alarm;
        }

        sem_getvalue(priv->semaforo, &priv->valor1);
        if (priv->valor1 < 1)
        { // permite al productor enviar hasta 2 imagenes por TCP al empezar la conexion
            sem_post(priv->semaforo);
        }
        sem_getvalue(priv->semaforo, &priv->valor2);
    }

    sem_close(priv->semaforo);
    sem_unlink(SEM_NAME);

    return 0;
}

template <typename state_type, typename type>
constexpr type process_core_unsafe_get(state_type* state, size_t offset)
{
    auto priv = (nit_process_core_private_state*)state->priv;
    return (type) * (((volatile uint32_t*)priv->proc_var_shm) + offset);
}

template <typename state_type, typename type>
constexpr type process_core_unsafe_set(state_type* state, size_t offset, type value)
{
    auto priv = (nit_process_core_private_state*)state->priv;
    return (type)(*(((uint32_t*)priv->proc_var_shm) + offset) = value);
}

#define DRIVER_FIELD_AS_FUNCTION_DEFINITION(name, parameter, type, size, offset)   \
    int nit_##name##_##parameter##_set(nit_##name##_state_t *state, type value)    \
    {                                                                              \
        print_debug("%s: (%04x): %d\n", __func__, offset, value);                  \
        int retval = 0;                                                            \
        if ((retval = nit_##name##_assert(state)) != 0)                            \
        {                                                                          \
            print_debug("%s: %s %d\n", __func__, "failed", retval);                \
            return retval;                                                         \
        }                                                                          \
        process_core_unsafe_set<typeof(*state), uint32_t>(state, offset, value);   \
        return retval;                                                             \
    }                                                                              \
                                                                                   \
    int nit_##name##_##parameter##_get(nit_##name##_state_t *state, type *value)   \
    {                                                                              \
        int retval = 0;                                                            \
                                                                                   \
        if ((retval = nit_##name##_assert(state)) != 0)                            \
        {                                                                          \
            print_debug("%s: %s %d\n", __func__, "failed", retval);                \
            return retval;                                                         \
        }                                                                          \
                                                                                   \
        *value = process_core_unsafe_get<typeof(*state), uint32_t>(state, offset); \
        print_debug("%s: (%04x): %d\n", __func__, offset, *value);                 \
        return retval;                                                             \
    }

NIT_PROCESS_CORE_FIELD_TABLE(DRIVER_FIELD_AS_FUNCTION_DEFINITION)

double nit_process_core_calculate_width(int metadatos[12])
{
    double W = 0;
    double M00, M01, M10, M11, M02, M20;
    double X, Y;
    double u20, u11, u02;
    M00 = metadatos[1];
    M01 = metadatos[2];
    M10 = metadatos[3];
    M11 = metadatos[4];
    M02 = metadatos[5];
    M20 = metadatos[6];
    // Procesos de calculo
    if (M00 == 0)
        M00 = 1;
    X = M10 / M00;
    Y = M01 / M00;

    u20 = (M20 / M00) - (X * X);
    u11 = (M11 / M00) - (X * Y);
    u02 = (M02 / M00) - (Y * Y);

    W = sqrt(8 * (u20 + u02 - sqrt((4 * u11 * u11) + ((u20 - u02) * (u20 - u02))))); // Aqui se ha calculado el ancho
    return W;
}
