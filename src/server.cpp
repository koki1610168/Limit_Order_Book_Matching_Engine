#include "SocketWrapper.hpp"
#include <iostream>
#include <cstring>
int main() {
    SocketWrapper server;
    server.bindAndListen(12345);
    
    std::cout << "Server listening on port 12345..." << std::endl;

    server.acceptClient();
    std::cout << "Client connected" << std::endl;

    char buffer[128] = {};
    server.receive(buffer, sizeof(buffer));

    std::cout << "Received from client: " << buffer << std::endl;
    server.send(buffer, std::strlen(buffer));

}