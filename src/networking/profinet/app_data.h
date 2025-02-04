
#ifndef APP_DATA_H
#define APP_DATA_H

/**
 * @file
 * @brief Sample application data interface
 *
 * Functions for:
 * - Getting input data (Button 1 and counter value)
 * - Setting output data (LED 1)
 * - Setting default output state. This should be
 *   part of all device implementations for setting
 *   defined state when device is not connected to PLC
 * - Reading and writing parameters
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include <rtlabs/p-net/pnet_api.h>

#include "rtlabs/osal/osal.h"
#include "pnal.h"

#include "app_utils.h"
#include "app_gsdml.h"
#include "app_data.h"
#include "app_log.h"
#include "rtlabs/osal/osal.h"
#include "pnal.h"

#define APP_TICK_INTERVAL_US 1000 /* 1 ms */

/* Thread configuration for targets where sample
 * event loop is run in a separate thread (not main).
 * This applies for linux sample app implementation.
 */
#define APP_MAIN_THREAD_PRIORITY 15
#define APP_MAIN_THREAD_STACKSIZE 4096 /* bytes */

#define APP_DATA_LED_ID 1
#define APP_PROFINET_SIGNAL_LED_ID 2

#define APP_TICKS_READ_BUTTONS 10
#define APP_TICKS_UPDATE_DATA 100

    /** HW Offload configuration. */
    typedef enum
    {
        MODE_HW_OFFLOAD_NONE = 0,
        MODE_HW_OFFLOAD_CPU,
        MODE_HW_OFFLOAD_FULL,
    } app_mode_t;

    /** Command line arguments for sample application */
    typedef struct app_args
    {
        char path_storage_directory[PNET_MAX_DIRECTORYPATH_SIZE]; /** Terminated */
        char station_name[PNET_STATION_NAME_MAX_SIZE];            /** Terminated string */
        char eth_interfaces
            [PNET_INTERFACE_NAME_MAX_SIZE * (PNET_MAX_PHYSICAL_PORTS + 1) +
             PNET_MAX_PHYSICAL_PORTS]; /** Terminated string */
        int verbosity;
        int show;
        bool factory_reset;
        bool remove_files;
        app_mode_t mode;
    } app_args_t;

    typedef enum
    {
        RUN_IN_SEPARATE_THREAD,
        RUN_IN_MAIN_THREAD
    } app_run_in_separate_task_t;

    typedef struct app_data_t
    {
        pnet_t *net;

        /* P-Net configuration passed in app_init(). */
        const pnet_cfg_t *pnet_cfg;

        /* Application API for administration of plugged
         * (sub)modules and connection state. */
        app_api_t main_api;

        os_timer_t *main_timer;
        os_event_t *main_events;

        void* pdata;

        bool alarm_allowed;
        pnet_alarm_argument_t alarm_arg;
        uint8_t alarm_payload[APP_GSDML_ALARM_PAYLOAD_SIZE];

        /* Counters used to control when buttons are checked
         * and process data is updated */
        uint32_t process_data_tick_counter;

    } app_data_t;

    /**
     * Get application specific PNIO input data (for sending to PLC)
     *
     * The main sample application keeps track
     * of button so it is a parameter to this function.
     *
     * This function is not called for the DAP submodules (slot_nbr==0).
     *
     * @param slot_nbr      In:  Slot number
     * @param subslot_nbr   In:  Subslot number
     * @param submodule_id  In:  Submodule id
     * @param button_state  In:  State of button 1
     * @param size          Out: Size of pnio data.
     *                           Not modified on error.
     * @param iops          Out: Provider status. If for example
     *                           a sensor is failing or a short
     *                           circuit is detected on digital
     *                           input this shall be set to BAD.
     *                           Not modified on error.
     * @return Reference to PNIO data, NULL on error
     */
    int app_data_get_input_data(
        const app_data_t* app,
        const app_subslot_t* subslot,
        uint8_t* input_data,
        uint16_t *size,
        uint8_t *iops
    );

    /**
     * Set application specific PNIO output data (received from PLC)
     *
     * This function is not called for the DAP submodules (slot_nbr==0).
     *
     * @param slot_nbr      In:  Slot number
     * @param subslot_nbr   In:  Subslot number
     * @param submodule_id  In:  Submodule id
     * @param data          In:  Reference to output data
     * @param size          In:  Length of output data
     * @return 0 on success, -1 on error
     */
    int app_data_set_output_data(
        const app_data_t* app,
        const app_subslot_t* subslot,
        // uint16_t slot_nbr,
        // uint16_t subslot_nbr,
        // uint32_t submodule_id,
        uint8_t *data,
        uint16_t size);

    /**
     * Set default outputs for all subslots.
     *
     * For the sample application this means that
     * LED 1 is turned off.
     *
     * @return 0 on success, -1 on error
     */
    int app_data_set_default_outputs(void);

    /**
     * Write parameter index for a subslot
     *
     * @param slot_nbr      In:  Slot number
     * @param subslot_nbr   In:  Subslot number
     * @param submodule_id  In:  Submodule id
     * @param index         In:  Parameter index
     * @param data          In:  New parameter value
     * @param write_length  In:  Length of parameter data
     * @return 0 on success, -1 on error
     */
    int app_data_write_parameter(
        uint16_t slot_nbr,
        uint16_t subslot_nbr,
        uint32_t submodule_id,
        uint32_t index,
        const uint8_t *data,
        uint16_t write_length);

    /**
     * Read parameter index from a subslot
     *
     * @param slot_nbr      In:    Slot number
     * @param subslot_nbr   In:    Subslot number
     * @param submodule_id  In:    Submodule id
     * @param index         In:    Parameter index
     * @param data          In:    Reference to parameter data
     * @param length        InOut: The maximum (in) and actual (out) length in
     *                             bytes of the data.
     * @return 0 on success, -1 on error
     */
    int app_data_read_parameter(
        uint16_t slot_nbr,
        uint16_t subslot_nbr,
        uint32_t submodule_id,
        uint32_t index,
        uint8_t **data,
        uint16_t *length);

    /**
     * AR specific event handler type.
     *
     * Handles an AR specific event.
     *
     * @param app          InOut: Application handle
     * @param arep         In:    Arep of the AR.
     *
     * @return 0 to indicate that the arep should be kept
     *         1 to indicate that the arep should be forgotten
     */
    typedef int (*app_ar_event_handler_t)(app_data_t *app, uint32_t arep);

    /** Partially initialise config values, and use proper callbacks
     *
     * @param pnet_cfg     Out:   Configuration to be updated
     */
    void app_pnet_cfg_init_default(pnet_cfg_t *pnet_cfg);

    /**
     * Initialize P-Net stack and application.
     *
     * The \a pnet_cfg argument shall have been initialized using
     * \a app_pnet_cfg_init_default() before this function is
     * called.
     *
     * @param pnet_cfg               In:    P-Net configuration
     * @param app_args               In:    Application arguments
     * @return Application handle, NULL on error
     */
    app_data_t *app_init(const pnet_cfg_t *pnet_cfg, const app_args_t *app_args);

    /**
     * Start application main loop
     *
     * Application must have been initialized using \a app_init() before
     * this function is called.
     *
     * If \a task_config parameters is set to RUN_IN_SEPARATE_THREAD a
     * thread execution the \a app_loop_forever() function is started.
     * If task_config is set to RUN_IN_MAIN_THREAD no such thread is
     * started and the caller must call the \a app_loop_forever() after
     * calling this function.
     *
     * RUN_IN_MAIN_THREAD is intended for rt-kernel targets.
     * RUN_IN_SEPARATE_THREAD is intended for linux targets.
     *
     * @param app                 In:    Application handle
     * @param task_config         In:    Defines if stack and application
     *                                   is run in main or separate task.
     * @return 0 on success, -1 on error
     */
    int app_start(app_data_t *app, app_run_in_separate_task_t task_config);

    /**
     * Application task definition. Handles events in eternal loop.
     *
     * @param arg                 In: Application handle
     */
    void app_loop_forever(void *arg);

    /**
     * Get P-Net instance from application
     *
     * @param app                 In:    Application handle
     * @return P-Net instance, NULL on failure
     */
    pnet_t *app_get_pnet_instance(app_data_t *app);

    /**
     * Set LED state
     * Hardware specific. Implemented in sample app main file for
     * each supported platform.
     *
     * @param id               In:    LED number, starting from 0.
     * @param led_state        In:    LED state. Use true for on and false for off.
     */
    void app_set_led(uint16_t id, bool led_state);

    /**
     * Read button state
     *
     * Hardware specific. Implemented in sample app main file for
     * each supported platform.
     *
     * @param id               In:    Button number, starting from 0.
     * @return  true if button is pressed, false if not
     */
    bool app_get_button(uint16_t id);

#ifdef __cplusplus
}
#endif

#endif /* APP_DATA_H */
