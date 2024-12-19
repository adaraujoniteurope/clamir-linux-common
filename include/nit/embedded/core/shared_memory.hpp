#ifndef _nit_embedded_core_
#define _nit_embedded_core_

#include <cstddef>
#include <cstdlib>

namespace nit::embedded::core {
    class shared_memory {
        public:
        shared_memory(size_t addr, size_t size);
        virtual void open() = 0;
        virtual void close() = 0;
    };
};
#endif