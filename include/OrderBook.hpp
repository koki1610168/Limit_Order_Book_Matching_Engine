#pragma once
#include "Order.hpp"
#include <map>
#include <deque>
#include <vector>

// What actually happens in the book, not the user's submitioni
struct Trade {
    uint64_t buy_order_id;
    uint64_t sell_order_id;
    double price;
    uint32_t quantity;
};

class OrderBook {
    public:
        void addOrder(const Order& order);
        // match an incoming order against the opposite book
        std::vector<Trade> match(Order& order);
        void printBook() const;

    private:
        //deque, because FIFO.
        //sort in decending order.
        std::map<double, std::deque<Order>, std::greater<>> buyOrders;
        std::map<double, std::deque<Order>> sellOrders;
};