#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "components/timer.hpp"

// timer timer::s_singleton;

// timer &timer::get_instance()
// {
//     return s_singleton;
// }

// template <typename function_type>
// timer timer::create(long interval, function_type callback, std::atomic_bool &shutdown)
// {
//     return std::move(timer(interval, callback, shutdown));
// }

// long timer::get_current_time_ns()


// void timer::run()


// void timer::wait()
