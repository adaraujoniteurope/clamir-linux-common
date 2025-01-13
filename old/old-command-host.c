

enum tcp_protocol_header_command_target_enum
{
	COMMAND_TARGET_CONTROL_UNIT_CORE = 5,
	COMMAND_TARGET_MB_CORE_TYPE0 = 3,
	COMMAND_TARGET_MB_CORE_TYPE1 = 4,
};

enum tcp_protocol_header_command_control_unit_target_parameter_enum
{

	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_INT_TIME = 0x01,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_BIAS_V = 0x02,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_OFFSET_EN = 0x03,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_OFFSET_UPDATE = 0x04,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_SHUTTER = 0x05,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_BPC_EN = 0x06,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_BPC_MEM_WRITE = 0x07,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_BPC_IDENTIFY = 0x08,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_TEMP1 = 0x09,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_TEMP2 = 0x0A,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_TEMP3 = 0x0B,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_TEMP4 = 0x0C,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_TRIGGER_USEC = 0x0D,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_BLACK_LEVEL = 0x0E,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_SINCRONIZATION = 0x0F,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_SAVE_EMBEDDED_CONF = 0x10,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_ARM_SW_VERSION = 0x11,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_DRIFT_ENABLE = 0x12,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_DRIFT_POSITION = 0x13,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_DRIFT_LEVEL = 0x14,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_FPGA_VERSION = 0x15,

	COMMAND_TARGET_PARAMETER_MB_CORE_KI = 0x01,
	COMMAND_TARGET_PARAMETER_MB_CORE_KP = 0x02,
	COMMAND_TARGET_PARAMETER_MB_CORE_KD = 0x03,
	COMMAND_TARGET_PARAMETER_MB_CORE_MAX_POWER = 0x04,
	COMMAND_TARGET_PARAMETER_MB_CORE_MIN_POWER = 0x05,
	COMMAND_TARGET_PARAMETER_MB_CORE_POWER_MAN = 0x06,
	COMMAND_TARGET_PARAMETER_MB_CORE_AUTO_SHUTTER = 0x07,
	COMMAND_TARGET_PARAMETER_MB_CORE_SET_REF_WIDTH = 0x08,
	COMMAND_TARGET_PARAMETER_MB_CORE_POWER_LIMIT_MAX = 0x09,
	COMMAND_TARGET_PARAMETER_MB_CORE_POWER_LIMIT_MIN = 0x0A,
	COMMAND_TARGET_PARAMETER_MB_CORE_WIDTH_REF = 0x0B,
	COMMAND_TARGET_PARAMETER_MB_CORE_PIXEL_MM_RATIO = 0x0C,
	COMMAND_TARGET_PARAMETER_MB_CORE_PID_ERROR = 0x0D,
	COMMAND_TARGET_PARAMETER_MB_CORE_END_OF_PROCESS = 0x0E,
	COMMAND_TARGET_PARAMETER_MB_CORE_LIMIT_INTEGRAL = 0x0F,
	COMMAND_TARGET_PARAMETER_MB_CORE_LIMIT_SLEW = 0x10,
	COMMAND_TARGET_PARAMETER_MB_CORE_BUFF_SIZE = 0x11,
	COMMAND_TARGET_PARAMETER_MB_CORE_ENABLE_ALARM = 0x12,
	COMMAND_TARGET_PARAMETER_MB_CORE_ALARM_MAX = 0x13,
	COMMAND_TARGET_PARAMETER_MB_CORE_ALARM_MIN = 0x14,
	COMMAND_TARGET_PARAMETER_MB_CORE_ALARM_TIME = 0x15,
	COMMAND_TARGET_PARAMETER_MB_CORE_SERIAL_NUMBER_LOW = 0x16,
	COMMAND_TARGET_PARAMETER_MB_CORE_AUTOMEASURE = 0x17,
	COMMAND_TARGET_PARAMETER_MB_CORE_AUTOSHUTTER_CONFIG = 0x18,
	COMMAND_TARGET_PARAMETER_MB_CORE_AUTOSHUTTER_TEMP = 0x19,
	COMMAND_TARGET_PARAMETER_MB_CORE_AUTOSHUTTER_TIMER = 0x1A,
	COMMAND_TARGET_PARAMETER_MB_CORE_TRACK_REF_START = 0x1B,
	COMMAND_TARGET_PARAMETER_MB_CORE_LASER_EXTERAL_CONTROL = 0x1C,
	COMMAND_TARGET_PARAMETER_MB_CORE_DELAY_LASER_ON = 0x1D,
	COMMAND_TARGET_PARAMETER_MB_CORE_PREHEATING_ENA = 0x1E,
	COMMAND_TARGET_PARAMETER_MB_CORE_PREHEATING_TIME = 0x1F,
	COMMAND_TARGET_PARAMETER_MB_CORE_PREHEATING_POWER = 0x20,

	COMMAND_TARGET_PARAMETER_MB_CORE_START_TRACK_MOM_T = 0x06,
	COMMAND_TARGET_PARAMETER_MB_CORE_END_OF_TRACK = 0x08,
	COMMAND_TARGET_PARAMETER_MB_CORE_MODE = 0x0A,
	COMMAND_TARGET_PARAMETER_MB_CORE_REFERENCE_TRACK = 0x0B,
	COMMAND_TARGET_PARAMETER_MB_CORE_TIME_TRACK_LOW = 0x0C,

	COMMAND_TARGET_PARAMETER_MB_CORE_THRESHOLD = 0x08F,
	COMMAND_TARGET_PARAMETER_MB_CORE_ROI_ROUND = 0x090,
	COMMAND_TARGET_PARAMETER_MB_CORE_ENABLE_ROI = 0x091,
	COMMAND_TARGET_PARAMETER_MB_CORE_ROI_X1 = 0x012,
	COMMAND_TARGET_PARAMETER_MB_CORE_ROI_Y1 = 0x093,
	COMMAND_TARGET_PARAMETER_MB_CORE_ROI_X2 = 0x094,
	COMMAND_TARGET_PARAMETER_MB_CORE_ROI_Y2 = 0x095,

	COMMAND_TARGET_PARAMETER_MB_CORE_PWM = 0x97,
	COMMAND_TARGET_PARAMETER_MB_CORE_PWM_LIMIT_MAX = 0x9D,
	COMMAND_TARGET_PARAMETER_MB_CORE_PWM_LIMIT_MIN = 0x9E,
	COMMAND_TARGET_PARAMETER_MB_CORE_DIGITAL_OUT_CONF = 0xB0,
	COMMAND_TARGET_PARAMETER_MB_CORE_DIGITAL_OUT_0 = 0xB9,
	COMMAND_TARGET_PARAMETER_MB_CORE_DIGITAL_OUT_1 = 0xBA,
	COMMAND_TARGET_PARAMETER_MB_CORE_DIGITAL_OUT_2 = 0xBB,
	COMMAND_TARGET_PARAMETER_MB_CORE_DIGITAL_OUT_3 = 0xBC,

	COMMAND_TARGET_PARAMETER_ARM_CORE_SOFT_RST = 0x0D,
	COMMAND_TARGET_PARAMETER_ARM_CORE_IP_OP_MODE = 0x16,
	COMMAND_TARGET_PARAMETER_ARM_CORE_LED_R = 0x9A,
	COMMAND_TARGET_PARAMETER_ARM_CORE_LED_G = 0x9B,
	COMMAND_TARGET_PARAMETER_ARM_CORE_LED_B = 0x9C,

};

typedef struct
{
	unsigned int rw;
	unsigned int base;
	unsigned int offset;
} application_tcp_command_host_action_t;

struct __attribute__((packed)) tcp_protocol_header_struct
{
	uint8_t target_parameter : 7;
	uint8_t rw : 1;
	uint8_t reserved1 : 4;
	uint8_t target : 4;
};

typedef union tcp_protocol_header_union
{
	uint16_t val;
	struct tcp_protocol_header_struct obj;
} tcp_protocol_header_t;


int main()
{

    /*
     * Bucle de lectura
     */
    while ((nr >= 0) && (cont_tramas_vacias < 3) && (nw >= 0))
    {

        bzero(buffer, 4);

        nr = recv(newsockfd, buffer, sizeof(buffer), MSG_WAITALL);

        if (nr < 0)
        {
            printf("ERROR reading from socket");
        }
        if (nr <= 0)
        {
            cont_tramas_vacias++;
            // printf("nr: %d, contador: %d\n", nr, cont_tramas_vacias);
        }
        else
        {
            // printf("Recibido address-command 0x%x, data %d\n",buffer[0],buffer[1]);
            gest = command_manager(buffer[0]);

            if (gest.fpga_write == 0)
            {
                if (gest.rd > 127)
                { // lectura
                    if (gest.offset == SERIAL_NUMBER_LOW)
                    {
                        buffer[1] = (uint16_t)(process_variables->serial_number_low & 0x0000FFFF);
                        nw = write(newsockfd, buffer, 4);
                        buffer[1] = (uint16_t)((process_variables->serial_number_low & 0xFFFF0000) >> 16);
                        buffer[0] = 0x04AB;
                        nw = write(newsockfd, buffer, 4);
                        buffer[1] = (uint16_t)(process_variables->serial_number_high & 0x0000FFFF);
                        buffer[0] = 0x04AC;
                        nw = write(newsockfd, buffer, 4);
                        buffer[1] = (uint16_t)((process_variables->serial_number_high & 0xFFFF0000) >> 16);
                        buffer[0] = 0x04AD;
                        nw = write(newsockfd, buffer, 4);
                    }
                    else
                    {
                        // this is the problem how am I am going to get the process offset from a packed struct
                        buffer[1] = (uint16_t)(((int *)process_variables)[gest.offset] & 0x0000FFFF);
                        nw = write(newsockfd, buffer, 4);
                    }
                }
                else
                { // escritura de datos en memoria
                    switch (gest.offset)
                    {
                    case POWER_LIMIT_MAX:
                        ((int *)process_variables)[gest.offset] = buffer[1];
                        nit_mb_core_pwm_limit_max_set(mb_core_state, (unsigned int)((double)buffer[1] - (double)process_variables->min_power) * (16383 / ((double)process_variables->max_power - (double)process_variables->min_power)));
                        break;
                    case POWER_LIMIT_MIN:
                        ((int *)process_variables)[gest.offset] = buffer[1];
                        nit_mb_core_pwm_limit_min_set(mb_core_state, (unsigned int)((double)buffer[1] - (double)process_variables->min_power) * (16383 / ((double)process_variables->max_power - (double)process_variables->min_power)));
                        break;
                    case MAX_POWER:
                        ((int *)process_variables)[gest.offset] = buffer[1];
                        nit_mb_core_pwm_limit_max_set(mb_core_state, (unsigned int)((double)process_variables->power_limit_max - (int16_t)process_variables->min_power) * (16383 / ((double)process_variables->max_power - (int16_t)process_variables->min_power)));
                        nit_mb_core_pwm_limit_min_set(mb_core_state, (unsigned int)((double)process_variables->power_limit_min - (int16_t)process_variables->min_power) * (16383 / ((double)process_variables->max_power - (int16_t)process_variables->min_power)));
                        break;
                    case MIN_POWER:
                        ((int *)process_variables)[gest.offset] = (int16_t)buffer[1];
                        nit_mb_core_pwm_limit_max_set(mb_core_state, (unsigned int)((double)process_variables->power_limit_max - (int16_t)buffer[1]) * (16383 / ((double)process_variables->max_power - (int16_t)buffer[1])));
                        nit_mb_core_pwm_limit_min_set(mb_core_state, (unsigned int)((double)process_variables->power_limit_min - (int16_t)buffer[1]) * (16383 / ((double)process_variables->max_power - (int16_t)buffer[1])));
                        break;
                    default:
                        ((int *)process_variables)[gest.offset] = buffer[1];
                        break;
                    }
                }
            }
            else
            {
                switch (gest.base)
                {
                case NIT_CONTROL_UNIT_BASE_ADDRESS:
                    if (gest.rd > 127)
                    { // lectura
                        if (gest.offset == NIT_SINCRONIZATION)
                        {
                            buffer[1] = 1;
                            nw = write(newsockfd, buffer, 4); // Mero comando de sincronizacion, no se guarda en memoria
                        }
                        else if (gest.offset == NIT_ARM_SW_VERSION)
                        {
                            buffer[1] = (uint16_t)VERSION;
                            nw = write(newsockfd, buffer, 4);
                        }
                        else if ((gest.offset == NIT_FPGA_VERSION) && (buffer[0] == 0x05A6))
                        {
                            buffer[1] = (uint16_t)((((uint8_t *)control_unit_state->priv)[gest.offset] & 0xFFFF0000) >> 16);
                            nw = write(newsockfd, buffer, 4);
                        }
                        else
                        {
                            buffer[1] = (uint16_t)(((uint8_t *)control_unit_state->priv)[gest.offset] & 0x0000FFFF);
                            nw = write(newsockfd, buffer, 4);
                        }
                        // printf("Respondiendo NIT lectura address-command 0x%x, data %d\n",buffer[0],buffer[1]);
                    }
                    else
                    { // escritura de datos en memoria
                        if (gest.offset == NIT_SAVE_EMBEDDED_CONF)
                        {
                            if (config_save("/mnt/mmc/sys/CONFIG.sys", (volatile int *)process_variables, mb_core_state, control_unit_state) == 0)
                            {
                                printf("Couldn't save current configuration\n");
                            }
                        }
                        else
                        {
                            ((uint8_t *)control_unit_state->priv)[gest.offset] = buffer[1];
                        }
                    }
                    break;

                case NIT_MB_CORE_BASE_ADDRESS:
                    if (gest.rd > 127)
                    {
                        if (gest.offset != TIME_TRACK_LOW)
                        {
                            buffer[1] = (uint16_t)(((uint8_t *)mb_core_state->priv)[gest.offset] & 0x0000FFFF);
                            nw = write(newsockfd, buffer, 4);
                        }
                        else
                        {
                            time_track_aux = ((unsigned int)mb_core_state[TIME_TRACK_HIGH]);
                            time_track_aux = ((time_track_aux << 32) | ((unsigned int)mb_core_state[TIME_TRACK_LOW]));
                            buffer[1] = ((uint16_t)(((double)time_track_aux / CLK_100MHZ) * 10));
                            nw = write(newsockfd, buffer, 4);
                        }
                    }
                    else
                    {
                        switch (gest.offset)
                        {

                        case TIME_TRACK_LOW:
                            time_track_aux = (uint64_t)(((double)buffer[1]) * (CLK_100MHZ / 10));
                            ((uint8_t *)mb_core_state->priv)[gest.offset] = (unsigned int)(0x00000000FFFFFFFF & time_track_aux);
                            nit_mb_core_time_track_high_set(mb_core_state, (unsigned int)((0xFFFFFFFF00000000 & time_track_aux) >> 32));
                            break;

                        case MODE:
                            ((uint8_t *)mb_core_state->priv)[gest.offset] = buffer[1];
                            nit_mb_core_change_op_mode_set(mb_core_state, 1);
                            break;

                        default:
                            ((uint8_t *)mb_core_state->priv)[gest.offset] = buffer[1];
                            break;
                        }
                        // printf("escribiendo en  0x%x +  0x%x = 0x%x\n",gest.baseaddress,(gest.offset*4),gest.baseaddress+ (gest.offset*4));
                    }
                    break;
                case NIT_ARM_CORE_BASE_ADDRESS:
                    ((uint8_t *)arm_core_state->priv)[gest.offset] = buffer[1];
                    break;
                default:
                    printf("Error in ADDRESS identification :%d \n", gest.base);
                    break;
                }
            }
        }
    }
    if (cont_tramas_vacias == 3)
    {
        printf("Error en lectura, recibiendo tramas vacias\n");
    }
}

double metadata_get_width(framebuffer_metadata_t *metadata);


/*
 * Funcion de cáclculo de ancho, empleada por varios estados del automata de control
 */
double metadata_get_width(framebuffer_metadata_t *metadata)
{
	double W = 0;
	double X, Y;
	double u20, u11, u02;

	// Procesos de calculo
	if (metadata->M00 == 0)
		metadata->M00 = 1;
	X = metadata->M10 / metadata->M00;
	Y = metadata->M01 / metadata->M00;

	u20 = (metadata->M20 / metadata->M00) - (X * X);
	u11 = (metadata->M11 / metadata->M00) - (X * Y);
	u02 = (metadata->M02 / metadata->M00) - (Y * Y);

	W = sqrt(8 * (u20 + u02 - sqrt((4 * u11 * u11) + ((u20 - u02) * (u20 - u02))))); // Aqui se ha calculado el ancho
	return W;
}


/*
 * Funcián de escritura de imagenes desde el CLAMIR
 */
void application_tcp_socket_handler(int sock, application_state_t *state)
{
	int retval = 0;
	unsigned char frame_sync[] = {0x5f, 0x00, 0x48, 0xf6, 0x70, 0x44, 0x94, 0xee};

	int timer_fd = open("/dev/uio0", O_RDWR);

	int timer_ctrl = 1;
	int timer_status = 0;
	write(timer_fd, (void *)&timer_ctrl, sizeof(int));

	framebuffer_core_state_t framebuffer_core_state;
	framebuffer_core_open(&framebuffer_core_state);
	volatile uint16_t *buffer = framebuffer_get_memory_map(&framebuffer_core_state);

	const int shutter_enable_counter_max = 1000;
	int shutter_enable_counter = 0;

	control_unit_offset_update_set(control_unit_state, 1);

	while (retval >= 0)
	{

		read(timer_fd, (int *)&timer_status, sizeof(int));
		write(timer_fd, (void *)&timer_ctrl, sizeof(int));

		/**
		 * Enable shutter
		 */

		if (shutter_enable_counter-- > 0)
		{
			control_unit_shutter_set(control_unit_state, 0);
		}
		else
		{
			control_unit_shutter_set(control_unit_state, 1);
			shutter_enable_counter = shutter_enable_counter_max;
		}

		retval = write(sock, frame_sync, sizeof(frame_sync));

		metadata->frame_number++;

		if (retval < 0)
		{
			printf("ERROR writing image to socket (frame sync)\n");
			break;
		}

		retval = write(sock, (unsigned char *)metadata, sizeof(framebuffer_metadata_t));

		if (retval < 0)
		{
			printf("ERROR writing image to socket (header)\n");
			break;
		}

		retval = write(sock, (unsigned char *)buffer, NIT_FRAMEBUFFER_CORE_SIZE);
		if (retval < 0)
		{
			printf("ERROR writing image to socket (image)\n");
			break;
		}
	}

	framebuffer_core_close(&framebuffer_core_state);
}

int system_command_host_process_action(const application_tcp_command_host_action_t *action)
{
	return 0;
}

int application_tcp_command_host_buffer_dump(const unsigned char *buffer)
{
	return 0;
}

int application_tcp_command_host_action_dump(const application_tcp_command_host_action_t *action)
{
	return 0;
}

int application_tcp_command_host_decode_buffer(application_tcp_command_host_action_t *dest, const unsigned char *buffer)
{
	return 0;
}

void application_tcp_command_host(int newsockfd, control_unit_core_state_t *control_unit_state, mb_core_state_t *mb_core_state, arm_core_state_t *arm_core_state)
{

	uint64_t time_track_aux;
	int cont_tramas_vacias = 0;
	uint8_t buffer[4];
	application_tcp_command_host_action_t action;
	uint16_t bytes_read = 0;
	int result;

	while (true)
	{
		bytes_read = recv(newsockfd, buffer, sizeof(buffer), MSG_WAITALL);

		if (bytes_read < 0)
		{
			break;
		}

		if (bytes_read == 0)
		{
			continue;
		}

		result = application_tcp_command_host_decode_buffer(&action, buffer);

		if (result < 0)
		{
			printf("failed to decode TCP action %d", result);
			application_tcp_command_host_buffer_dump(buffer);
			application_tcp_command_host_action_dump(&action);
			continue;
		}

		system_command_host_process_action(&action);
	}
}

int application_config_save(application_state_t *state, const char *path)
{
	int result = 0;
	uint64_t time_track_aux = 0;
	FILE *fd;
	fd = fopen(path, "w");
	return result;
}

int application_initialize(application_state_t *state)
{

	signal(SIGPIPE, application_SIGPIPE_handler);

	{
		int retval = 0;

		retval = roi_core_open(&state->roi_core_state);
		roi_core_config_load_from_file(&state->roi_core_state, state->config.roi_core_config_path);

		if (retval < 0)
		{
			printf("Failed to open control unit core\n");
			exit(retval);
		}
	}

	{
		int retval = 0;

		retval = pwm_core_open(&state->pwm_core_state);
		pwm_core_config_load_from_file(&state->pwm_core_state, state->config.pwm_core_config_path);

		if (retval < 0)
		{
			printf("Failed to open control unit core\n");
			exit(retval);
		}
	}

	{
		int retval = 0;

		retval = mom_core_open(&state->mom_core_state);
		mom_core_config_load_from_file(&state->mom_core_state, state->config.mom_core_config_path);

		if (retval < 0)
		{
			printf("Failed to open control unit core\n");
			exit(retval);
		}
	}

	{
		int retval = 0;

		retval = gen_core_open(&state->gen_core_state);
		gen_core_config_load_from_file(&state->gen_core_state, state->config.gen_core_config_path);

		if (retval < 0)
		{
			printf("Failed to open control unit core\n");
			exit(retval);
		}
	}

	{
		int retval = 0;

		retval = framebuffer_core_open(&state->framebuffer_core_state);
		framebuffer_core_config_load_from_file(&state->framebuffer_core_state, state->config.framebuffer_core_config_path);

		if (retval < 0)
		{
			printf("Failed to open control unit core\n");
			exit(retval);
		}
	}

	{
		int retval = 0;

		retval = control_unit_core_open(&state->control_unit_core_state);
		control_unit_core_config_load_from_file(&state->control_unit_core_state, state->config.control_unit_core_config_path);

		if (retval < 0)
		{
			printf("Failed to open control unit core\n");
			exit(retval);
		}
	}

	{
		int retval = 0;

		retval = bpc_table_core_open(&state->bpc_table_core_state);
		bpc_table_core_config_load_from_file(&state->bpc_table_core_state, state->config.bpc_table_core_config_path);

		if (retval < 0)
		{
			printf("Failed to open control unit core\n");
			exit(retval);
		}
	}

	{
		int retval = 0;

		retval = arm_core_open(&state->arm_core_state);
		arm_core_config_load_from_file(&state->arm_core_state, state->config.arm_core_config_path);

		if (retval < 0)
		{
			printf("Failed to open control unit core\n");
			exit(retval);
		}
	}

	return 0;
}