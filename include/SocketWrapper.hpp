#pragma once
#include "MatchingEngine.hpp"
#include <string>

class SocketWrapper {
    public:
        SocketWrapper();
        ~SocketWrapper();

        // binds to a port and starts listening for incoming connections
        void bindAndListen(int port);
        void acceptClient();
        void connectToServer(const std::string& ip, int port);

        // For multi-clients, epoll
        void runEpollServerLoop();

        void setMatchingEngine(MatchingEngine* engine);

        ssize_t send(const void* buf, size_t len);
        ssize_t receive(void *buf, size_t len);
        // void setNonBlocking(int fd);
        // int getFd() const;

    private:
        int sockfd_;
        int connfd_;
        bool is_server_;
        int epoll_fd_;

        MatchingEngine* engine_;

        void makeSocketNonBlocking(int fd);
};