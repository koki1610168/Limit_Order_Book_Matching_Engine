#include "ClientUtils.hpp"
#include "SocketWrapper.hpp"
#include "json.hpp"
#include <iostream>

using json = nlohmann::json;

std::string buildJson(const std::string& side,
                    const std::string& type,
                    double price,
                    uint32_t quantity) {
    json order = {
        {"side", side},
        {"type", type},
        {"price", price},
        {"quantity", quantity}
    };
    return order.dump(); 
}


void sendOrder(SocketWrapper& client,
            const std::string& side,
            const std::string& type,
            double price,
            uint32_t quantity) {

    std::string message = buildJson(side, type, price, quantity);
    client.send(message.c_str(), message.size());

    char buffer[4096];
    ssize_t n = client.receive(buffer, sizeof(buffer));
    
    if (n > 0) {
        std::string response(buffer, n);
        std::cout << "Server Response:\n" << response << std::endl;
    }
    
}
