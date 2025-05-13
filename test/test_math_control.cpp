#include <gtest/gtest.h>

#include <math/control.hpp>

#include <utils/time.hpp>
#include <utils/numeric.hpp>

using namespace math::control;

using namespace nit::embedded::utils::time;
using namespace nit::embedded::utils::numeric;

class dynamic_system : public utils::pollable_worker
{
    public:
    double R = 10.0e3;
    double C = 10.0e-6;

    utils::signal<double> output_changed;

    void input_set(double input) { u = input; }
    double input_get() { return u; }
    double output_get() { return y; }

    protected:
    virtual void poll() override
    {

        m_last = m_now;
        m_now = std::chrono::high_resolution_clock::now() - m_start;
        m_diff = m_now - m_last;

        m_last_secs = m_now_secs;
        m_now_secs = to_seconds_double(m_now);
        m_diff_secs = m_now_secs - m_last_secs;

        auto dt = m_diff_secs;

        y = u + ( y - u ) * exp( -dt / ( R * C ) );
        output_changed.emit(y);
    }

    private:
    std::chrono::high_resolution_clock::time_point m_start = std::chrono::high_resolution_clock::now();
    std::chrono::nanoseconds m_last = std::chrono::nanoseconds(0);
    std::chrono::nanoseconds m_now = std::chrono::nanoseconds(0);
    std::chrono::nanoseconds m_diff = std::chrono::nanoseconds(0);

    double m_now_secs;
    double m_last_secs;
    double m_diff_secs;

    double y;
    double u;
    
};

class pid_controller_test : public testing::Test {
    protected:

    pid_controller_test() {
        // You can do set-up work for each test here.

        controller.kp_set(0.0);
        controller.ki_set(0.0);
        controller.kd_set(0.000);
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
    
    dynamic_system system_stub = dynamic_system();
    std::thread system_stub_thread = std::thread(system_stub.get_worker(system_stub_shutdown, system_stub_timer));
    
    std::atomic_bool controller_shutdown = false;
    std::atomic_bool controller_timer_shutdown = false;
    
    linux_rtc_timer controller_timer = linux_rtc_timer(1000, controller_timer_shutdown);
    std::thread controller_timer_thread = std::thread(controller_timer.get_worker());
    
    pid_controller<double> controller = pid_controller<double>();
    std::thread controller_thread = std::thread(controller.get_worker(controller_shutdown, controller_timer));
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    std::chrono::seconds experiment_duration = std::chrono::seconds(10);

};

TEST_F(pid_controller_test, open_loop_system) {
    std::ofstream ofs;
    ofs.open("basic_pid_controller_open_loop_system.csv");

    system_stub.input_set(1.0);

    ofs
    << "time (s)"
    << ",%s", "input"
    << ",%s", "output"
    << std::endl;

    for(auto t = std::chrono::high_resolution_clock::now() - start; t < experiment_duration; t = std::chrono::high_resolution_clock::now() - start)
    {
        
        if (t > std::chrono::milliseconds(1000) && t < std::chrono::milliseconds(2000)) {
            system_stub.input_set(0.0);
        }

        if (t > std::chrono::milliseconds(2000) && t < std::chrono::milliseconds(3000)) {
            system_stub.input_set(1.0);
        }

        if (t > std::chrono::milliseconds(3000) && t < std::chrono::milliseconds(4000)) {
            system_stub.input_set(0.0);
        }

        if (t > std::chrono::milliseconds(4000) && t < std::chrono::milliseconds(5000)) {
            system_stub.input_set(1.0);
        }

        ofs
            << to_seconds_double(t)
            << ",%s", system_stub.input_get()
            << ",%s", system_stub.output_get()
            << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    ofs.close();


    ASSERT_TRUE(true);
}

TEST_F(pid_controller_test, closed_loop_system) {

    std::ofstream ofs;

    controller.output_changed += [&](auto value) {
        system_stub.input_set(value);
    };

    system_stub.output_changed += [&](auto value) {
        controller.feedback_set(value);
    };

    controller.kp_set(5.0);
    controller.ki_set(5.0);
    controller.kd_set(0.0);

    ofs.open("basic_pid_controller_closed_loop_system.csv");

    ofs
        << "time (s)"
        << ",%s", "controller input"
        << ",%s", "system output"
        << std::endl;

    for(auto t = std::chrono::high_resolution_clock::now() - start; t < experiment_duration; t = std::chrono::high_resolution_clock::now() - start)
    {
        
        if (t > std::chrono::milliseconds(1000) && t < std::chrono::milliseconds(2000)) {
            controller.set_point_set(0.0);
        }

        if (t > std::chrono::milliseconds(2000) && t < std::chrono::milliseconds(3000)) {
            controller.set_point_set(1.0);
        }

        if (t > std::chrono::milliseconds(3000) && t < std::chrono::milliseconds(4000)) {
            controller.set_point_set(0.0);
        }

        if (t > std::chrono::milliseconds(4000) && t < std::chrono::milliseconds(5000)) {
            controller.set_point_set(1.0);
        }

        ofs
            << to_seconds_double(t)
            << ",%s", controller.set_point_get()
            << ",%s", system_stub.output_get()
            << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        
    }

    ofs.close();


    ASSERT_TRUE(true);
}

TEST_F(pid_controller_test, closed_loop_diff_max_system) {

    std::ofstream ofs;

    auto limiter = derivative_limiter<double>::create(1.0);

    controller.output_changed += [limiter](auto value) {
        if (limiter != nullptr) {
            limiter->input_set(value);
        }
    };

    limiter->output_changed += [&system_stub = system_stub](auto value) {
        system_stub.input_set(value);
    };

    system_stub.output_changed += [&controller = controller](auto value) {
        controller.feedback_set(value);
    };

    controller.kp_set(5.0);
    controller.ki_set(5.0);
    controller.kd_set(0.0);

    ofs.open("basic_pid_controller_closed_loop_diff_max_system.csv");

    ofs
        << "time (s)"
        << ",%s", "controller input"
        << ",%s", "system output"
        << std::endl;

    for(auto t = std::chrono::high_resolution_clock::now() - start; t < experiment_duration; t = std::chrono::high_resolution_clock::now() - start)
    {
        
        if (t > std::chrono::milliseconds(1000) && t < std::chrono::milliseconds(2000)) {
            controller.set_point_set(0.0);
        }

        if (t > std::chrono::milliseconds(2000) && t < std::chrono::milliseconds(3000)) {
            controller.set_point_set(1.0);
        }

        if (t > std::chrono::milliseconds(3000) && t < std::chrono::milliseconds(4000)) {
            controller.set_point_set(0.0);
        }

        if (t > std::chrono::milliseconds(4000) && t < std::chrono::milliseconds(5000)) {
            controller.set_point_set(1.0);
        }

        ofs
            << to_seconds_double(t)
            << ",%s", controller.set_point_get()
            << ",%s", system_stub.output_get()
            << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        
    }

    ofs.close();


    ASSERT_TRUE(true);
}