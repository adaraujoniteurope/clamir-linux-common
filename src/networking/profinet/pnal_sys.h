
#ifndef PNAL_SYS_H
#define PNAL_SYS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "osal.h"

#include <netinet/in.h>

#define PNAL_BUF_MAX_SIZE 1522

typedef struct os_buf
{
   void * payload;
   uint16_t len;
} pnal_buf_t;

#ifdef __cplusplus
}
#endif

#endif /* PNAL_SYS_H */
