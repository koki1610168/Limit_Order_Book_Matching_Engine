#include "SocketWrapper.hpp"
#include "MatchingEngine.hpp"
#include <iostream>
#include <cstring>
#include "json.hpp"

constexpr int BUFFER_SIZE = 4096;
using json = nlohmann::json;

int main() {
    try {
        // Set up a server and listen on the port 12345
        SocketWrapper server;
        server.bindAndListen(12345);
        std::cout << "Server listening on port 12345..." << std::endl;
        server.acceptClient();
        std::cout << "Client connected" << std::endl;

        MatchingEngine engine;
        char buffer[BUFFER_SIZE];

        while (true) {
            ssize_t received = server.receive(buffer, BUFFER_SIZE);
            if (received <= 0) break;
            // std::cout << "I Received!" << std::endl;
            std::string input(buffer, received);
            try {
                json j = json::parse(input);
                Side side = (j["side"] == "BUY") ? Side::BUY : Side::SELL;
                OrderType type = (j["type"] == "MARKET") ? OrderType::MARKET : OrderType::LIMIT;
                double price = j.value("price", 0.0);
                uint32_t quantity = j["quantity"];

                engine.submitOrder(side, type, price, quantity);
                auto trades = engine.getLastTrades();

                json response;
                response["status"] = engine.getLastStatus();
                for (const auto& trade : trades) {
                    response["trades"].push_back({
                        {"buy_order_id", trade.buy_order_id},
                        {"sell_order_id", trade.sell_order_id},
                        {"price", trade.price},
                        {"quantity", trade.quantity}
                    });
                }

                std::string out = response.dump();
                server.send(out.c_str(), out.size());
                
                engine.printOrderBook();

            } catch (const std::exception& e) {
                json errorResp = {
                    {"status", "ERROR"},
                    {"message", e.what()}
                };
                std::string err = errorResp.dump();
                server.send(err.c_str(), err.size());
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}