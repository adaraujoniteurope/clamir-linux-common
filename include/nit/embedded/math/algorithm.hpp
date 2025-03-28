#ifndef _MATH_ALGORITHM_HPP_
#define _MATH_ALGORITHM_HPP_

#include <cmath>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <string>

namespace math::algorithm
{
    namespace moments
    {

        template<typename buffer_type>
        constexpr buffer_type moments(buffer_type* buffer, const size_t cols, const size_t rows, size_t i, size_t j)
        {
            buffer_type m_ij = 0;
            for (size_t r = 0; r < rows; r++)
            {
                for (size_t c = 0; c < cols; c++)
                {
                    m_ij += (double) buffer[r * cols + c] * ::pow(c,i) * ::pow(c,j);
                }
            }

            return m_ij;
        }

        template<typename type>
        constexpr std::tuple<type,type> centroid_2d(type m00, type m10, type m01) {
            
            if (m00 == 0) {
                m00 = 1;
            }

            return { m10/m00, m01/m00 };
        }

        template<typename type>
        constexpr type width_2d(type m00, type m01, type m10, type m11, type m02, type m20) {

            if (m00 == 0) {
                m00 = 1;
            }

            auto [ x, y ] = centroid_2d(m00, m10, m01);

            auto u20 = (m20 / m00) - (x * y);
            auto u11 = (m11 / m00) - (x * y);
            auto u02 = (m02 / m00) - (x * y);

            return ::sqrt(8 * (u20 + u02 - ::sqrt((4 * u11 * u11) + ((u20 - u02) * (u20 - u02)))));

        }
    }
}

#endif