#pragma once
#include "OrderBook.hpp"
#include <atomic>

class MatchingEngine {
    public:
        MatchingEngine();

        void submitOrder(Side side, double price, uint32_t quantity);
        void printOrderBook() const;

        const auto& getBuyOrders() const { return orderBook_.getBuyOrders(); }
        const auto& getSellOrders() const { return orderBook_.getSellOrders(); }

    private:
        OrderBook orderBook_;
        //prevent race condition
        std::atomic<uint64_t> orderIdCounter_;
};