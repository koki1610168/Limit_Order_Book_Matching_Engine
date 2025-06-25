#include "OrderBook.hpp"
#include <gtest/gtest.h>

TEST(OrderBookTest, MatchBuyOrderPartialFill) {
    OrderBook book;

    Order sell(1, Side::SELL, 100.0, 5);
    book.addOrder(sell);

    Order buy(2, Side::BUY, 100.0, 3);
    auto trades = book.match(buy);

    ASSERT_EQ(trades.size(), 1);
    EXPECT_EQ(trades[0].buy_order_id, 2);
    EXPECT_EQ(trades[0].sell_order_id, 1);
    EXPECT_EQ(trades[0].price, 100.0);
    EXPECT_EQ(trades[0].quantity, 3);
}

TEST(OrderBookTest, MatchBuyOrderFullFill) {
    OrderBook book;

    Order buy(1, Side::BUY, 101.0, 10);
    book.addOrder(buy);

    Order sell(2, Side::SELL, 100.0, 10);
    auto trades = book.match(sell);

    ASSERT_EQ(trades.size(), 1);
    EXPECT_EQ(trades[0].buy_order_id, 1);
    EXPECT_EQ(trades[0].sell_order_id, 2);
    // This is 101 because exchange always give best offer to the seller.
    EXPECT_EQ(trades[0].price, 101.0);
    EXPECT_EQ(trades[0].quantity, 10);
}

TEST(OrderBOokTest, NoMatchOrderStored) {
    OrderBook book;

    Order sell(1, Side::SELL, 105.0, 4);
    auto trades = book.match(sell);

    EXPECT_TRUE(trades.empty());
    book.addOrder(sell);

    Order sell2(2, Side::SELL, 105.0, 3);
    auto trades2 = book.match(sell2);
    EXPECT_TRUE(trades2.empty());

    Order buy(3, Side::BUY, 104.0, 3);
    auto trades3 = book.match(buy);
    EXPECT_TRUE(trades3.empty());
    book.addOrder(buy);
}

