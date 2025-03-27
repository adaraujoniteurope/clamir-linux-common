#ifndef UTILS_POLLABLE_HPP_
#define UTILS_POLLABLE_HPP_

namespace utils
{
    class pollable
    {
    public:
        virtual ~pollable() {
            std::cout << __func__ << std::endl;
        }
        virtual void poll() = 0;
    };
};

#endif