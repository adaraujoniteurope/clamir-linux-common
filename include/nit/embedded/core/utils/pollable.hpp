#ifndef _NIT_EMBEDED_CORE_POLLABLE_HPP_
#define _NIT_EMBEDED_CORE_POLLABLE_HPP_

namespace nit::embedded::core
{
    class pollable {
        public:
        virtual void poll() = 0;
    };
}

#endif