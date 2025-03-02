#ifndef _MATH_CONTROL_H_
#define _MATH_CONTROL_H_

#include <atomic>
#include <chrono>
#include <thread>
#include <deque>
#include <limits>

#include <ostream>
#include <fstream>
#include <cmath>

#include <utils/waitable.hpp>
#include <components/timer.hpp>
#include <utils/pollable_worker.hpp>
#include <utils/numeric.hpp>
#include <utils/signal.hpp>

using namespace utils::numeric;

namespace math::control
{

    template<typename value_type>
    class pid_controller : public utils::pollable_worker
    {
    public:

        const value_type& max = traits::max<value_type>::value;
        const value_type& min = traits::min<value_type>::value;
        const value_type& zero = traits::zero<value_type>::value;

        utils::signal<value_type> output_changed;

        virtual ~pid_controller() {
            std::cout << __func__ << std::endl;
        }

        pid_controller()
        {
        }

        virtual void reset()
        {
            m_kp = zero;
            m_kd = zero;
            m_ki = zero;
            m_set_point = zero;
            m_control_output = zero;
        }

        virtual void hold()
        {
            /** not implemented */
        }

        virtual void setup()
        {
            m_set_point = zero;
            m_error = zero;
            m_error_last = zero;
            m_error_sum = zero;
            m_error_diff = zero;
            m_kp = zero;
            m_kd = zero;
            m_ki = zero;
            m_control_output = zero;
            m_interval = 1;

            m_error_max = std::numeric_limits<value_type>::max();
            m_error_min = std::numeric_limits<value_type>::min();

            m_error_diff_max = std::numeric_limits<value_type>::max();
            m_error_diff_min = std::numeric_limits<value_type>::min();

            m_error_sum_max = std::numeric_limits<value_type>::max();
            m_error_sum_min = std::numeric_limits<value_type>::min();

            m_control_output_max = std::numeric_limits<value_type>::max();
            m_control_output_min = std::numeric_limits<value_type>::min();

            m_control_output_diff_max = std::numeric_limits<value_type>::max();
        }

        virtual void stop()
        {
        }

        virtual void interval_set(value_type value) { m_interval = value; }
        virtual value_type interval_get() { return m_interval; }

        virtual void poll() override
        {

            /** compute interval */
            m_start = m_now;
            m_now = std::chrono::high_resolution_clock::now();
            m_interval = (value_type)((value_type)(m_now - m_start).count() / (value_type)std::chrono::nanoseconds::period::den);

            m_error_last = m_error;
            m_error = m_set_point - m_feedback;

            if (m_error > m_error_max)
                m_error = m_error_max;
            if (m_error < m_error_min)
                m_error = m_error_min;

            m_error_sum += m_ki * m_error * m_interval;

            if (m_error_sum > m_error_sum_max)
                m_error_sum = m_error_sum_max;
            if (m_error_sum < m_error_sum_min)
                m_error_sum = m_error_sum_min;

            m_error_diff = m_kd * (m_error - m_error_last) / m_interval;

            if (m_error_diff > m_error_diff_max)
                m_error_diff = m_error_diff_max;
            if (m_error_diff < m_error_diff_min)
                m_error_diff = m_error_diff_min;

            auto m_control_output_next = m_set_point + (m_error * m_kp) + m_error_sum + m_error_diff;

            auto control_output_diff = (m_control_output_next - m_control_output)/m_interval;

            if (abs(control_output_diff) > m_control_output_diff_max)
            {
                
                if (control_output_diff < 0) {
                    control_output_diff = -1*m_control_output_diff_max;
                } else {
                    control_output_diff = m_control_output_diff_max;
                }

            }

            m_control_output = control_output_diff * m_interval + m_control_output;

            if (m_control_output > m_control_output_max)
                m_control_output = m_control_output_max;
            if (m_control_output < m_control_output_min)
                m_control_output = m_control_output_min;

            // m_output = m_control_output;
            output_set(m_control_output);
        }

        constexpr value_type error_last_get() { return m_error_last; }
        constexpr void error_last_set(value_type value) { m_error_last = value; }

        constexpr value_type error_diff_get() { return m_error_diff; }
        constexpr void error_diff_set(value_type value)
        {

            if (m_error_diff == value)
            {
                return;
            }
            if (m_error_diff > m_error_diff_max)
            {
                m_error_diff = m_error_diff_max;
                return;
            }
            if (m_error_diff < m_error_diff_min)
            {
                m_error_diff = m_error_diff_min;
                return;
            }
            m_error_diff = value;
        }

        constexpr value_type error_sum_get() { return m_error_sum; }

        constexpr void error_sum_set(value_type value)
        {
            if (m_error_sum == value)
            {
                return;
            }
            if (m_error_sum > m_error_sum_max)
            {
                m_error_sum = m_error_sum_max;
                return;
            }
            if (m_error_sum < m_error_sum_min)
            {
                m_error_sum = m_error_sum_min;
                return;
            }
            m_error_sum = value;
        }

        constexpr value_type error_get() { return m_error; }

        constexpr void error_set(value_type value)
        {
            if (m_error == value)
            {
                return;
            }
            if (m_error > m_error_max)
            {
                m_error = m_error_max;
                return;
            }
            if (m_error < m_error_min)
            {
                m_error = m_error_min;
                return;
            }
            m_error = value;
        }

        constexpr value_type kp_get() { return m_kp; }
        constexpr void kp_set(value_type value) {

            if (m_kp != value && value >= zero) {
                m_kp = value;
            }

            if (value == -zero) {
                m_kp = zero;
            }

        }

        constexpr value_type ki_get() { return m_ki; }
        constexpr void ki_set(value_type value) {

            if (m_ki != value && value >= zero) {
                m_ki = value;
            }

            if (value == -zero) {
                m_kp = zero;
            }

        }

        constexpr value_type kd_get() { return m_kd; }
        constexpr void kd_set(value_type value) {

            if (m_kd != value && value >= zero) {
                m_kd = value;
            }

            if (value == -zero) {
                m_kp = zero;
            }
            
        }
        value_type feedback_get() { return m_feedback; }
        void feedback_set(const value_type value) { m_feedback = value; }
        value_type output_get() { return m_output; }


        constexpr void set_point_set(value_type value)
        {
            if (value != m_set_point)
            {
                m_set_point = value;
            }
        }
        constexpr value_type set_point_get() { return m_set_point; }

        constexpr value_type control_output_get() { return m_control_output; }
        constexpr void control_output_set(value_type value) { m_control_output = value; }

        constexpr value_type control_output_diff_max_get() { return m_control_output_diff_max; }
        constexpr void control_output_diff_max_set(value_type value) { m_control_output_diff_max = abs(value); }

        constexpr value_type control_output_max_get() { return m_control_output_max; }
        constexpr void control_output_max_set(value_type value) { m_control_output_max = value; }

        constexpr value_type control_output_min_get() { return m_control_output_min; }
        constexpr void control_output_min_set(value_type value) { m_control_output_min = value; }

        constexpr value_type error_max_get() { return m_error_max; }

        constexpr void error_max_set(value_type value)
        {
            if (m_error_min != value && value > zero)
            {
                m_error_max = value;
            }
        }

        constexpr value_type error_min_get() { return m_error_min; }

        constexpr void error_min_set(value_type value)
        {
            if (m_error_min != value && value < zero)
            {
                m_error_min = value;
            }
        }

        constexpr value_type error_sum_max_get() { return m_error_sum_max; }

        constexpr void error_sum_max_set(value_type value)
        {
            if (m_error_sum_max != value && value > zero)
            {
                m_error_sum_max = value;
            }
        }

        constexpr value_type error_sum_min_get() { return m_error_sum_min; }

        constexpr void error_sum_min_set(value_type value)
        {
            if (m_error_sum_min != value && value < zero)
            {
                m_error_sum_min = value;
            }
        }

        constexpr value_type error_diff_max_get() { return m_error_diff_max; }

        constexpr void error_diff_max_set(value_type value)
        {
            if (m_error_diff_max != value && value > zero)
            {
                m_error_diff_max = value;
            }
        }

        constexpr value_type error_diff_min_get() { return m_error_diff_min; }

        constexpr void error_diff_min_set(value_type value)
        {
            if (m_error_diff_min != value && value < zero)
            {
                m_error_diff_min = value;
            }
        }

    protected:
        virtual constexpr void output_set(value_type& value) {
            if (m_output != value) {
                m_output = value;
                output_changed.emit(m_output);
            }
        }

    private:

        std::chrono::high_resolution_clock::time_point m_start = std::chrono::high_resolution_clock::now();
        std::chrono::high_resolution_clock::time_point m_now = std::chrono::high_resolution_clock::now();

        value_type m_feedback;
        value_type m_output;

        value_type m_set_point;

        value_type m_error;
        value_type m_error_max;
        value_type m_error_min;

        value_type m_error_last;

        value_type m_error_sum;
        value_type m_error_sum_max;
        value_type m_error_sum_min;

        value_type m_error_diff;
        value_type m_error_diff_max;
        value_type m_error_diff_min;

        value_type m_component_proportional;
        value_type m_component_proportional_max;
        value_type m_component_proportional_min;

        value_type m_component_integral;
        value_type m_component_integral_max;
        value_type m_component_integral_min;

        value_type m_component_derivative;
        value_type m_component_derivative_max;
        value_type m_component_derivative_min;

        value_type m_kp;
        value_type m_ki;
        value_type m_kd;

        value_type m_control_output;
        value_type m_control_output_diff_max;
        value_type m_control_output_max;
        value_type m_control_output_min;

        value_type m_interval;
    };

}

#endif