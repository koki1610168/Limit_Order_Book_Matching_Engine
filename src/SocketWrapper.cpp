#include "SocketWrapper.hpp"
#include "json.hpp"
#include "stdexcept"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

using json = nlohmann::json;


constexpr int MAX_EVENTS = 64;


SocketWrapper::SocketWrapper() : sockfd_(-1), connfd_(-1), is_server_(false) {}

SocketWrapper::~SocketWrapper() {
    if (connfd_ != -1) close(connfd_);
    if (sockfd_ != -1) close (sockfd_);
    if (epoll_fd_ != -1) close (epoll_fd_);   
}

void SocketWrapper::setMatchingEngine(MatchingEngine* engine) {
    engine_ = engine;
}

void SocketWrapper::makeSocketNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1 || fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        throw std::runtime_error("Failed to make socket non-blocking");
    }
}

void SocketWrapper::bindAndListen(int port) {
    is_server_ = true;
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ < 0) throw std::runtime_error("Socket creating failed");

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    int opt = 1;
    if (setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw std::runtime_error("setsocket opt (SO_REUSEADDR) failed");
    }

    if (bind(sockfd_, (sockaddr*)&addr, sizeof(addr)) < 0) {
        throw std::runtime_error(std::string("Bind failed: ") + std::strerror(errno));
    }

    if (listen(sockfd_, SOMAXCONN) < 0) {
        throw std::runtime_error("Listen failed");
    }

    epoll_fd_ = epoll_create1(0);
    if (epoll_fd_ == -1) {
        throw std::runtime_error("Failed to create epoll instance");
    }

    epoll_event ev{};
    ev.data.fd = sockfd_;
    ev.events = EPOLLIN;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, sockfd_, &ev) == -1) {
        throw std::runtime_error("Failed to add listen socket to epoll");
    }
}

void SocketWrapper::acceptClient() {
    sockaddr_in client_addr{};
    socklen_t len = sizeof(client_addr);
    connfd_ = accept(sockfd_, (sockaddr*)&client_addr, &len);
    if (connfd_ < 0) {
        throw std::runtime_error("Accept failed");
    }
}

void SocketWrapper::runEpollServerLoop() {
    epoll_event events[MAX_EVENTS];
    
    while (true) {
        int n = epoll_wait(epoll_fd_, events, MAX_EVENTS, -1);
        
        for (int i = 0; i < n; ++i) {
            if (events[i].data.fd == sockfd_) {
                // std::cout << "This is running" << std::endl;
                // Accept new connection, create a client socket
                sockaddr_in client_addr{};
                socklen_t client_len = sizeof(client_addr);
                int client_fd = accept(sockfd_, (sockaddr*)&client_addr, &client_len);
                if (client_fd < 0) continue;

                // Print new client connection info
                char client_ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
                std::cout << "New client connected: " << client_ip << ":" << ntohs(client_addr.sin_port) << std::endl;

                makeSocketNonBlocking(client_fd);
                
                // Add the newly created socket to epoll watchlist
                epoll_event client_ev{};
                client_ev.data.fd = client_fd;
                client_ev.events = EPOLLIN;
                epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, client_fd, &client_ev);
            } else {
                // Handle client data
                char buf[512];
                ssize_t count = recv(events[i].data.fd, buf, sizeof(buf), 0);
                if (count <= 0) {
                    close(events[i].data.fd);
                } else {
                    std::string input(buf, count);
                    try {
                        json j = json::parse(input);
                        Side side = (j["side"] == "BUY") ? Side::BUY : Side::SELL;
                        OrderType type = (j["type"] == "MARKET") ? OrderType::MARKET : OrderType::LIMIT;
                        double price = j.value("price", 0.0);
                        uint32_t quantity = j["quantity"];

                        engine_->submitOrder(side, type, price, quantity);
                        auto trades = engine_->getLastTrades();

                        json response;
                        response["status"] = engine_->getLastStatus();
                        for (const auto& trade : trades) {
                            response["trades"].push_back({
                                {"buy_order_id", trade.buy_order_id},
                                {"sell_order_id", trade.sell_order_id},
                                {"price", trade.price},
                                {"quantity", trade.quantity}
                            });
                        }

                        std::string out = response.dump();
                        std::cout << out << std::endl;
                        ::send(events[i].data.fd, out.c_str(), out.size(), 0);
                        
                        engine_->printOrderBook();

                    } catch (const std::exception& e) {
                        json errorResp = {
                            {"status", "ERROR"},
                            {"message", e.what()}
                        };
                        std::string err = errorResp.dump();
                        ::send(events[i].data.fd, err.c_str(), err.size(), 0);
                    }
                }
            }
        }
    }
}

void SocketWrapper::connectToServer(const std::string& ip, int port) {
    is_server_ = false;
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ < 0) throw std::runtime_error("Socket creating failed");

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr);

    if (connect(sockfd_, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        throw std::runtime_error(std::string("Connection failed") + std::strerror(errno));
    }
}

ssize_t SocketWrapper::send(const void* buf, size_t len) {
    return ::send(is_server_ ? connfd_ : sockfd_, buf, len, 0);
}

ssize_t SocketWrapper::receive(void* buf, size_t len) {
    return ::recv(is_server_ ? connfd_ : sockfd_, buf, len, 0);
}