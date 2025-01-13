
#ifndef APP_GSDML_H
#define APP_GSDML_H

#include <pnet_api.h>

#define APP_GSDML_API 0

#ifndef APP_GSDML_DEFAULT_STATION_NAME
#define APP_GSDML_DEFAULT_STATION_NAME "rt-labs-dev"
#endif

#ifndef APP_GSDML_VENDOR_ID
#define APP_GSDML_VENDOR_ID 0x0493
#endif

/* GSDML tag: DeviceID */
#ifndef APP_GSDML_DEVICE_ID
#define APP_GSDML_DEVICE_ID 0x0002
#endif

/* Used in DCP communication */
#ifndef APP_GSDML_OEM_VENDOR_ID
#define APP_GSDML_OEM_VENDOR_ID 0xcafe
#endif

#ifndef APP_GSDML_OEM_DEVICE_ID
#define APP_GSDML_OEM_DEVICE_ID 0xee02
#endif

/* Used in I&M0 */
#ifndef APP_GSDML_IM_HARDWARE_REVISION
#define APP_GSDML_IM_HARDWARE_REVISION 3
#endif

#ifndef APP_GSDML_IM_VERSION_MAJOR
#define APP_GSDML_IM_VERSION_MAJOR 1
#endif

#ifndef APP_GSDML_IM_VERSION_MINOR
#define APP_GSDML_IM_VERSION_MINOR 2
#endif

/* Allowed: 'V', 'R', 'P', 'U', 'T' */
#ifndef APP_GSDML_SW_REV_PREFIX
#define APP_GSDML_SW_REV_PREFIX 'V'
#endif

#ifndef APP_GSDML_PROFILE_ID
#define APP_GSDML_PROFILE_ID 0x1234
#endif

#ifndef APP_GSDML_PROFILE_SPEC_TYPE
#define APP_GSDML_PROFILE_SPEC_TYPE 0x5678
#endif

#ifndef APP_GSDML_IM_REVISION_COUNTER
#define APP_GSDML_IM_REVISION_COUNTER 0 /* Typically 0 */
#endif

/* Note: You need to read out the actual hardware serial number instead */
#ifndef APP_GSDML_EXAMPLE_SERIAL_NUMBER
#define APP_GSDML_EXAMPLE_SERIAL_NUMBER "007"
#endif

/* Initial values. Can be overwritten by PLC */
#ifndef APP_GSDML_TAG_FUNCTION
#define APP_GSDML_TAG_FUNCTION "my function"
#endif

#ifndef APP_GSDML_TAG_LOCATION
#define APP_GSDML_TAG_LOCATION "my location"
#endif

#ifndef APP_GSDML_IM_DATE
#define APP_GSDML_IM_DATE "2022-03-01 10:03"
#endif

#ifndef APP_GSDML_DESCRIPTOR
#define APP_GSDML_DESCRIPTOR "my descriptor"
#endif

#ifndef APP_GSDML_SIGNATURE
#define APP_GSDML_SIGNATURE ""
#endif

/* GSDML tag: Writeable_IM_Records */
#define APP_GSDML_IM_SUPPORTED \
   (PNET_SUPPORTED_IM1 | PNET_SUPPORTED_IM2 | PNET_SUPPORTED_IM3)

/* GSDML tag: OrderNumber */
#ifndef APP_GSDML_ORDER_ID
#define APP_GSDML_ORDER_ID "12345 Abcdefghijk"
#endif

/* GSDML tag: ModuleInfo / Name */
#ifndef APP_GSDML_PRODUCT_NAME
#define APP_GSDML_PRODUCT_NAME "P-Net Sample Application"
#endif

/* GSDML tag: MinDeviceInterval */
#ifndef APP_GSDML_MIN_DEVICE_INTERVAL
#define APP_GSDML_MIN_DEVICE_INTERVAL 32 /* 1 ms */
#endif

#ifndef APP_GSDML_DIAG_CUSTOM_USI
#define APP_GSDML_DIAG_CUSTOM_USI 0x1234
#endif

/* See "Specification for GSDML" 8.26 LogBookEntryItem for allowed values */
#ifndef APP_GSDML_LOGBOOK_ERROR_CODE
#define APP_GSDML_LOGBOOK_ERROR_CODE 0x20   /* Manufacturer specific */
#endif

#ifndef APP_GSDML_LOGBOOK_ERROR_DECODE
#define APP_GSDML_LOGBOOK_ERROR_DECODE 0x82 /* Manufacturer specific */
#endif

#ifndef APP_GSDML_LOGBOOK_ERROR_CODE_1
#define APP_GSDML_LOGBOOK_ERROR_CODE_1 PNET_ERROR_CODE_1_FSPM
#endif

#ifndef APP_GSDML_LOGBOOK_ERROR_CODE_2
#define APP_GSDML_LOGBOOK_ERROR_CODE_2 0x00       /* Manufacturer specific */
#endif

#ifndef APP_GSDML_LOGBOOK_ENTRY_DETAIL
#define APP_GSDML_LOGBOOK_ENTRY_DETAIL 0xFEE1DEAD /* Manufacturer specific */
#endif

/* Use same size for all parameters in example */
#ifndef APP_GSDML_PARAMETER_LENGTH
#define APP_GSDML_PARAMETER_LENGTH 4
#endif

#ifndef APP_GSDML_DEFAULT_MAUTYPE
#define APP_GSDML_DEFAULT_MAUTYPE 0x10 /* Copper 100 Mbit/s Full duplex */
#endif

#ifndef APP_GSDML_ALARM_PAYLOAD_SIZE
#define APP_GSDML_ALARM_PAYLOAD_SIZE 1 /* bytes */
#endif

/**
 * @file
 * @brief Device properties defined by the GSDML device definition
 *
 * Functions for getting module, submodule and parameter
 * configurations using their ids.
 *
 * Important:
 * Any change in this file may require an update of the GSDML file.
 * Note that when the GSDML file is updated it has to be reloaded
 * in your Profinet engineering tool. PLC applications may be affected.
 *
 * Design requires unique submodule IDs and unique parameter indexes.
 */
#ifdef __cplusplus
extern "C"
{
#endif

   typedef struct app_gsdml_module
   {
      uint32_t id;

      /** Module name */
      const char *name;

      /** Submodule IDs. Variable length, ends with 0. */
      uint32_t submodules[];
   } app_gsdml_module_t;

   typedef int (*app_gsdml_submodule_value_setter)(void* arg, void* value, uint16_t size);
   typedef int (*app_gsdml_submodule_value_getter)(void* arg, void* value, uint16_t* size);

   typedef struct app_gsdml_submodule
   {
      uint32_t id;

      /** Submodule name */
      const char *name;

      uint32_t api;
      pnet_submodule_dir_t data_dir;
      uint16_t insize;
      uint16_t outsize;

      uint8_t* container;

      app_gsdml_submodule_value_setter set;
      app_gsdml_submodule_value_getter get;

      /**
       * Parameter indexes. See app_gsdml_parameters.
       * Variable length, ends with 0.
       **/
      uint16_t parameters[];
   } app_gsdml_submodule_t;

   typedef int (*app_gsdml_param_value_setter)(void* parameter, void* value, uint16_t size);
   typedef int (*app_gsdml_param_value_getter)(void* parameter, void** value, uint16_t* size);

   typedef struct
   {
      uint32_t index;
      const char *name;
      uint16_t length;
      uint8_t* container;
      app_gsdml_param_value_setter set;
      app_gsdml_param_value_getter get;
      
   } app_gsdml_param_t;

   /**
    * Get module configuration from module ID
    * @param module_id  In: Module ID
    * @return Module configuration, NULL if not found
    */
   const app_gsdml_module_t *app_gsdml_get_module_cfg(uint32_t module_id);

   /**
    * Get submodule module configuration from submodule ID
    * @param submodule_id  In: Submodule ID
    * @return Submodule configuration, NULL if not found
    */
   const app_gsdml_submodule_t *app_gsdml_get_submodule_cfg(uint32_t submodule_id);

   /**
    * Get parameter configuration from parameter index
    * @param submodule_id  In: Submodule ID
    * @param index         In: Parameters index
    * @return Parameter configuration, NULL if not found
    */
   const app_gsdml_param_t *app_gsdml_get_parameter_cfg(uint32_t submodule_id, uint32_t index);

   extern app_gsdml_module_t *app_gsdml_modules[];
   extern app_gsdml_submodule_t *app_gsdml_submodules[];
   extern app_gsdml_param_t* app_gsdml_parameters[];

   int app_gsdml_default_param_set(void* arg, void *value, uint16_t size);
   int app_gsdml_default_param_get(void* arg, void **value, uint16_t *size);

#ifdef __cplusplus
}
#endif

#endif /* APP_GSDML_H */
