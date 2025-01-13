
#ifndef PF_CMRPC_HELPERS_H
#define PF_CMRPC_HELPERS_H

#ifdef __cplusplus
extern "C" {
#endif

/************ Internal functions, made available for unit testing ************/

void pf_generate_uuid (
   uint32_t timestamp,
   uint32_t session_number,
   pnet_ethaddr_t mac_address,
   pf_uuid_t * p_uuid);

#ifdef __cplusplus
}
#endif

#endif /* PF_CMRPC_HELPERS_H */
