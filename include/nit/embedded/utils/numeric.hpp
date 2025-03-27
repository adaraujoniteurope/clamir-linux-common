#ifndef _UTILS_NUMERIC_HPP_
#define _UTILS_NUMERIC_HPP_

#include <float.h>

namespace utils::numeric
{

    namespace traits
    {
        template<typename type> 
        struct zero
        {
            static const type value;
        };

        template<>
        struct zero<double>
        {
            static constexpr const double value = 0.0;
        };

        template<typename type>
        struct max
        {
            static const type value;
        };

        template<typename type>
        struct min
        {
            static const type value;
        };

        template<>
        struct max<double>
        {
            static constexpr const double value = DBL_MAX;
        };

        template<>
        struct min<double>
        {
            static constexpr const double value = -DBL_MAX;
        };

        template<typename type>
        struct is_pointer
        {
            static const type value = false;
        };

        template<typename type>
        struct is_pointer<type*>
        {
            static const type value = true;
        };
    }
}

#endif