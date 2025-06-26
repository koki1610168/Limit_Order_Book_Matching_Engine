#include "MatchingEngine.hpp"
#include <random>
#include <chrono>
#include <iostream>

// evaluated at the compiler
constexpr int NUM_ORDERS = 1000000;
int main() {
    MatchingEngine engine;
    engine.startLogging();

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> price_dist(90.0, 110.0);
    std::uniform_int_distribution<uint32_t> quantity_dist(1, 10);
    std::bernoulli_distribution side_dist(0.5);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_ORDERS; ++i) {
        Side side = side_dist(rng) ? Side::BUY : Side::SELL;
        double raw = price_dist(rng);
        double price = std::round(raw * 100.0) / 100.0;
        uint32_t qty = quantity_dist(rng);
        engine.submitOrder(side, price, qty);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    engine.stopLogging();

    std::cout << "Submitted " << NUM_ORDERS << " orders in "
            << elapsed.count() << " seconds\n";
    std::cout << "Throughput: " << (NUM_ORDERS / elapsed.count()) << " orders/sec\n";
    
    return 0;
}