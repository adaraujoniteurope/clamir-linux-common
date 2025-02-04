#include <gtest/gtest.h>

#include <math/control.hpp>

#include <utils/time.hpp>
#include <utils/numeric.hpp>

using namespace math::control;

using namespace utils::time;
using namespace utils::numeric;

class rc_system : public abstract_system<double>
{
    public:
    double R = 10.0e3;
    double C = 10.0e-6;

    protected:
    virtual void poll() override
    {

        m_last = m_now;
        m_now = std::chrono::high_resolution_clock::now() - m_start;
        m_diff = m_now - m_last;

        m_last_secs = m_now_secs;
        m_now_secs = to_seconds_double(m_now);
        m_diff_secs = m_now_secs - m_last_secs;

        auto y = src.get();
        auto u = sink.get();
        auto dt = m_diff_secs;

        src.set(u + ( y - u ) * exp( -dt / ( R * C ) ));

    }

    private:
    std::chrono::high_resolution_clock::time_point m_start = std::chrono::high_resolution_clock::now();
    std::chrono::nanoseconds m_last = std::chrono::nanoseconds(0);
    std::chrono::nanoseconds m_now = std::chrono::nanoseconds(0);
    std::chrono::nanoseconds m_diff = std::chrono::nanoseconds(0);

    double m_now_secs;
    double m_last_secs;
    double m_diff_secs;
    
};

class pid_controller_test : public testing::Test {
    protected:

    pid_controller_test() {
        // You can do set-up work for each test here.

        controller.kp_set(10.0);
        controller.ki_set(50.0);
        controller.kd_set(0.001);
        controller.set_point_set(0.000);
    
        controller.control_output_max_set(10);
        controller.control_output_min_set(-10);
    
        controller.error_max_set(20);
        controller.error_min_set(-20);
    
        controller.error_diff_max_set(20);
        controller.error_diff_min_set(-20);
    
        controller.error_sum_max_set(20);
        controller.error_sum_min_set(-20);

     }
   
     ~pid_controller_test() override {
        // You can do clean-up work that doesn't throw exceptions here.

        controller_shutdown = true;
        if (controller_thread.joinable()) {
            controller_thread.join();
        }
    
        controller_timer_shutdown = true;
        if (controller_timer_thread.joinable()) {
            controller_timer_thread.join();
        }
    
        system_stub_shutdown = true;
        if (system_stub_thread.joinable()) {
            system_stub_thread.join();
        }
    
        system_stub_timer_shutdown = true;
        if (system_stub_timer_thread.joinable()) {
            system_stub_timer_thread.join();
        }

     }

     void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
     }
   
     void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
     }

    double R = 10.0e3;
    double C = 10.0e-6;

    std::atomic_bool system_stub_shutdown = false;
    std::atomic_bool system_stub_timer_shutdown = false;
    
    linux_rtc_timer system_stub_timer = linux_rtc_timer(1000, system_stub_timer_shutdown);
    std::thread system_stub_timer_thread = std::thread(system_stub_timer.get_worker());
    
    rc_system system_stub = rc_system();
    std::thread system_stub_thread = std::thread(system_stub.get_worker(system_stub_shutdown, system_stub_timer));
    
    std::atomic_bool controller_shutdown = false;
    std::atomic_bool controller_timer_shutdown = false;
    
    linux_rtc_timer controller_timer = linux_rtc_timer(1000, controller_timer_shutdown);
    std::thread controller_timer_thread = std::thread(controller_timer.get_worker());
    
    pid_controller<double> controller = pid_controller<double>();
    std::thread controller_thread = std::thread(controller.get_worker(controller_shutdown, controller_timer));
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    std::chrono::seconds experiment_duration = std::chrono::seconds(5);

};

TEST_F(pid_controller_test, open_loop_system) {

    std::ofstream ofs;
    ofs.open("basic_pid_controller_open_loop_system.csv");

    ASSERT_TRUE(false);
}

TEST_F(pid_controller_test, parameter_set_kp) {

    controller.kp_set(1.0);
    ASSERT_EQ(controller.kp_get(),1.0);

    controller.kp_set(-1.0);
    ASSERT_EQ(controller.kp_get(),1.0);

    controller.kp_set(-0.0);
    ASSERT_EQ(controller.kp_get(),0.0);

    /**
     * NOTE: Using Ubuntu Version 22.04 and GCC 13.3.0 I've found
     * that the numeric limit for double is wrong.
     * 
     * This test will fail.
     * 
     * When GNU compiler collection fix the issue it will pass.
     */
    controller.kp_set(traits::min<double>::value);
    ASSERT_EQ(controller.kp_get(),0.0);

    controller.reset();
}

TEST_F(pid_controller_test, parameter_set_kp_max) {
    ASSERT_TRUE(false);
}

TEST_F(pid_controller_test, parameter_set_kp_min) {
    ASSERT_TRUE(false);
}

TEST_F(pid_controller_test, parameter_set_ki) {
    controller.ki_set(1.0);
    ASSERT_EQ(controller.ki_get(),1.0);

    controller.ki_set(-1.0);
    ASSERT_EQ(controller.ki_get(),1.0);

    controller.ki_set(-0.0);
    ASSERT_EQ(controller.ki_get(),0.0);

    /**
     * NOTE: Using Ubuntu Version 22.04 and GCC 13.3.0 I've found
     * that the numeric limit for double is wrong.
     * 
     * This test will fail.
     * 
     * When GNU compiler collection fix the issue it will pass.
     */
    controller.ki_set(traits::min<double>::value);
    ASSERT_EQ(controller.ki_get(),0.0);

    controller.reset();
}

TEST_F(pid_controller_test, parameter_set_ki_max) {
    ASSERT_TRUE(false);
}

TEST_F(pid_controller_test, parameter_set_ki_min) {
    ASSERT_TRUE(false);
}

TEST_F(pid_controller_test, parameter_set_kd) {
    controller.kd_set(1.0);
    ASSERT_EQ(controller.kd_get(),1.0);

    controller.kd_set(-1.0);
    ASSERT_EQ(controller.kd_get(),1.0);

    controller.kd_set(-0.0);
    ASSERT_EQ(controller.kd_get(),0.0);

    /**
     * NOTE: Using Ubuntu Version 22.04 and GCC 13.3.0 I've found
     * that the numeric limit for double is wrong.
     * 
     * This test will fail.
     * 
     * When GNU compiler collection fix the issue it will pass.
     */
    controller.kd_set(traits::min<double>::value);
    ASSERT_EQ(controller.kd_get(),0.0);

    controller.reset();
}

TEST_F(pid_controller_test, parameter_set_kd_max) {
    /**
     * not implemented
     */
    ASSERT_TRUE(false);
}

TEST_F(pid_controller_test, parameter_set_kd_min) {
    /**
     * not implemented
     */
    ASSERT_TRUE(false);
}

TEST_F(pid_controller_test, parameter_set_error_limits) {

    controller.error_max_set(1.0);
    controller.error_min_set(-1.0);
    ASSERT_EQ(controller.error_min_get(), -1.0);
    ASSERT_EQ(controller.error_max_get(), 1.0);

    controller.error_max_set(1.0);
    controller.error_max_set(-1.0);
    ASSERT_EQ(controller.error_max_get(), 1.0);

    controller.error_min_set(-1.0);
    controller.error_min_set(1.0);
    ASSERT_EQ(controller.error_min_get(), -1.0);

    controller.reset();
}

TEST_F(pid_controller_test, closed_loop_system) {
    
    std::ofstream ofs;
    ofs.open("basic_pid_controller_closed_loop_system.csv");

    ofs
        << "time (s)"
        << "," << "set point"
        << "," << "controller input"
        << "," << "controller error"
        << "," << "controller error (integral)"
        << "," << "controller error (derivative)"
        << "," << "controller output"
        << std::endl;

    controller.reset();
    controller.set_point_set(0.000);

    controller.kp_set(10.0);
    controller.ki_set(50.0);
    controller.kd_set(0.001);

    controller.control_output_max_set(10);
    controller.control_output_min_set(-10);

    controller.error_max_set(20);
    controller.error_min_set(-20);

    controller.error_diff_max_set(20);
    controller.error_diff_min_set(-20);

    controller.error_sum_max_set(20);
    controller.error_sum_min_set(-20);

    double set_point = 0.0;

    for(auto t = std::chrono::high_resolution_clock::now() - start; t < experiment_duration; t = std::chrono::high_resolution_clock::now() - start)
    {
        
        if (t > std::chrono::milliseconds(1000) && t < std::chrono::milliseconds(2000)) {
            set_point = 1.0;
        }

        if (t > std::chrono::milliseconds(2000) && t < std::chrono::milliseconds(3000)) {
            set_point = 2.0;
        }

        if (t > std::chrono::milliseconds(3000) && t < std::chrono::milliseconds(4000)) {
            set_point -= 0.001;
        }

        if (t > std::chrono::milliseconds(4000) && t < std::chrono::milliseconds(5000)) {
            set_point += 0.001;
        }

        controller.set_point_set(set_point);

        ofs
            << to_seconds_double(t)
            << "," << controller.set_point_get()
            << "," << controller.input_get()
            << "," << controller.error_get()
            << "," << controller.error_sum_get()
            << "," << controller.error_diff_get()
            << "," << controller.output_get()
            << std::endl;

        /**
         * Control system assertions
         */
        ASSERT_LE(system_stub.src.get(), set_point * (1.5));

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    ofs.close();

}

TEST_F(pid_controller_test, closed_loop_proportional_saturation_protection) {
    std::ofstream ofs;
    ofs.open("basic_pid_controller_closed_loop_proportional_saturation_protection.csv");
    ofs.close();
    /**
     * not implemented
     */
    ASSERT_TRUE(false);
}

TEST_F(pid_controller_test, closed_loop_integral_saturation_protection) {

    std::ofstream ofs;
    ofs.open("basic_pid_controller_closed_loop_integral_saturation_protection.csv");
    ofs.close();
    /**
     * not implemented
     */
    ASSERT_TRUE(false);
}

TEST_F(pid_controller_test, closed_loop_derivative_saturation_protection) {

    std::ofstream ofs;
    ofs.open("basic_pid_controller_closed_loop_derivative_saturation_protection.csv");
    ofs.close();
    /**
     * not implemented
     */
    ASSERT_TRUE(false);
}

TEST_F(pid_controller_test, closed_loop_output_disable) {


    std::ofstream ofs;
    ofs.open("basic_pid_controller_closed_loop_output_disable.csv");
    ofs.close();
    /**
     * not implemented
     */
    ASSERT_TRUE(false);
}