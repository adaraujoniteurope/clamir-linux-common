#include "networking/profinet/device.hpp"

#include <pnet_api.h>
#include "app_data.h"

#include "app_data.h"
#include "app_gsdml.h"
#include "app_log.h"
#include "app_utils.h"

#include "osal.h"
#include "osal_log.h" /* For LOG_LEVEL */
#include "pnal.h"
#include "pnal_filetools.h"
#include <pnet_api.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#if PNET_MAX_PHYSICAL_PORTS == 1
#define APP_DEFAULT_ETHERNET_INTERFACE "end0"
#else
#define APP_DEFAULT_ETHERNET_INTERFACE "br0,eth0,eth1,end0"
#endif

#define APP_MAIN_SLEEPTIME_US 5000 * 1000
#define APP_SNMP_THREAD_PRIORITY 1
#define APP_SNMP_THREAD_STACKSIZE 256 * 1024 /* bytes */
#define APP_ETH_THREAD_PRIORITY 10
#define APP_ETH_THREAD_STACKSIZE 4096 /* bytes */
#define APP_BG_WORKER_THREAD_PRIORITY 5
#define APP_BG_WORKER_THREAD_STACKSIZE 4096 /* bytes */

static int app_pnet_cfg_init_storage(pnet_cfg_t *p_cfg, const app_args_t *p_args);

app_args_t app_args = {0};
/**
 * TODO:
 * 
 * 1. Use shutdown flag to stop application
 * 2. The callbacks of the application should point
 *      to custom local implementations to grab the
 *      application exposed parameters.
 * 3. It's fucked up to do this bridge but it is not
 *    impossible. It will work like a charm!
 */
device::handler_type device::create(const char *hwaddr, void* pdata, std::atomic_bool &shutdown)
{
    return [&]()
    {
        int ret;
        int32_t app_log_level = APP_LOG_LEVEL_FATAL;
        pnet_cfg_t pnet_cfg = {0};
        app_data_t *sample_app = NULL;
        app_utils_netif_namelist_t netif_name_list;
        pnet_if_cfg_t netif_cfg = {0};
        uint16_t number_of_ports = 1;

        /* Enable line buffering for printouts, especially when logging to
           the journal (which is default when running as a systemd job) */
        setvbuf(stdout, NULL, _IOLBF, 0);

        strcpy(app_args.path_storage_directory, "/var/cache/clamir/p-net");
        strcpy(app_args.station_name, APP_GSDML_DEFAULT_STATION_NAME);
        strcpy(app_args.eth_interfaces, APP_DEFAULT_ETHERNET_INTERFACE);

        app_args.verbosity = 7;
        app_args.show = 0;
        app_args.factory_reset = false;
        app_args.remove_files = false;
        app_args.mode = MODE_HW_OFFLOAD_NONE;

        app_args.factory_reset = false;
        app_args.remove_files = false;

        // app_log_level = (app_args.verbosity <= APP_LOG_LEVEL_FATAL) ? APP_LOG_LEVEL_FATAL - app_args.verbosity : APP_LOG_LEVEL_DEBUG;
        app_log_level = APP_LOG_LEVEL_DEBUG;
        app_log_set_log_level(app_log_level);

        printf("\n** Starting P-Net sample application " PNET_VERSION " **\n");

        APP_LOG_INFO("Number of slots: u (incl slot for DAP module)\n", PNET_MAX_SLOTS);
        APP_LOG_INFO("P-net log level:      %u (DEBUG=0, FATAL=4)\n", LOG_LEVEL);
        APP_LOG_INFO("App log level:        %u (DEBUG=0, FATAL=4)\n", app_log_level);
        APP_LOG_INFO("Max number of ports:  %u\n", PNET_MAX_PHYSICAL_PORTS);
        APP_LOG_INFO("Network interfaces:   %s\n", app_args.eth_interfaces);
        APP_LOG_INFO("Default station name: %s\n", app_args.station_name);

        /* Prepare configuration */
        app_pnet_cfg_init_default(&pnet_cfg);
        strcpy(pnet_cfg.station_name, app_args.station_name);
        ret = app_utils_pnet_cfg_init_netifs("end0", &netif_name_list, &number_of_ports, &netif_cfg);

        if (ret != 0)
        {
            // exit(EXIT_FAILURE);
            return;
        }
        pnet_cfg.if_cfg = netif_cfg;
        pnet_cfg.num_physical_ports = number_of_ports;

        app_utils_print_network_config(&netif_cfg, number_of_ports);

        /* Operating system specific settings */
        pnet_cfg.pnal_cfg.snmp_thread.prio = APP_SNMP_THREAD_PRIORITY;
        pnet_cfg.pnal_cfg.snmp_thread.stack_size = APP_SNMP_THREAD_STACKSIZE;
        pnet_cfg.pnal_cfg.eth_recv_thread.prio = APP_ETH_THREAD_PRIORITY;
        pnet_cfg.pnal_cfg.eth_recv_thread.stack_size = APP_ETH_THREAD_STACKSIZE;
        pnet_cfg.pnal_cfg.bg_worker_thread.prio = APP_BG_WORKER_THREAD_PRIORITY;
        pnet_cfg.pnal_cfg.bg_worker_thread.stack_size = APP_BG_WORKER_THREAD_STACKSIZE;

        ret = app_pnet_cfg_init_storage(&pnet_cfg, &app_args);
        if (ret != 0)
        {
            printf("Failed to initialize storage.\n");
            printf("Aborting application\n");
            exit(EXIT_FAILURE);
        }

        /* Remove files and exit */
        if (app_args.remove_files == true)
        {
            printf("\nRemoving stored files\n");
            printf("Exit application\n");
            (void)pnet_remove_data_files(pnet_cfg.file_directory);
            return;
        }

        /* Initialise stack and application */
        sample_app = app_init(&pnet_cfg, &app_args);


        if (sample_app == NULL)
        {
            printf("Failed to initialize P-Net.\n");
            printf("Do you have enough Ethernet interface permission?\n");
            printf("Aborting application\n");
            // exit(EXIT_FAILURE);
            return;
        }

        /* Do factory reset and exit */
        if (app_args.factory_reset == true)
        {
            printf("\nPerforming factory reset\n");
            printf("Exit application\n");
            (void)pnet_factory_reset(app_get_pnet_instance(sample_app));
            return;
        }

        /* Show stack info and exit */
        if (app_args.show > 0)
        {
            int level = 0xFFFF;

            printf("\nShowing stack information.\n\n");
            if (app_args.show == 1)
            {
                level = 0x2010; /* See documentation for pnet_show() */
            }

            pnet_show(app_get_pnet_instance(sample_app), level);
            printf("Exit application\n");
            return;
        }

        /* Start main loop */
        if (app_start(sample_app, RUN_IN_SEPARATE_THREAD) != 0)
        {
            printf("Failed to start\n");
            printf("Aborting application\n");
            return;
        }

        while(!shutdown.load())
        {
            os_usleep(APP_MAIN_SLEEPTIME_US);
        }
    };
}

/** Update configuration with file storage path.
 *  Validate this path, and Linux button file paths
 *
 * @param p_cfg      InOut: Configuration to be updated
 * @param p_args     In:    Command line arguments
 * @return 0 on success, -1 on error.
 */
static int app_pnet_cfg_init_storage(pnet_cfg_t *p_cfg, const app_args_t *p_args)
{
    strcpy(p_cfg->file_directory, p_args->path_storage_directory);

    if (p_args->verbosity > 0)
    {
        printf("Storage directory: %s\n\n", p_cfg->file_directory);
    }

    /* Validate paths */
    if (!pnal_does_file_exist(p_cfg->file_directory))
    {
        printf("Error: The given storage directory does not exist: %s\n", p_cfg->file_directory);
        return -1;
    }
    return 0;
}