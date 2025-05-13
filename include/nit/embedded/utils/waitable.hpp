#ifndef _UTILS_WAITABLE_HPP_
#define _UTILS_WAITABLE_HPP_

namespace nit::embedded::utils
{
    class waitable
    {
        public:
        virtual void wait() = 0;
    };
}

#endif