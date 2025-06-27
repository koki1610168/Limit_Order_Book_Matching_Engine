#include "SocketWrapper.hpp"
#include "ClientUtils.hpp"
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

int main() {
    SocketWrapper client;
    client.connectToServer("127.0.0.1", 12345);
    std::cout << "Connected to server" << std::endl;


    sendOrder(client, "BUY", "LIMIT", 101.5, 10);
    sendOrder(client, "SELL", "LIMIT", 101.5, 15);
    sendOrder(client, "BUY", "LIMIT", 99.5, 5);


    return 0;
}