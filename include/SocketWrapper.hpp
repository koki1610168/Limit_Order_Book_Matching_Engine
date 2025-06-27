#pragma once
#include <string>

class SocketWrapper {
    public:
        SocketWrapper();
        ~SocketWrapper();

        // binds to a port and starts listening for incoming connections
        void bindAndListen(int port);
        void acceptClient();
        void connectToServer(const std::string& ip, int port);

        ssize_t send(const void* buf, size_t len);
        ssize_t receive(void *buf, size_t len);
        // void setNonBlocking(int fd);
        // int getFd() const;

    private:
        int sockfd_;
        int connfd_;
        bool is_server_;
};