
#ifndef PF_CPM_DRIVER_SW_H
#define PF_CPM_DRIVER_SW_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize p-net default cpm driver.
 * @param net              InOut: The p-net stack instance
 */
void pf_cpm_driver_sw_init (pnet_t * net);

#ifdef __cplusplus
}
#endif

#endif /* PF_CPM_DRIVER_SW_H */
