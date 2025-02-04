#ifndef _UTILS_TIME_HPP_
#define _UTILS_TIME_HPP_

namespace utils::time
{
    template<typename duration_type>
    double to_seconds_double(const duration_type& d)
    {
        return (double) d.count() / (double) duration_type::period::den;
    }
}

#endif