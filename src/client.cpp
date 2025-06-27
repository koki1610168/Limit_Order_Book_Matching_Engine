#include "SocketWrapper.hpp"
#include "ClientUtils.hpp"
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

int main() {
    SocketWrapper client;
    client.connectToServer("127.0.0.1", 12345);
    std::cout << "Connected to server" << std::endl;


    sendOrder(client, "BUY", "LIMIT", 100.0, 10);
    sendOrder(client, "SELL", "MARKET", 99.0, 5);
    sendOrder(client, "SELL", "LIMIT", 100.0, 10);
    sendOrder(client, "BUY", "MARKET", 98.0, 3);

    return 0;
}