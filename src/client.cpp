#include "SocketWrapper.hpp"
#include <iostream>
#include <cstring>

int main() {
    SocketWrapper client;
    client.connectToServer("127.0.0.1", 12345);

    const char* message = "Hello from client";


    client.send(message, std::strlen(message));

    char buffer[128] = {};
    client.receive(buffer, sizeof(buffer));

    std::cout << "Received from server: " << buffer << std::endl;
}