
/**
 * @file
 * @brief Helper functions for CMRPC
 *
 * Located in a separate file to enabling mocking during tests.
 *
 */

#include "pf_includes.h"

/**
 * @internal
 * Generate an UUID, inspired by version 1, variant 1
 *
 * See RFC 4122
 *
 * @param timestamp        In:   Some kind of timestamp
 * @param session_number   In:   Session number
 * @param mac_address      In:   MAC address
 * @param p_uuid           Out:  The generated UUID
 */
void pf_generate_uuid (
   uint32_t timestamp,
   uint32_t session_number,
   pnet_ethaddr_t mac_address,
   pf_uuid_t * p_uuid)
{
   /* TODO Maybe we should take pointer to mac address instead, to avoid
           copying? */
   p_uuid->data1 = timestamp;
   p_uuid->data2 = session_number >> 16;
   p_uuid->data3 = (session_number >> 8) & 0x00ff;
   p_uuid->data3 |= 1 << 12; /* UUID version 1 */
   p_uuid->data4[0] = 0x80;  /* UUID variant 1 */
   p_uuid->data4[1] = session_number & 0xff;
   p_uuid->data4[2] = mac_address.addr[0];
   p_uuid->data4[3] = mac_address.addr[1];
   p_uuid->data4[4] = mac_address.addr[2];
   p_uuid->data4[5] = mac_address.addr[3];
   p_uuid->data4[6] = mac_address.addr[4];
   p_uuid->data4[7] = mac_address.addr[5];
}
