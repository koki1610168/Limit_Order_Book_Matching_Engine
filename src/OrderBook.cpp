#include "OrderBook.hpp"
#include <iostream>

void OrderBook::addOrder(const Order& order) {
    if (order.side == Side::BUY) {
        buyOrders[order.price].push_back(order);
    } else {
        sellOrders[order.price].push_back(order);
    }
}

std::vector<Trade> OrderBook::match(Order& order) {
    std::vector<Trade> trades;

    if (order.side == Side::BUY) {
        auto& book = sellOrders;
    } else {
        auto& book = buyOrders;
    }

    // Match against sellOrders
    if (order.side == Side::BUY) {
        for (auto it = sellOrders.begin(); it != sellOrders.end() && order.quantity > 0;) {
            if (order.price < it->first) break;

            auto& queue = it->second;
            while (!queue.empty() && order.quantity > 0) {
                Order& top = queue.front();
                uint32_t qty = std::min(order.quantity, top.quantity);

                trades.push_back({order.id, top.id, it->first, qty});
                order.quantity -= qty;
                top.quantity -= qty;

                if (top.quantity == 0) queue.pop_front();
            }

            if (queue.empty()) it = sellOrders.erase(it);
            else ++it;
        }
    } else {
        for (auto it = buyOrders.begin(); it != buyOrders.end() && order.quantity > 0;) {
            if (order.price > it->first) break;

            auto& queue = it->second;
            while (!queue.empty() && order.quantity > 0) {
                Order& top = queue.front();
                uint32_t qty = std::min(order.quantity, top.quantity);

                trades.push_back({top.id, order.id, it->first, qty});
                order.quantity -= qty;
                top.quantity -= qty;

                if (top.quantity == 0) queue.pop_front();
            }

            if (queue.empty()) it = buyOrders.erase(it);
            else ++it;
        }
    }
    return trades;
}

void OrderBook::printBook() const {
    std::cout << "\nORDER BOOK\n";

    std::cout << "BUY:\n";
    for (const auto& [price, orders] : buyOrders) {
        for (const auto& o : orders) {
            std::cout << "  ID " << o.id << " | Qty: " << o.quantity << " @ " << price << "\n";
        }
    }

    std::cout << "SELL:\n";
    for (const auto& [price, orders] : sellOrders) {
        for (const auto& o : orders) {
            std::cout << "  ID " << o.id << " | Qty: " << o.quantity << " @ " << price << "\n";
        }
    }
}