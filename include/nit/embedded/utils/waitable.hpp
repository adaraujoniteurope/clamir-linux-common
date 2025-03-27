#ifndef _UTILS_WAITABLE_HPP_
#define _UTILS_WAITABLE_HPP_

namespace utils
{
    class waitable
    {
        public:
        virtual void wait() = 0;
    };
}

#endif