#ifndef _nit_embedded_utils_memory_map_hpp_
#define _nit_embedded_utils_memory_map_hpp_

#include <cstddef>

namespace utils
{
    void *memory_map_open(void* __addr, size_t __len, int __prot, int __flags, int __fd, size_t __offset);
    void memory_map_close(void* map, size_t length);
}

#endif