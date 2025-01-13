/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2021 rt-labs AB, Sweden.
 *
 * This software is dual-licensed under GPLv3 and a commercial
 * license. See the file LICENSE.md distributed with this software for
 * full license information.
 ********************************************************************/

#include "app_data.h"
#include "app_utils.h"
#include "app_gsdml.h"
#include "app_log.h"
#include "sampleapp_common.h"
#include "osal.h"
#include "pnal.h"
#include <pnet_api.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "drivers/arm_core.h"
#include "drivers/bpc_table_core.h"
#include "drivers/gen_core.h"
#include "drivers/mb_core.h"
#include "drivers/pwm_core.h"
#include "drivers/mom_core.h"
#include "drivers/pwm_core.h"
#include "drivers/roi_core.h"

#define APP_DATA_DEFAULT_OUTPUT_DATA 0

/**
 * Set LED state.
 *
 * Compares new state with previous state, to minimize system calls.
 *
 * Uses the hardware specific app_set_led() function.
 *
 * @param led_state        In:    New LED state
 */
static void app_handle_data_led_state(bool led_state)
{
   static bool previous_led_state = false;
   APP_LOG_DEBUG("%s\n", __func__);
   if (led_state != previous_led_state)
   {
      app_set_led(APP_DATA_LED_ID, led_state);
   }
   previous_led_state = led_state;
}

uint8_t *app_data_get_input_data
(
    uint16_t slot_nbr,
    uint16_t subslot_nbr,
    uint32_t submodule_id,
    uint16_t *size,
    uint8_t *iops)
{
   /**
    * 
    * It's not trivial and not typical
    * normally we get the size of the data
    * and then allocate it.
    * 
    * But here I don't want do allocate nothing
    * dynamically so the input data pointer will
    * be filled by the getter callback from the
    * submodule itself.
    * 
    * if no data is retrieved from the accesor
    * it will return a failure.
    * 
    */
   uint8_t* inputdata;

   APP_LOG_DEBUG("%s\n", __func__);

   if (size == NULL || iops == NULL)
   {
      return NULL;
   }

   const app_gsdml_submodule_t* submodule = app_gsdml_get_submodule_cfg(submodule_id);

   if (submodule == NULL)
   {
      return NULL;
   }

   if (submodule->get == NULL)
   {
      *size = -1;
      *iops = PNET_IOXS_BAD;
      return NULL;
   }

   int result = submodule->get((void*)submodule, inputdata, size);

   if (result < 0)
   {
      /**
       * Here is the protocol in the beginning of the function.
       */
      *size = -1;
      *iops = PNET_IOXS_BAD;
      return NULL;
   }

   *iops = PNET_IOXS_GOOD;
   return inputdata;
}

int app_data_set_output_data(
    uint16_t slot_nbr,
    uint16_t subslot_nbr,
    uint32_t submodule_id,
    uint8_t *data,
    uint16_t size)
{
   bool led_state;
   APP_LOG_DEBUG("%s\n", __func__);

   if (data == NULL)
   {
      return -1;
   }

   const app_gsdml_submodule_t* submodule = app_gsdml_get_submodule_cfg(submodule_id);

   if (submodule == NULL)
   {
      return -2;
   }

   if (submodule->set != NULL)
   {
      submodule->set((void*) submodule, data, size);
   }
}

int app_data_set_default_outputs(void)
{
   APP_LOG_DEBUG("%s\n", __func__);
   return 0;
}

int app_data_write_parameter(
    uint16_t slot_nbr,
    uint16_t subslot_nbr,
    uint32_t submodule_id,
    uint32_t index,
    const uint8_t *data,
    uint16_t length)
{
   const app_gsdml_param_t *par_cfg;

   APP_LOG_DEBUG("%s\n", __func__);

   par_cfg = app_gsdml_get_parameter_cfg(submodule_id, index);
   if (par_cfg == NULL)
   {
      APP_LOG_WARNING(
          "PLC write request unsupported submodule/parameter. "
          "Submodule id: %u Index: %u\n",
          (unsigned)submodule_id,
          (unsigned)index);
      return -1;
   }

   if (length != par_cfg->length)
   {
      APP_LOG_WARNING(
          "PLC write request unsupported length. "
          "Index: %u Length: %u Expected length: %u\n",
          (unsigned)index,
          (unsigned)length,
          par_cfg->length);
      return -1;
   }

   if (par_cfg->set == NULL)
   {

      APP_LOG_WARNING(
         "PLC write request not implemented. "
         "Index: %u Length: %u Expected Length: %u\n",
         (unsigned) index,
         (unsigned) length,
         par_cfg->length
      );

      return -1;
   }
   
   APP_LOG_DEBUG("  Writing parameter \"%s\"\n", par_cfg->name);
   app_log_print_bytes(APP_LOG_LEVEL_DEBUG, data, length);

   int result = par_cfg->set((void*)par_cfg, (void*) data, length);

   if (result < 0) {
      APP_LOG_ERROR(
         "Failed to write parameter \"%s\"\n",
         (unsigned) par_cfg->name
      );
   }

   return 0;
}

int app_data_read_parameter(
    uint16_t slot_nbr,
    uint16_t subslot_nbr,
    uint32_t submodule_id,
    uint32_t index,
    uint8_t **data,
    uint16_t* length)
{
   const app_gsdml_param_t *par_cfg;
   APP_LOG_DEBUG("%s\n", __func__);

   par_cfg = app_gsdml_get_parameter_cfg(submodule_id, index);
   if (par_cfg == NULL)
   {
      APP_LOG_WARNING(
          "PLC read request unsupported submodule/parameter. "
          "Submodule id: %u Index: %u\n",
          (unsigned)submodule_id,
          (unsigned)index);
      return -1;
   }

   if (*length < par_cfg->length)
   {
      APP_LOG_WARNING(
          "PLC read request unsupported length. "
          "Index: %u Max length: %u Data length for our parameter: %u\n",
          (unsigned)index,
          (unsigned)*length,
          par_cfg->length);
      return -1;
   }

   if (par_cfg->get == NULL)
   {
      APP_LOG_ERROR(
         "Failed to get parameter %s getter not implemented\n",
         (unsigned) par_cfg->name
      );
      return -1;
   }

   APP_LOG_DEBUG(
      "Getting parameter %s\n",
      par_cfg->name
   );

   int result = par_cfg->get((void*)par_cfg, (void**) data, length);

   if (result < 0 || data == NULL || length == NULL)
   {
      APP_LOG_ERROR(
         "Failed to get parameter %s with code %d",
         par_cfg->name,
         par_cfg->index
      );

      return -1;
   }

   app_log_print_bytes(APP_LOG_LEVEL_DEBUG, *data, *length);

   return 0;
}
