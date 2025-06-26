#include "MatchingEngine.hpp"
#include <iostream>

int main() {
    MatchingEngine engine;

    engine.startLogging();

    engine.submitOrder(Side::BUY, OrderType::LIMIT, 100.0, 10);
    engine.submitOrder(Side::SELL, OrderType::MARKET, 99.0, 5);
    engine.submitOrder(Side::SELL, OrderType::LIMIT, 100.0, 10);
    engine.submitOrder(Side::BUY, OrderType::MARKET, 98.0, 3);

    std::cout << "\nFinal Order Book:\n";
    engine.printOrderBook();
    
    engine.stopLogging();
    return 0;
}