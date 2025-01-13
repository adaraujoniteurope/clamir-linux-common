
#include "pnal.h"
#include "pf_includes.h"

#include <string.h>
#include <unistd.h>

int pnal_udp_open (pnal_ipaddr_t addr, pnal_ipport_t port)
{
   struct sockaddr_in local;
   int id;
   const int enable = 1;

   id = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (id == -1)
   {
      return -1;
   }

   if (setsockopt(id, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) != 0)
   {
      goto error;
   }

   /* set IP and port number */
   local = (struct sockaddr_in){
      .sin_family = AF_INET,
      .sin_addr.s_addr = htonl (addr),
      .sin_port = htons (port),
      .sin_zero = {0},
   };

   if (bind (id, (struct sockaddr *)&local, sizeof (local)) != 0)
   {
      goto error;
   }

   return id;

error:
   close (id);
   return -1;
}

int pnal_udp_sendto (
   uint32_t id,
   pnal_ipaddr_t dst_addr,
   pnal_ipport_t dst_port,
   const uint8_t * data,
   int size)
{
   struct sockaddr_in remote;
   int len;

   remote = (struct sockaddr_in){
      .sin_family = AF_INET,
      .sin_addr.s_addr = htonl (dst_addr),
      .sin_port = htons (dst_port),
      .sin_zero = {0},
   };
   len =
      sendto (id, data, size, 0, (struct sockaddr *)&remote, sizeof (remote));

   return len;
}

int pnal_udp_recvfrom (
   uint32_t id,
   pnal_ipaddr_t * src_addr,
   pnal_ipport_t * src_port,
   uint8_t * data,
   int size)
{
   struct sockaddr_in remote;
   socklen_t addr_len = sizeof (remote);
   int len;

   memset (&remote, 0, sizeof (remote));
   len = recvfrom (
      id,
      data,
      size,
      MSG_DONTWAIT,
      (struct sockaddr *)&remote,
      &addr_len);
   if (len > 0)
   {
      *src_addr = ntohl (remote.sin_addr.s_addr);
      *src_port = ntohs (remote.sin_port);
   }

   return len;
}

void pnal_udp_close (uint32_t id)
{
   close (id);
}
