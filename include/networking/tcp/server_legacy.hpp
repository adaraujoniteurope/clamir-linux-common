#ifndef NETWORKING_TCP_SERVER_H_
#define NETWORKING_TCP_SERVER_H_

#include <string>
#include <cstdint>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <time.h>
#include <assert.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>
#include <sched.h>

#include <memory>
#include <future>
#include <atomic>

#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <boost/asio.hpp>
#include <format>
#include <future>
#include <map>

#include <coroutine>

namespace
{

  struct connection_handler
  {
    connection_handler(tcp::socket sock) : socket(std::move(sock)) {}
    std::mutex mutex;
    tcp::socket socket;
  };

  class tcp_server
  {
  public:
    using accept_handler = std::function<void(boost::system::error_code ec, std::shared_ptr<connection_handler> socket)>;
    tcp_server(boost::asio::io_context &ioc, uint16_t port = 4004)
        : m_ioc(ioc), m_endpoint(tcp::v4(), port)
    {
    }

    virtual ~tcp_server() {}

    void accept(accept_handler handler = nullptr)
    {
      m_accept_handler = handler;
      m_acceptor = std::make_shared<tcp::acceptor>(m_ioc, m_endpoint);
      m_acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
      m_acceptor->async_accept(std::bind(&tcp_server::handle_accept, this, std::placeholders::_1, std::placeholders::_2));
      m_acceptor->listen();
    }

  private:
    std::thread m_acceptor_thread;

    void handle_accept(boost::system::error_code ec, tcp::socket socket)
    {

      if (ec)
      {
        std::cout << ec.what() << std::endl;
        return;
      }

      auto acceptor_thread = std::thread([&]()
                                         { m_accept_handler(ec, std::make_shared<connection_handler>(std::move(socket))); });

      m_acceptor->async_accept(std::bind(&tcp_server::handle_accept, this, std::placeholders::_1, std::placeholders::_2));
      m_acceptor_threads[acceptor_thread.get_id()] = std::move(acceptor_thread);
    }

    std::map<std::thread::id, std::thread> m_acceptor_threads;

    accept_handler m_accept_handler;
    boost::asio::io_context &m_ioc;
    tcp::endpoint m_endpoint;
    std::shared_ptr<tcp::acceptor> m_acceptor;

    void *parent;
  };
}

#endif