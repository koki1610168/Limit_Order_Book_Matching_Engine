#include "MatchingEngine.hpp"
#include "ClientUtils.hpp"
#include "SocketWrapper.hpp"
#include <random>
#include <chrono>
#include <iostream>

// evaluated at the compiler
constexpr int NUM_ORDERS = 1000000;
int main() {
    SocketWrapper client;
    client.connectToServer("127.0.0.1", 12345);
    std::cout << "Connected to server" << std::endl;

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> price_dist(90.0, 110.0);
    std::uniform_int_distribution<uint32_t> quantity_dist(1, 10);
    std::bernoulli_distribution side_dist(0.5);
    std::bernoulli_distribution type_dist(0.5);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_ORDERS; ++i) {
        std::string side = side_dist(rng) ? "BUY" : "SELL";
        std::string type = type_dist(rng) ? "LIMIT" : "MARKET";
        double raw = price_dist(rng);
        double price = std::round(raw * 100.0) / 100.0;
        uint32_t qty = quantity_dist(rng);
        
        sendOrder(client, side, type, price, qty);
        
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Submitted " << NUM_ORDERS << " orders in "
            << elapsed.count() << " seconds\n";
    std::cout << "Throughput: " << (NUM_ORDERS / elapsed.count()) << " orders/sec\n";
    
    return 0;
}