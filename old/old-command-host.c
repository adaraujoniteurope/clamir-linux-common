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