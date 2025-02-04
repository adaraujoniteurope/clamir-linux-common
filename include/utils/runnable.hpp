#ifndef UTILS_RUNNABLE_HPP_
#define UTILS_RUNNABLE_HPP_

namespace utils
{
    class runnable
    {
    public:
        virtual ~runnable() { std::cout << __func__ << std::endl; }
        virtual void run() = 0;
    };
};

#endif