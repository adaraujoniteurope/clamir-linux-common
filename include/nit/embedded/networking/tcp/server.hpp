// #ifndef _networking_tcp_server_hpp_
// #define _networking_tcp_server_hpp_

// #include <atomic>
// #include <memory>
// #include <map>
// #include <list>
// #include <vector>
// #include <memory>

// #include <nit/embedded/core/object.hpp>
// #include <nit/embedded/core/signal.hpp>

// #include <sys/epoll.h>

// using namespace nit::embedded::core;

// template <typename type> class creator : public object<type> {
// public:
//   template <typename... args_types>
//   static std::shared_ptr<type> create(args_types... args) {
//     return std::make_shared(std::forward<args_types>(args)...);
//   }
// };

// namespace nit::embedded::networking::tcp {

// class socket_wrapper;
// class server;

// class socket_wrapper : public creator<socket_wrapper> {
// public:
//   socket_wrapper(int fd) : _fd(fd) {}

//   int get_fd() { return _fd; }

// private:
//   int _fd;
// };

// class client_handler;

// struct message : public creator<message>
// {
//   public:

//   message(std::shared_ptr<client_handler> _client, const std::vector<uint8_t>& _buffer) : client(_client), buffer(_buffer) {}
//   std::shared_ptr<client_handler> client;
//   std::vector<uint8_t> buffer;

// };

// class client_handler : public creator<client_handler> {
//   public:

//   core::signal<client_handler, std::vector<uint8_t>> data_received;
//   core::signal<client_handler, std::vector<uint8_t>> error;

//   void write(const std::vector<uint8_t>& bytes) {
//     auto msg = message::create(shared_from_this(), bytes);
//     // _server.write(message);
//   }

//   private:
//   std::shared_ptr<server> _server;
// };

// class server : public creator<server> {
// public:
//   using pointer = typename std::shared_ptr<server>;

//   core::signal<server::pointer, int> error;
//   core::signal<server::pointer> listening;
//   core::signal<server::pointer, client_handler> client_connected;

//   server(int _max_client = 1024) : _epoll_max_events(_max_client), _epoll_events(std::vector<struct epoll_event>(_max_client)) {
//     listening += std::bind(&server::on_listen, this, std::placeholders::_1);
//     error += std::bind(&server::on_error, this, std::placeholders::_1,
//                        std::placeholders::_2);
//   }

//   virtual ~server() { close(); }

//   void listen(const char *host, int port) {
//     std::unique_lock<std::mutex> lk(mutex_get());
//     listening(shared_from_this());
//   }

//   void write(std::shared_ptr<client_handler> client, const std::vector<uint8_t>& buffer) {

//   }


//   void close() {
//     std::unique_lock<std::mutex> lk(mutex_get());
//     _close = true;
//     _epoll_wait_thread.join();
//   }

// private:

//   core::signal<std::shared_ptr<server>, struct epoll_event> _epoll_epollin_received;
//   core::signal<std::shared_ptr<server>, struct epoll_event> _epoll_epollout_received;

//   void on_listen(pointer server) {
//     _epoll_wait_thread = std::thread(&server::_epoll_wait, this);
//   }

//   void on_error(pointer server, int err) {}

//   void _epoll_wait() {
//     while(!_close) {
//       std::unique_lock<std::mutex> lk(_epoll_events_mutex);
//       int event_count = epoll_wait(_epoll_fd, _epoll_events.data(), _epoll_max_events, 100);

//       for (auto i = 0; i < event_count; i++) {

//         if (_epoll_events[i].events & EPOLLIN) {
//           _epoll_epollin_received(shared_from_this(), _epoll_events[i]);
//         }

//         if (_epoll_events[i].events & EPOLLOUT) {
//           _epoll_epollin_received(shared_from_this(), _epoll_events[i]);
//         }
//       }
//     }
//   }

//   void _epoll_epollin_server_accept(std::shared_ptr<server> server, struct epoll_event event)
//   {
//     // try lock server?
//     std::unique_lock<std::mutex> lk(mutex_get());
//   }

//   void _epoll_epollin_client_receive(std::shared_ptr<server> server, struct epoll_event event)
//   {
//     // try lock server?
//     std::unique_lock<std::mutex> lk(mutex_get());
//   }

//   void _epoll_epollout_client_send(std::shared_ptr<server> server, struct epoll_event event)
//   {
//     // try lock server?
//     std::unique_lock<std::mutex> lk(mutex_get());
//   }

//   std::atomic_bool _close;
//   std::thread _epoll_wait_thread;

//   int _epoll_fd;
//   int _fd;
//   const int _epoll_max_events;
//   std::mutex _epoll_events_mutex;
//   std::vector<struct epoll_event> _epoll_events;

//   std::map<int, std::shared_ptr<client_handler>> _clients;
// };

// } // namespace nit::embedded::networking::tcp

// #endif