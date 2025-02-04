#include "common/defs.h"
#include "utils/config_file.h"

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

    sem_t *semaforo;

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
    uint16_t alarm_enable; // 1 alarm ON, 0 alarm OFF
    double alarm_min;
    double alarm_max;
    uint16_t alarm_time;
    volatile int &cnt_aux_alarm = proc_var_shm[ALARM_TIME];
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

    volatile int *proc_var_shm;
    volatile int *virtual_metadata_shm;
    volatile int *arm_core_shm;
    volatile int *real_metadata_shm;
    volatile int *gen_core_shm;
    volatile int *control_unit_shm;
    volatile int *metaPtr;
};

nit_process_core_state_t nit_process_core_driver;

double nit_process_core_calculate_width(int metadatos[12]);
nit_process_core_private_state *nit_process_core_process_data_ptr_get(nit_process_core_state_t *state);

int nit_process_core_open(nit_process_core_state_t *state, nit_arm_core_state_t *nit_arm_core_state, nit_control_unit_core_state_t *nit_control_unit_core_state, nit_mb_core_state_t *nit_mb_core_state, nit_framebuffer_core_state_t *nit_framebuffer_core_state)
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

    state->fd = open("/dev/mem", O_RDWR | O_SYNC);

    if (state->fd < 0)
    {
        return -2;
    }

    state->priv = (void*) malloc(sizeof(nit_process_core_private_state));

    if (state->priv == NULL)
    {
        return -3;
    }

    auto process = nit_process_core_process_data_ptr_get(state);

    process->semaforo = sem_open(SEM_NAME, O_CREAT, 0644, 0);

    // state->priv = (volatile int *)(volatile int *)mmap(NULL, 512, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // if (state->priv == nullptr) {
    //     return -4;
    // }

    state->priv = malloc(sizeof(nit_process_core_private_state));

    if (state->priv == nullptr)
    {
        return -1;
    }

    process->proc_var_shm = (volatile int *)mmap(NULL, 512, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    process->virtual_metadata_shm = (volatile int *)mmap(NULL, 256, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (process->virtual_metadata_shm == nullptr)
    {
        return -5;
    }

    if (nit_arm_core_assert(nit_arm_core_state) < 0){
        return -6;
    }

    process->arm_core_shm = (volatile int *)nit_arm_core_state->priv;

    if (nit_control_unit_core_assert(nit_control_unit_core_state) < 0) {
        return -7;
    }

    process->control_unit_shm = (volatile int *)nit_control_unit_core_state->priv;

    if (nit_mb_core_state_assert(nit_mb_core_state) < 0) {
        return  -8;
    }

    process->gen_core_shm = (volatile int *)nit_mb_core_state->priv;

    if (nit_framebuffer_core_state_assert(nit_framebuffer_core_state) < 0) {
        return -9;
    }

    process->real_metadata_shm = (volatile int *) nit_framebuffer_core_metadata(nit_framebuffer_core_state);

    if (state->priv == NULL)
    {
        close(state->fd);
        return -10;
    }

    state->is_open = true;

    return 0;
}

int nit_process_core_close(nit_process_core_state_t *state)
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
        munmap((void *)state->priv, NIT_PROCESS_CORE_SIZE);
        state->priv = NULL;
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }

    return 0;
}

int nit_process_core_assert(nit_process_core_state_t *state)
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

nit_process_core_private_state *nit_process_core_process_data_ptr_get(nit_process_core_state_t *state)
{
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return nullptr;
    }

    return (nit_process_core_private_state *)state->priv;
}

int nit_process_core_config_save_to_file(nit_process_core_state_t *state, const char *path)
{
    return config_file_save_to_file(state, path);
}

int nit_process_core_config_load_from_file(nit_process_core_state_t *state, const char *path)
{
    return config_file_load_from_file(state, path);
}

#include <utils/waitable.hpp>

int nit_process_core_run(nit_process_core_state_t *state, utils::waitable &timer)
{

    auto process_data = nit_process_core_process_data_ptr_get(state);

    process_data->estadoAutomata = 0;
    process_data->valor1 = 0;
    process_data->valor2 = 0;
    process_data->contadorFramesMidiendo = 0;
    process_data->width_ref = 1;
    process_data->error_t0 = 0;
    process_data->error_t1 = 0;
    process_data->potencia_t0 = 0;
    process_data->potencia_t1 = 0;
    process_data->dt = 0.001;
    process_data->mode = 0;
    process_data->laser_status = 0; // 0 OFF, 1 ON
    process_data->contadorFramesStop = 0;
    process_data->endP = 1000;
    process_data->delta_potencia = 0;
    process_data->track_cnt = 0;
    process_data->track = 0;
    process_data->track_length = 2000;
    process_data->semaforo = sem_open(SEM_NAME, O_CREAT, 0644, 0);
    process_data->pending = 0;
    process_data->enable = 1;
    process_data->metaPtr = process_data->real_metadata_shm;
    process_data->W = 0;
    process_data->width_aux = 0;
    process_data->W_circ_buff_aux = 0;

    // process_data->w_circ_buffer[512];

    for (int i = 0; i < 512; i++)
    {
        process_data->w_circ_buffer[i] = 0;
    }

    process_data->control_NAP = 0;
    process_data->measurement_NAP = 0;
    process_data->cont_calibration = 0;
    process_data->resultado = 0;
    process_data->cont = 0;
    process_data->z = 0;
    process_data->circ_buffer_size = 1;
    // write(fd_int, (void *)&enable, sizeof(int));
    process_data->derivative = 0;
    process_data->temperature_last_autoshutter = ((float)process_data->virtual_metadata_shm[13]) / 10;
    process_data->cnt_last_autoshutter = 0;
    process_data->pending_autoshutter = 0;
    process_data->laser_external = 0;
    process_data->last_laser_status = 0;
    process_data->delay_laser_on = 0;
    process_data->cont_preheating = 0;
    while (1)
    {
        process_data->ki = process_data->proc_var_shm[KI];
        process_data->kp = process_data->proc_var_shm[KP];
        process_data->kd = process_data->proc_var_shm[KD];
        process_data->potenciaMax = (double)process_data->proc_var_shm[MAX_POWER];
        process_data->potenciaMin = (double)process_data->proc_var_shm[MIN_POWER];
        if (process_data->potenciaMin > 32767)
        {
            process_data->potenciaMin = process_data->potenciaMin - 65536;
        }
        process_data->limPotenciaMax = process_data->proc_var_shm[POWER_LIMIT_MAX];
        process_data->limPotenciaMin = process_data->proc_var_shm[POWER_LIMIT_MIN];
        process_data->pixel_mm_ratio = ((double)process_data->proc_var_shm[PIXEL_MM_RATIO] / 1000); // se reciben en micras
        process_data->proc_var_shm[PID_ERROR] = (int)100 * process_data->error_t1;
        process_data->limiteIntegral = (double)process_data->proc_var_shm[LIMIT_INTEGRAL];
        process_data->limiteSlew = ((double)process_data->proc_var_shm[LIMIT_SLEW] / 100);
        process_data->circ_buffer_size = process_data->proc_var_shm[BUFF_SIZE];

        process_data->alarm_enable = process_data->proc_var_shm[ENABLE_ALARM];
        process_data->alarm_max = ((double)process_data->proc_var_shm[ALARM_MAX]) / 100;
        process_data->alarm_min = ((double)process_data->proc_var_shm[ALARM_MIN]) / 100;
        process_data->alarm_time = process_data->proc_var_shm[ALARM_TIME];
        process_data->automeasure_flag = process_data->proc_var_shm[AUTOMEASURE];

        process_data->autoshutter_config = process_data->proc_var_shm[AUTOSHUTTER_CONFIG]; // booleano, comprobar flags
        // decode_config
        process_data->auto_auto_shutter_enable = process_data->autoshutter_config & 0x01;
        process_data->auto_auto_shutter_enable_inprocess = (process_data->autoshutter_config >> 1) & 0x01;
        process_data->auto_auto_shutter_time_xtemp = (process_data->autoshutter_config >> 3) & 0x01;
        // decodee config end
        process_data->autoshutter_temp_target = ((float)process_data->proc_var_shm[AUTOSHUTTER_TEMP]) / 10; // de int16 recibido a double o float, dividir entre 10
        process_data->autoshutter_time_target = process_data->proc_var_shm[AUTOSHUTTER_TIMER] * 10000;      //	de int16 recibido a int32, multiplicar por 10

        process_data->laser_external = process_data->proc_var_shm[LASER_EXTERAL_CONTROL];

        if (process_data->circ_buffer_size < 1)
        {
            process_data->circ_buffer_size = 1;
        }
        else if (process_data->circ_buffer_size > LMAX_CIRCULAR_BUFFER_SIZE)
        {
            process_data->circ_buffer_size = LMAX_CIRCULAR_BUFFER_SIZE;
        }
        if (process_data->mode != process_data->gen_core_shm[MODE])
        {
            process_data->mode = process_data->gen_core_shm[MODE];
            if (process_data->mode == 2)
            {
                process_data->estadoAutomata = MANUAL;
                printf("\nCambio a estado MANUAL\n");
            }
            else
            {
                process_data->estadoAutomata = IDLE;
                printf("\nCambio a estado IDLE\n");
                process_data->gen_core_shm[CHANGE_OP_MODE] = 1;
            }
        }

        // read(fd_int, (int *)&pending, sizeof(int)); // Se bloquea hasta que sucede una interrupcion de uio0
        timer.wait();

        memcpy(process_data->metadatos, (void *)process_data->metaPtr, 48);

        // Laser status
        if (process_data->laser_external)
        {
            process_data->laser_status = process_data->gen_core_shm[DIGITAL_IN_0];
            // laser_status = gen_core_shm[DIGITAL_IN_1];
        }
        else
        {
            if (process_data->laser_status)
            {
                if (process_data->metadatos[1] < process_data->gen_core_shm[END_OF_TRACK])
                    process_data->laser_status = 0;
            }
            else
            {
                if (process_data->metadatos[1] >= process_data->gen_core_shm[START_TRACK_MOM_T])
                    process_data->laser_status = 1;
            }
        }

        // write(fd_int, (void *)&enable, sizeof(int)); // Habilita la interrupcion de uio0

        /*
         * Cambio a lectura buffer circular
         */
        if (process_data->proc_var_shm[AUTO_SHUTTER])
        {

            process_data->proc_var_shm[AUTO_SHUTTER] = 0;
            if (!process_data->measurement_NAP)
            {
                process_data->measurement_NAP = NAP_DURATION;
                process_data->control_NAP = NAP_DURATION;
                // proc_var_shm[AUTO_SHUTTER] = 0;
                process_data->cont_calibration = WAIT_START_CALIBRATION + 1 + WAIT_STOP_CALIBRATION + WAIT_SECOND_APERTURE;
                // if(control_unit_shm[NIT_SHUTTER] == 0)
                //{
                process_data->control_unit_shm[NIT_SHUTTER] = 1; // cerrar shutter
                                                                //}
                process_data->cnt_last_autoshutter = 0;
                process_data->temperature_last_autoshutter = process_data->current_temperature;
            }
            // control_unit_shm[NIT_SHUTTER_RESET]=1;
        }
        if (!process_data->measurement_NAP)
        {
            process_data->gen_core_shm[DIGITAL_OUT_3] = 0;
            if (process_data->delay_laser_on > 0)
            {
                process_data->delay_laser_on--;
            }
            process_data->z += 1;
            if (process_data->z >= process_data->circ_buffer_size)
            {
                process_data->z = 0;
            }
            process_data->w_circ_buffer[process_data->z] = process_data->pixel_mm_ratio * nit_process_core_calculate_width(process_data->metadatos);
            process_data->W_circ_buff_aux = 0;
            for (int y = 0; y < process_data->circ_buffer_size; y++)
            {
                process_data->W_circ_buff_aux += process_data->w_circ_buffer[y];
            }
            process_data->W = process_data->W_circ_buff_aux / (process_data->circ_buffer_size);
        }
        else
        { // siesta
            process_data->gen_core_shm[DIGITAL_OUT_3] = 1;
            process_data->W = process_data->W;
            if ((process_data->cont_calibration <= 0) && (process_data->control_unit_shm[NIT_SHUTTER] != 0))
            {
                process_data->control_unit_shm[NIT_SHUTTER] = 0; // open shutter
            }
            else
            {
                if (process_data->cont_calibration == (WAIT_SECOND_APERTURE))
                {
                    //	control_unit_shm[NIT_SHUTTER] = 0;
                }
                if (process_data->cont_calibration == (1 + WAIT_STOP_CALIBRATION + WAIT_SECOND_APERTURE))
                {
                    process_data->control_unit_shm[NIT_OFFSET_UPDATE] = 1;
                    // control_unit_shm[NIT_SHUTTER_RESET]=1;
                }
                // cont_calibration--;
            }
            process_data->cont_calibration--;
            process_data->measurement_NAP--;
            process_data->control_NAP--;
        }

        if (process_data->proc_var_shm[SET_REF_WIDTH] > 0)
        {
            process_data->width_ref = ((double)process_data->proc_var_shm[WIDTH_REF]) / 100;
            process_data->proc_var_shm[SET_REF_WIDTH] = 0;
        }

        switch (process_data->estadoAutomata)
        {
        case MANUAL:
            process_data->potencia_t0 = process_data->proc_var_shm[POWER_MAN];
            if (process_data->mode != 2)
            {
                process_data->estadoAutomata = IDLE;
                printf("\nCambio a estado IDLE\n");
                process_data->gen_core_shm[CHANGE_OP_MODE] = 1;
            }
            process_data->track_cnt = 0;
            process_data->track = 0;
            process_data->delay_laser_on = 0;
            break;

        case IDLE:
            process_data->gen_core_shm[CHANGE_OP_MODE] = 1;
            process_data->width_aux = 0;
            process_data->contadorFramesStop = 0;

            if (process_data->laser_status)
            {
                if (process_data->proc_var_shm[PREHEATING_ENA])
                {
                    process_data->cont_preheating = 0;
                    process_data->estadoAutomata = PREHEATING;
                    printf("\nCambio a estado PREHEATING\n");
                }
                else
                {
                    process_data->estadoAutomata = MIDIENDO;
                    printf("\nCambio a estado MIDIENDO\n");
                    process_data->contadorFramesMidiendo = 0;
                    pthread_mutex_unlock(&process_data->lock);
                }
                process_data->track_length = ((unsigned int)process_data->gen_core_shm[TIME_TRACK_HIGH]);
                process_data->track_length = (((process_data->track_length << 32) | ((unsigned int)process_data->gen_core_shm[TIME_TRACK_LOW])) * 1000) / (unsigned long)100e6;
            }
            else
            {
                process_data->potencia_t0 = process_data->proc_var_shm[POWER_MAN];
                process_data->potencia_t1 = process_data->potencia_t0;
            }
            process_data->track_cnt = 0;
            process_data->track = 0;
            process_data->delay_laser_on = 0;
            break;

        case PREHEATING:
            process_data->potencia_t0 = process_data->proc_var_shm[PREHEATING_POWER];
            if (process_data->cont_preheating++ >= process_data->proc_var_shm[PREHEATING_TIME])
            {
                process_data->estadoAutomata = MIDIENDO;
                process_data->contadorFramesMidiendo = 0;
                pthread_mutex_unlock(&process_data->lock);
                printf("\nCambio a estado MIDIENDO\n");
            }
            break;

        case MIDIENDO:
            process_data->cont++;
            if (process_data->mode == 0)
            {
                if (process_data->track_cnt >= process_data->track_length)
                {
                    process_data->track_cnt = 0;
                    process_data->track++;
                }
                else
                {
                    process_data->track_cnt++;
                }
            }
            else if (process_data->mode == 1)
            {
                if ((process_data->laser_status > 0) && (process_data->laser_status != process_data->last_laser_status))
                {
                    if (process_data->laser_external)
                    {
                        process_data->track++;
                        process_data->delay_laser_on = process_data->proc_var_shm[DELAY_LASER_ON];
                    }
                    else
                    {
                        if (!process_data->measurement_NAP)
                        {
                            process_data->track++;
                            process_data->delay_laser_on = process_data->proc_var_shm[DELAY_LASER_ON];
                        }
                    }
                }
            }
            // if ((laser_status) & (metadatos[7] >= proc_var_shm[TRACK_REF_START])) //sustituido por cuenta de tracks en el ARM
            if ((process_data->laser_status) & (process_data->track >= process_data->proc_var_shm[TRACK_REF_START]))
            {
                process_data->width_aux = process_data->width_aux + process_data->W;
                process_data->contadorFramesMidiendo++;
            }
            process_data->potencia_t0 = process_data->proc_var_shm[POWER_MAN];
            process_data->potencia_t1 = process_data->potencia_t0;
            // if (metadatos[7] >= gen_core_shm[REFERENCE_TRACK]){
            if (process_data->track >= process_data->gen_core_shm[REFERENCE_TRACK])
            {
                process_data->estadoAutomata = CONTROL;
                printf("\nCambio a estado CONTROL\n");
                if (process_data->contadorFramesMidiendo == 0)
                {
                    process_data->contadorFramesMidiendo = 1;
                }
                if (process_data->automeasure_flag == 1)
                {
                    process_data->width_ref = process_data->width_aux / process_data->contadorFramesMidiendo;
                }
                else
                {
                    process_data->width_ref = ((double)process_data->proc_var_shm[WIDTH_REF]) / 100;
                }
                process_data->width_aux = 0;
                process_data->error_t0 = 0;
                process_data->error_t1 = 0;
                process_data->integral = 0;
                process_data->derivative = 0;
                process_data->contadorFramesMidiendo = 0;
            }
            break;

        case CONTROL:
            process_data->endP = process_data->proc_var_shm[END_OF_PROCESS];
            process_data->cont++;

            if (process_data->mode == 0)
            {
                if (process_data->track_cnt >= process_data->track_length)
                {
                    process_data->track_cnt = 0;
                    process_data->track++;
                }
                else
                {
                    process_data->track_cnt++;
                }
            }
            else if (process_data->mode == 1)
            {
                if ((process_data->laser_status > 0) && (process_data->laser_status != process_data->last_laser_status))
                {
                    if (process_data->laser_external)
                    {
                        process_data->track++;
                        process_data->delay_laser_on = process_data->proc_var_shm[DELAY_LASER_ON];
                    }
                    else
                    {
                        if (!process_data->measurement_NAP)
                        {
                            process_data->track++;
                            process_data->delay_laser_on = process_data->proc_var_shm[DELAY_LASER_ON];
                        }
                    }
                }
            }
            if (process_data->delay_laser_on > 0)
            {
                process_data->delay_laser_on--;
            }
            else
            {
                if ((!process_data->laser_status) || (process_data->control_NAP))
                {
                    if (process_data->contadorFramesStop++ > process_data->endP)
                    {
                        process_data->estadoAutomata = IDLE;
                        printf("\nCambio a estado IDLE\n");
                        process_data->contadorFramesStop = 0;
                        process_data->cont = 0;
                        process_data->stop_logging = 1;
                        pthread_mutex_unlock(&process_data->lock);
                    }
                }
                else
                {
                    process_data->contadorFramesStop = 0;
                    // Comienza el c�clculo de la potencia
                    process_data->error_t0 = process_data->width_ref - process_data->W;
                    process_data->integral = process_data->integral + (process_data->error_t0 * process_data->dt);

                    if (process_data->ki > 0)
                    {
                        process_data->limit_integral_divided = process_data->limiteIntegral / process_data->ki;
                    }
                    else
                    {
                        process_data->limit_integral_divided = 1000;
                    }
                    if (fabs(process_data->integral) >= (process_data->limit_integral_divided))
                    {
                        if (process_data->integral < 0)
                        {
                            process_data->integral = 0 - process_data->limit_integral_divided;
                        }
                        else
                        {
                            process_data->integral = process_data->limit_integral_divided;
                        }
                    }

                    //				derivative = (error_t0 - error_t1)/dt; //comentada por posibilidad de valor muy grande
                    process_data->derivative = (process_data->error_t0 - process_data->error_t1);
                    process_data->potencia_t0 = process_data->proc_var_shm[POWER_MAN] + (process_data->kp * process_data->error_t0) + (process_data->ki * process_data->integral) + (process_data->kd * process_data->derivative);
                    process_data->error_t1 = process_data->error_t0;

                    process_data->delta_potencia = process_data->potencia_t1 - process_data->potencia_t0;
                    if (abs(process_data->delta_potencia) > process_data->limiteSlew)
                    {
                        // printf("potencia_t0 : %f", potencia_t0);
                        if (process_data->potencia_t0 < process_data->potencia_t1)
                        {
                            process_data->potencia_t0 = process_data->potencia_t1 - process_data->limiteSlew;
                        }
                        else
                        {
                            process_data->potencia_t0 = process_data->potencia_t1 + process_data->limiteSlew;
                        }
                        // printf(" potencia_t0 despues de limit : %f\n", potencia_t0);
                    }
                }
            }

            break;
        default:
            break;
        }

        process_data->resultado = (((int)(100 * process_data->width_ref) << 16)) | ((int)(process_data->W * 100));

        if (process_data->potencia_t0 > process_data->limPotenciaMax)
        {
            process_data->potencia_t0 = process_data->limPotenciaMax;
        }
        else
        {
            if (process_data->potencia_t0 < process_data->limPotenciaMin)
            {
                process_data->potencia_t0 = process_data->limPotenciaMin;
            }
        }
        process_data->potencia_t1 = process_data->potencia_t0;
        process_data->metadatos[0] = process_data->potencia_t0;
        process_data->metadatos[7] = process_data->track;
        process_data->metadatos[11] = (process_data->estadoAutomata << 24) | (process_data->laser_status << 16) | process_data->metadatos[11];
        // Comienza el c�lculo del duty cycle
        // duty = (potencia_t0 - potenciaMin) * (1000/(potenciaMax - potenciaMin));
        process_data->duty = (process_data->potencia_t0 - process_data->potenciaMin) * (16383 / (process_data->potenciaMax - process_data->potenciaMin));
        process_data->gen_core_shm[PWM] = (unsigned int)process_data->duty;
        process_data->last_laser_status = process_data->laser_status;

        memcpy((void *)&process_data->virtual_metadata_shm[0], &process_data->metadatos, 28);    // Power, MOM00, MOM01, MOM10, MOM11, MOM02, MOM20
        process_data->virtual_metadata_shm[7] = process_data->resultado;                         // Width
        memcpy((void *)&process_data->virtual_metadata_shm[8], &process_data->metadatos[7], 20); // Track Nmbr, Frame Max, Frame Number, Timestamp, IO Status

        // gestion de la alarma

        if ((process_data->alarm_enable == 1) && (process_data->laser_status))
        {
            if ((process_data->W > process_data->alarm_max) || (process_data->W < process_data->alarm_min))
            {
                if (process_data->cnt_aux_alarm > 0)
                {
                    process_data->cnt_aux_alarm--;
                    process_data->alarm = 0;
                }
                else
                {
                    process_data->alarm = 1;
                }
            }
            else
            {
                process_data->cnt_aux_alarm = process_data->alarm_time;
                process_data->alarm = 0;
            }
        }
        else
        {
            process_data->cnt_aux_alarm = process_data->alarm_time;
            process_data->alarm = 0;
        }

        // Gestion de logging
        if (process_data->cont >= 100)
        {
            process_data->cont = 0;
            pthread_mutex_unlock(&process_data->lock);
        }

        // LEDS y autoshutter
        process_data->cnt_last_autoshutter++;
        process_data->current_temperature = ((float)process_data->virtual_metadata_shm[13]) / 10;
        if (process_data->auto_auto_shutter_time_xtemp)
        {
            if (process_data->autoshutter_time_target < process_data->cnt_last_autoshutter)
            {
                process_data->pending_autoshutter = 1;
                process_data->cnt_last_autoshutter = 0;
            }
        }
        else
        {
            if (process_data->autoshutter_temp_target < (fabs(process_data->temperature_last_autoshutter - process_data->current_temperature)))
            {
                process_data->pending_autoshutter = 1;
                process_data->cnt_last_autoshutter = 0;
                process_data->temperature_last_autoshutter = process_data->current_temperature;
            }
        }

        switch (process_data->estadoAutomata)
        {
        case MANUAL:
            if (process_data->auto_auto_shutter_enable && process_data->pending_autoshutter)
            {
                process_data->proc_var_shm[AUTO_SHUTTER] = 1;
                process_data->pending_autoshutter = 0;
            }
            break;
        case IDLE:
            if (process_data->auto_auto_shutter_enable && process_data->pending_autoshutter)
            {
                process_data->proc_var_shm[AUTO_SHUTTER] = 1;
                process_data->pending_autoshutter = 0;
            }
            break;
        case MIDIENDO:
            if (process_data->auto_auto_shutter_enable && process_data->auto_auto_shutter_enable_inprocess && process_data->pending_autoshutter)
            {
                process_data->proc_var_shm[AUTO_SHUTTER] = 1;
                process_data->pending_autoshutter = 0;
            }

            break;

        case CONTROL:
            if (process_data->auto_auto_shutter_enable && process_data->auto_auto_shutter_enable_inprocess && process_data->pending_autoshutter)
            {
                process_data->proc_var_shm[AUTO_SHUTTER] = 1;
                process_data->pending_autoshutter = 0;
            }
            break;

        default:
            break;
        }

        if (process_data->alarm == 1)
        {
            // ROJO
            process_data->arm_core_shm[LED_R] = 0;
            process_data->arm_core_shm[LED_G] = 1;
            process_data->arm_core_shm[LED_B] = 1;
        }
        else
        {

            switch (process_data->estadoAutomata)
            {
            case MANUAL: // AMARILLO
                process_data->arm_core_shm[LED_R] = 0;
                process_data->arm_core_shm[LED_G] = 0;
                process_data->arm_core_shm[LED_B] = 1;

                break;
            case IDLE: // VERDE
                process_data->arm_core_shm[LED_R] = 1;
                process_data->arm_core_shm[LED_G] = 0;
                process_data->arm_core_shm[LED_B] = 1;

                break;

            case MIDIENDO: // MORADO
                process_data->arm_core_shm[LED_R] = 0;
                process_data->arm_core_shm[LED_G] = 1;
                process_data->arm_core_shm[LED_B] = 0;

                break;

            case CONTROL: // AZUL
                process_data->arm_core_shm[LED_R] = 1;
                process_data->arm_core_shm[LED_G] = 1;
                process_data->arm_core_shm[LED_B] = 0;

                break;
            default:
                break;
            }
        }

        process_data->gen_core_shm[DIGITAL_OUT_1] = 1; // naranja
        if (process_data->alarm_enable == 1)
        { // Solo si la alarma est� habilitada

            if (process_data->alarm == 1)
            {
                process_data->gen_core_shm[DIGITAL_OUT_0] = 0; // asignacion de la digital out 1 a la alarma
                process_data->gen_core_shm[DIGITAL_OUT_2] = 1; // verde
            }
            else
            {
                process_data->gen_core_shm[DIGITAL_OUT_0] = 1; // asignacion de la digital out 1 a la alarma
                process_data->gen_core_shm[DIGITAL_OUT_2] = 0; // verde
            }
        }
        else
        {
            process_data->gen_core_shm[DIGITAL_OUT_0] = 1; // asignacion de la digital out 1 a la alarma
            process_data->gen_core_shm[DIGITAL_OUT_2] = 0; // verde
        }

        sem_getvalue(process_data->semaforo, &process_data->valor1);
        if (process_data->valor1 < 1)
        { // permite al productor enviar hasta 2 imagenes por TCP al empezar la conexion
            sem_post(process_data->semaforo);
        }
        sem_getvalue(process_data->semaforo, &process_data->valor2);
    }
    // close(fd_int);
    // close(fdMetadatos);
    // munmap(((int *)metaPtr), 64);
    // metaPtr = NULL;
    sem_close(process_data->semaforo);
    sem_unlink(SEM_NAME);
    exit(1);
}

int nit_process_core_kp_set(nit_process_core_state_t *state, uint16_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_kp_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_kp_offset, value);
    return retval;
}

int nit_process_core_kp_get(nit_process_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_kp_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_kp_offset, *value);
    return retval;
}

int nit_process_core_ki_get(nit_process_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_ki_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_ki_offset, *value);
    return retval;
}

int nit_process_core_ki_set(nit_process_core_state_t *state, uint16_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_kp_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_ki_offset, value);
    return retval;
}

int nit_process_core_kd_get(nit_process_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_kd_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_kd_offset, *value);
    return retval;
}

int nit_process_core_kd_set(nit_process_core_state_t *state, uint16_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_kp_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_kd_offset, value);
    return retval;
}

int nit_process_core_max_power_get(nit_process_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_max_power_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_max_power_offset, *value);
    return retval;
}

int nit_process_core_max_power_set(nit_process_core_state_t *state, uint16_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_kp_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_max_power_offset, value);
    return retval;
}

int nit_process_core_min_power_get(nit_process_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_min_power_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_min_power_offset, *value);
    return retval;
}

int nit_process_core_min_power_set(nit_process_core_state_t *state, uint16_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_kp_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_min_power_offset, value);
    return retval;
}

int nit_process_core_power_man_get(nit_process_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_power_man_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_power_man_offset, *value);
    return retval;
}

int nit_process_core_power_man_set(nit_process_core_state_t *state, uint16_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_kp_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_power_man_offset, value);
    return retval;
}

int nit_process_core_power_limit_max_get(nit_process_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_power_limit_max_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_power_limit_max_offset, *value);
    return retval;
}

int nit_process_core_power_limit_max_set(nit_process_core_state_t *state, uint16_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_kp_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_power_limit_max_offset, value);
    return retval;
}

int nit_process_core_power_limit_min_get(nit_process_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_power_limit_min_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_power_limit_min_offset, *value);
    return retval;
}

int nit_process_core_power_limit_min_set(nit_process_core_state_t *state, uint16_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_kp_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_power_limit_min_offset, value);
    return retval;
}

int nit_process_core_set_ref_width_get(nit_process_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_set_ref_width_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_set_ref_width_offset, *value);
    return retval;
}

int nit_process_core_set_ref_width_set(nit_process_core_state_t *state, uint16_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_kp_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_set_ref_width_offset, value);
    return retval;
}

int nit_process_core_width_ref_get(nit_process_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_width_ref_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_width_ref_offset, *value);
    return retval;
}

int nit_process_core_width_ref_set(nit_process_core_state_t *state, uint16_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_kp_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_width_ref_offset, value);
    return retval;
}

int nit_process_core_pixel_mm_ratio_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_pixel_mm_ratio_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_pixel_mm_ratio_offset, value);
    return retval;
}
int nit_process_core_pixel_mm_ratio_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_pixel_mm_ratio_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_pixel_mm_ratio_offset, *value);
    return retval;
}

int nit_process_core_pid_error_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_pid_error_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_pid_error_offset, value);
    return retval;
}
int nit_process_core_pid_error_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_pid_error_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_pid_error_offset, *value);
    return retval;
}

int nit_process_core_end_of_process_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_end_of_process_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_end_of_process_offset, value);
    return retval;
}
int nit_process_core_end_of_process_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_end_of_process_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_end_of_process_offset, *value);
    return retval;
}

int nit_process_core_limit_integral_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_limit_integral_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_limit_integral_offset, value);
    return retval;
}
int nit_process_core_limit_integral_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_limit_integral_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_limit_integral_offset, *value);
    return retval;
}

int nit_process_core_limit_slew_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_limit_slew_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_limit_slew_offset, value);
    return retval;
}
int nit_process_core_limit_slew_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_limit_slew_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_limit_slew_offset, *value);
    return retval;
}

int nit_process_core_buff_size_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_buff_size_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_buff_size_offset, value);
    return retval;
}
int nit_process_core_buff_size_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_buff_size_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_buff_size_offset, *value);
    return retval;
}

int nit_process_core_auto_shutter_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_auto_shutter_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_auto_shutter_offset, value);
    return retval;
}
int nit_process_core_auto_shutter_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_auto_shutter_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_auto_shutter_offset, *value);
    return retval;
}

int nit_process_core_enable_alarm_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_enable_alarm_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_enable_alarm_offset, value);
    return retval;
}
int nit_process_core_enable_alarm_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_enable_alarm_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_enable_alarm_offset, *value);
    return retval;
}

int nit_process_core_alarm_max_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_alarm_max_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_alarm_max_offset, value);
    return retval;
}
int nit_process_core_alarm_max_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_alarm_max_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_alarm_max_offset, *value);
    return retval;
}

int nit_process_core_alarm_min_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_alarm_min_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_alarm_min_offset, value);
    return retval;
}
int nit_process_core_alarm_min_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_alarm_min_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_alarm_min_offset, *value);
    return retval;
}

int nit_process_core_alarm_time_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_alarm_time_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_alarm_time_offset, value);
    return retval;
}
int nit_process_core_alarm_time_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_alarm_time_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_alarm_time_offset, *value);
    return retval;
}

int nit_process_core_serial_number_low_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_serial_number_low_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_serial_number_low_offset, value);
    return retval;
}
int nit_process_core_serial_number_low_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_serial_number_low_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_serial_number_low_offset, *value);
    return retval;
}

int nit_process_core_serial_number_high_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_serial_number_high_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_serial_number_high_offset, value);
    return retval;
}
int nit_process_core_serial_number_high_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_serial_number_high_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_serial_number_high_offset, *value);
    return retval;
}

int nit_process_core_automeasure_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_automeasure_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_automeasure_offset, value);
    return retval;
}
int nit_process_core_automeasure_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_automeasure_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_automeasure_offset, *value);
    return retval;
}

int nit_process_core_autoshutter_config_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_autoshutter_config_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_autoshutter_config_offset, value);
    return retval;
}
int nit_process_core_autoshutter_config_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_autoshutter_config_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_autoshutter_config_offset, *value);
    return retval;
}

int nit_process_core_autoshutter_temp_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_autoshutter_temp_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_autoshutter_temp_offset, value);
    return retval;
}
int nit_process_core_autoshutter_temp_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_autoshutter_temp_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_autoshutter_temp_offset, *value);
    return retval;
}

int nit_process_core_autoshutter_timer_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_autoshutter_timer_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_autoshutter_timer_offset, value);
    return retval;
}
int nit_process_core_autoshutter_timer_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_autoshutter_timer_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_autoshutter_timer_offset, *value);
    return retval;
}

int nit_process_core_track_ref_start_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_track_ref_start_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_track_ref_start_offset, value);
    return retval;
}
int nit_process_core_track_ref_start_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_track_ref_start_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_track_ref_start_offset, *value);
    return retval;
}

int nit_process_core_laser_exteral_control_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_laser_exteral_control_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_laser_exteral_control_offset, value);
    return retval;
}
int nit_process_core_laser_exteral_control_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_laser_exteral_control_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_laser_exteral_control_offset, *value);
    return retval;
}

int nit_process_core_delay_laser_on_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_delay_laser_on_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_delay_laser_on_offset, value);
    return retval;
}
int nit_process_core_delay_laser_on_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_delay_laser_on_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_delay_laser_on_offset, *value);
    return retval;
}

int nit_process_core_preheating_ena_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_preheating_ena_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_preheating_ena_offset, value);
    return retval;
}
int nit_process_core_preheating_ena_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_preheating_ena_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_preheating_ena_offset, *value);
    return retval;
}

int nit_process_core_preheating_time_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_preheating_time_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_preheating_time_offset, value);
    return retval;
}
int nit_process_core_preheating_time_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_preheating_time_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_preheating_time_offset, *value);
    return retval;
}

int nit_process_core_preheating_power_set(nit_process_core_state_t *state, uint32_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_preheating_power_offset, value);
    int retval = 0;

    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    unsafe_set<typeof(*state), uint32_t>(state, nit_process_core_preheating_power_offset, value);
    return retval;
}
int nit_process_core_preheating_power_get(nit_process_core_state_t *state, uint32_t *value)
{
    int retval;
    if ((retval = nit_process_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_process_core_preheating_power_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_process_core_preheating_power_offset, *value);
    return retval;
}

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
