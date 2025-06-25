#include "Order.hpp"
#include "OrderBook.hpp"
#include <iostream>

int main() {
    OrderBook book;

    // Add BUY order: ID 1, price 100.0, 10 shares
    Order buy1(1, Side::BUY, 100.0, 10);
    book.addOrder(buy1);

    Order sell1(2, Side::SELL, 99.0, 5);
    auto trades1 = book.match(sell1);

    std::cout << "\nTrades from SELL(2):\n";
    for (const auto& trade : trades1) {
        std::cout << " BuyID " << trade.buy_order_id
                    << ", SellID " << trade.sell_order_id
                    << ", Qty: " << trade.quantity
                    << ", Price: $" << trade.price << "\n";
    }

    if (sell1.quantity > 0) {
        book.addOrder(sell1);
    }


    Order sell2(3, Side::SELL, 100.0, 10);
    auto trades2 = book.match(sell2);

    std::cout << "\nTrades from SELL(3):\n";
    for (const auto& trade : trades2) {
        std::cout << " BuyID " << trade.buy_order_id
                    << ", SellID " << trade.sell_order_id
                    << ", Qty: " << trade.quantity
                    << ", Price: $" << trade.price << "\n";
    }

    if(sell2.quantity > 0) {
        book.addOrder(sell2);
    }

    book.printBook();
    return 0;
}