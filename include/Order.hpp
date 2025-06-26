#pragma once
#include <cstdint>
#include <chrono>

enum class Side { BUY, SELL };

enum class OrderType { LIMIT, MARKET };

// Order submitted by a user.
struct Order {
    uint64_t id;
    Side side;
    OrderType type;
    double price;
    uint32_t quantity;
    // First in First Out structure
    uint64_t timestamp;

    Order(uint64_t id, Side side, double price, uint32_t qty, OrderType type = OrderType::LIMIT)
        : id(id), side(side), type(type), price(price), quantity(qty)
    {
        using namespace std::chrono;
        timestamp = duration_cast<microseconds>(
            high_resolution_clock::now().time_since_epoch()).count();
    }
};