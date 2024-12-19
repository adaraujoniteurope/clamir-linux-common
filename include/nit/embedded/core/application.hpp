#ifndef _nit_embedded_core_application_
#define _nit_embedded_core_application_

#include <cstddef>
#include <cstdlib>
#include <chrono>
#include <thread>

#include <nit/embedded/core/version.hpp>

namespace nit::embedded::core {

    class application {
        public:

        application() {}

        virtual void load_config() {}

        virtual void init(const char * settings = nullptr) {}

        virtual void poll() {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    };

}
#endif