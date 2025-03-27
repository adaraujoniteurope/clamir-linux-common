#include <nit/embedded/utils/memory_map.hpp>
#include <sys/mman.h>
#include <cstdlib>

#ifdef NIT_CLAMIR_HOST_MOCKUP
namespace utils
{
    void* memory_map_open(void* __addr, size_t __len, int __prot, int __flags, int __fd, size_t __offset)
    {
        return malloc(__len);
    }

    void memory_map_close(void* __addr, size_t __len)
    {
        free(__addr);
        return;
    }
}
#else
namespace utils
{
    void* memory_map_open(void* __addr, size_t __len, int __prot, int __flags, int __fd, size_t __offset)
    {
        return ::mmap(__addr, __len, __prot, __flags, __fd, __offset);
    }

    void memory_map_close(void* __addr, size_t __len)
    {
        munmap(__addr, __len);
        return;
    }
}

#endif