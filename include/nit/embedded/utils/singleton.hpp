#ifndef _nit_embedded_singleton_hpp_
#define _nit_embedded_singleton_hpp_

#include <memory>
namespace nit::embedded::utils
{
    template<typename derived>
    struct singleton : public std::enable_shared_from_this<derived> {
        static std::shared_ptr<derived> get_instance() {
            
            static std::shared_ptr<derived> _instance;

            if (_instance == nullptr) {
                _instance = std::make_shared<derived>();
            }

            return _instance;
        }
    };

};

#endif