
/**
 * @file
 * @brief Driver interface
 */

#ifndef PF_DRIVER_H
#define PF_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pf_includes.h"

/**
 * Initialize hardware offload driver
 * Called during P-Net initialization if option
 * PNET_OPTION_DRIVER_ENABLE is enabled.
 * @param net              InOut: The p-net stack instance
 * @return  0  on success
 *          -1 on error
 */
int pf_driver_init (pnet_t * net);

/**
 * Install hardware offload ppm driver
 * Called during P-Net initialization if option
 * PNET_OPTION_DRIVER_ENABLE is enabled.
 *
 * @return  0  on success
 *          -1 on error
 */
int pf_driver_ppm_init (pnet_t * net);

/**
 * Install hardware offload cpm driver
 * Called during P-Net initialization if option
 * PNET_OPTION_DRIVER_ENABLE is enabled.
 *
 * @return  0  on success
 *          -1 on error
 */
int pf_driver_cpm_init (pnet_t * net);

#ifdef __cplusplus
}
#endif

#endif /* PF_DRIVER_H*/
