#include "MatchingEngine.hpp"
#include <gtest/gtest.h>

TEST(MatchingEnineTest, BuyOrderStoredIfUnmached) {
    MatchingEngine engine;
    engine.submitOrder(Side::BUY, 100.0, 5);

    const auto& buyOrders = engine.getBuyOrders();
    ASSERT_EQ(buyOrders.size(), 1);
    auto it = buyOrders.begin();
    EXPECT_EQ(it->first, 100.0);
    ASSERT_EQ(it->second.size(), 1);
    EXPECT_EQ(it->second[0].quantity, 5);
}

TEST(MatchingEngineTest, SellOrderMatchesBuy) {
    MatchingEngine engine;
    engine.submitOrder(Side::BUY, 101.0, 5);
    engine.submitOrder(Side::SELL, 100.0, 5);

    EXPECT_TRUE(engine.getBuyOrders().empty());
    EXPECT_TRUE(engine.getSellOrders().empty());
}

TEST(MatchingEngineTest, PartialFillStoresRemaining) {
    MatchingEngine engine;
    engine.submitOrder(Side::SELL, 100.0, 10);
    engine.submitOrder(Side::BUY, 100.0, 6);

    const auto& sellOrders = engine.getSellOrders();
    ASSERT_EQ(sellOrders.size(), 1);
    auto it = sellOrders.begin();
    EXPECT_EQ(it->first, 100.0);
    EXPECT_EQ(it->second.front().quantity, 4);
}