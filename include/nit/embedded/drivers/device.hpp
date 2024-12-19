#ifndef _NIT_EMBEDDED_DRIVERS_DEVICE_HPP_
#define _NIT_EMBEDDED_DRIVERS_DEVICE_HPP_

namespace nit::embedded::drivers
{
    class device {
        public:
        virtual void open() = 0;
        virtual void close() = 0;
    };
}

#endif