#ifndef _nit_embedded_application_hpp_
#define _nit_embedded_application_hpp_

#include <atomic>
#include <memory>

#include <nit/embedded/utils/singleton.hpp>

namespace nit::embedded::utils
{
    template<typename derived>
    class abstract_application : public utils::singleton<derived> {
        public:

        static std::shared_ptr<derived> get_instance() {
            static std::shared_ptr<derived> _instance;
            if (_instance == nullptr) {
                _instance = std::make_shared<derived>();
            }

            return _instance;
        }

        virtual int initialize(int argc = 0, char* argv[] = nullptr) = 0;
        virtual int run(std::atomic_bool& shutdown) = 0;

        private:
    };

};

#endif