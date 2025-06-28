#include "SocketWrapper.hpp"
#include "ClientUtils.hpp"
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;
/*
void receiveAndPrintResponse(SocketWrapper& client) {
    char buffer[4096];
    ssize_t n = client.receive(buffer, sizeof(buffer));
    if (n > 0) {
        std::string response_str(buffer, n);

        try {
            json response = json::parse(response_str);
            std::cout << response.dump() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Failed to parse server response" << std::endl;
        }
    } else {
        std::cerr << "No response" << std::endl;
    }
}
*/
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