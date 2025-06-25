#include "MatchingEngine.hpp"
#include <iostream>

int main() {
    MatchingEngine engine;

    engine.submitOrder(Side::BUY, 100.0, 10);
    engine.submitOrder(Side::SELL, 99.0, 5);
    engine.submitOrder(Side::SELL, 100.0, 10);
    engine.submitOrder(Side::BUY, 98.0, 3);

    std::cout << "\nFinal Order Book:\n";
    engine.printOrderBook();
    
    return 0;
}