#ifndef _nit_embedded_core_
#define _nit_embedded_core_

#include <cstddef>
#include <cstdlib>

#include <nit/embedded/networking/control_interface.hpp>

namespace nit::embedded::networking {
    class websockets : public control_interface  {
        public:
        virtual void open();
        virtual void close();
    };
};

#endif