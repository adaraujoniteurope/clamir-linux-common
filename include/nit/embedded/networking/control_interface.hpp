#ifndef _nit_embedded_core_
#define _nit_embedded_core_

#include <cstddef>
#include <cstdlib>

#include <nit/embedded/core/utils/pollable.hpp>

namespace nit::embedded::core {
    class control_interface : public pollable {
        public:
        virtual void open() = 0;
        virtual void close() = 0;
        virtual void poll() = 0;
    };
};
#endif