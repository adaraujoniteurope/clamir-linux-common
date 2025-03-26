#include <networking/tcp/server.hpp>

#include <atomic>
#include <sys/epoll.h>

std::function<void()> tcp_server::create(uint16_t port, std::function<void(int socket_fd)> handler, std::atomic_bool& shutdown, bool reuseaddr)
{
    return [port, handler, reuseaddr, &shutdown = shutdown]() -> void
    {
        std::vector<std::thread> threads;
        
        // Create socket
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            throw std::runtime_error("Failed to create socket");
        }

        // Set socket options
        if (reuseaddr) {
            int opt = 1;
            if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
                close(server_fd);
                throw std::runtime_error("Failed to set socket options");
            }
        }

        // Bind socket
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            close(server_fd);
            throw std::runtime_error("Failed to bind socket");
        }

        // Listen
        if (listen(server_fd, SOMAXCONN) < 0) {
            close(server_fd);
            throw std::runtime_error("Failed to listen");
        }

        // Create epoll instance
        int epoll_fd = epoll_create1(0);
        if (epoll_fd < 0) {
            close(server_fd);
            throw std::runtime_error("Failed to create epoll instance");
        }

        // Add server socket to epoll
        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = server_fd;
        
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) < 0) {
            close(server_fd);
            close(epoll_fd);
            throw std::runtime_error("Failed to add server socket to epoll");
        }

        const int MAX_EVENTS = 10;
        struct epoll_event events[MAX_EVENTS];

        while (!shutdown.load())
        {

            int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, 1);
            int err = errno;

            if (nfds < 0) {
                if (errno == EINTR && !shutdown.load()) continue;
                break;
            }

            for (int n = 0; n < nfds; ++n) {

                if (events[n].data.fd == server_fd) {
                    try {
                        struct sockaddr_in client_addr;
                        socklen_t client_len = sizeof(client_addr);
                        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
                        
                        if (client_fd < 0) {
                            throw std::runtime_error("Failed to accept connection");
                        }

                        threads.emplace_back([&handler, client_fd](){
                            handler(client_fd);
                            close(client_fd);
                        });

                    } catch (const std::exception& ex) {
                        std::cerr << ex.what() << std::endl;
                    }
                }
            }
        }

        // Cleanup
        for (auto& thread : threads) {
            thread.join();
        }
        
        close(epoll_fd);
        close(server_fd);
    };
}