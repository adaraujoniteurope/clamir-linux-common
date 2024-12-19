#ifndef _nit_embedded_networking_stream_inteface_
#define _nit_embedded_networking_stream_inteface_

#include <cstddef>
#include <cstdlib>

namespace nit::embedded::core {
    class stream_interface {
        public:
        virtual void open();
        virtual void close();
    };

};
#endif