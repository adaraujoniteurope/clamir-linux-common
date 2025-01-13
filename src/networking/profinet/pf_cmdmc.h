#ifndef PF_CMDMC_H
#define PF_CMDMC_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ?
 * @param p_ar             InOut: The AR instance.
 * @return  0  if operation succeeded.
 *          -1 if an error occurred.
 */
int pf_cmdmc_activate_req (pf_ar_t * p_ar);

/**
 * ?
 * @param p_ar             InOut: The AR instance.
 * @return  0  if operation succeeded.
 *          -1 if an error occurred.
 */
int pf_cmdmc_close_req (pf_ar_t * p_ar);

/**
 * ?
 * @param p_ar             InOut: The AR instance.
 * @param ix               In:    ?
 * @param start            In:    ?
 * @return  0  if operation succeeded.
 *          -1 if an error occurred.
 */
int pf_cmdmc_cpm_state_ind (pf_ar_t * p_ar, uint16_t ix, bool start);

#ifdef __cplusplus
}
#endif

#endif /* PF_CMDMC_H */
