#include "MatchingEngine.hpp"
#include <iostream>

MatchingEngine::MatchingEngine() : orderIdCounter_(1) {}

void MatchingEngine::submitOrder(Side side, double price, uint32_t quantity) {
    uint64_t id = orderIdCounter_.fetch_add(1);
    Order order(id, side, price, quantity);

    std::cout << "\nNew Order - ID: " << id
            << ", Side: " << (side == Side::BUY ? "BUY" : "SELL")
            << ", Qty: " << quantity
            << ", Price: $" << price << "\n";
    
    auto trades = orderBook_.match(order);

    for (const auto& trade : trades) {
        std::cout << "  TRADE: BuyID " << trade.buy_order_id
                << ", SellID " << trade.sell_order_id
                << ", Qty: " << trade.quantity
                << ", Price: $" << trade.price << "\n";
    }

    if (order.quantity > 0) {
        orderBook_.addOrder(order);
        std::cout << "  ---> Remaining Qty " << order.quantity << " added to book.\n";
    }
}

void MatchingEngine::printOrderBook() const {
    orderBook_.printBook();
}