#include "MatchingEngine.hpp"
#include <iostream>

MatchingEngine::MatchingEngine() : orderIdCounter_(1), logger_("../log/trades.log") {}

void MatchingEngine::startLogging() {
    logger_.start();
}

void MatchingEngine::stopLogging() {
    logger_.stop();
}
void MatchingEngine::submitOrder(Side side, double price, uint32_t quantity) {
    // increment by 1
    uint64_t id = orderIdCounter_.fetch_add(1);
    Order order(id, side, price, quantity);

    std::cout << "\nNew Order - ID: " << id
            << ", Side: " << (side == Side::BUY ? "BUY" : "SELL")
            << ", Qty: " << quantity
            << ", Price: $" << price << "\n";
    
    // match the incoming order with the existing orders in the order book.
    auto trades = orderBook_.match(order);

    for (const auto& trade : trades) {
        std::cout << "TRADE: BuyID " << trade.buy_order_id
                << ", SellID " << trade.sell_order_id
                << ", Qty: " << trade.quantity
                << ", Price: $" << trade.price << "\n";

        logger_.logTrade(trade);
    }

    if (order.quantity > 0) {
        orderBook_.addOrder(order);
        std::cout << "  ---> Remaining Qty " << order.quantity << " added to " << (side == Side::BUY ? "buy" : "sell") << " order book.\n";
    }
    std::cout << "--------------------------------------------------------" << std::endl;

}

void MatchingEngine::printOrderBook() const {
    orderBook_.printBook();
}