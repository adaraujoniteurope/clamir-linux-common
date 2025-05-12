#ifndef _nit_embedded_abstract_application_hpp_
#define _nit_embedded_abstract_application_hpp_

#include "nit/embedded/clamir_service.hpp"
#include <atomic>
#include <dbus-cxx.h>
#include <memory>
#include <nit/embedded/abstract_application.hpp>

namespace nit::embedded {
class clamir_service_dbus : public abstract_application<clamir_service_dbus> {
public:
  int initialize(int argc, char *argv[]) override {

    /// Initialize CLAMIR Controller
    _clamir_service->initialize(argc, argv);

    /// Create DBUS Interface
    _dispatcher = DBus::StandaloneDispatcher::create();
    _connection = _dispatcher->create_connection(DBus::BusType::SESSION);

    _object = _connection->create_object(
        "/org/nit/clamir/ctrl", DBus::ThreadForCalling::DispatcherThread);

    // Register Clamir CTRL Interface in the BUS
    // for now, just the server packet decoder directly...
    _object->create_method<int32_t(int32_t)>(
        "clamir.ctrl.server", "command_processor_legacy",
        sigc::mem_fun(*_clamir_service,
                      &clamir_service::command_processor_legacy));

    if (_connection->request_name("clamir.ctrl.server",
                                  DBUSCXX_NAME_FLAG_REPLACE_EXISTING) !=
        DBus::RequestNameResponse::PrimaryOwner) {
      return -EACCES;
    }

    return 0;
  }

  int run(std::atomic_bool &shutdown) override {

    /// RUN Clamir Controller
    _clamir_service->run(shutdown);
    return 0;
  }

protected:
private:
  std::shared_ptr<DBus::Dispatcher> _dispatcher;
  std::shared_ptr<DBus::Object> _object;
  std::shared_ptr<DBus::Connection> _connection;

  std::shared_ptr<clamir_service> _clamir_service;
};
} // namespace nit::embedded

#endif