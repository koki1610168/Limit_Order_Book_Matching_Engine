#include "MatchingEngine.hpp"
#include <gtest/gtest.h>

TEST(MatchingEnineTest, BuyOrderStoredIfUnmached) {
    MatchingEngine engine;
    engine.submitOrder(Side::BUY, OrderType::LIMIT, 100.0, 5);

    const auto& buyOrders = engine.getBuyOrders();
    ASSERT_EQ(buyOrders.size(), 1);
    auto it = buyOrders.begin();
    EXPECT_EQ(it->first, 100.0);
    ASSERT_EQ(it->second.size(), 1);
    EXPECT_EQ(it->second[0].quantity, 5);
}

TEST(MatchingEngineTest, SellOrderMatchesBuy) {
    MatchingEngine engine;
    engine.submitOrder(Side::BUY, OrderType::LIMIT, 101.0, 5);
    engine.submitOrder(Side::SELL, OrderType::LIMIT, 100.0, 5);

    EXPECT_TRUE(engine.getBuyOrders().empty());
    EXPECT_TRUE(engine.getSellOrders().empty());
}

TEST(MatchingEngineTest, PartialFillStoresRemaining) {
    MatchingEngine engine;
    engine.submitOrder(Side::SELL, OrderType::LIMIT, 100.0, 10);
    engine.submitOrder(Side::BUY, OrderType::LIMIT, 100.0, 6);

    const auto& sellOrders = engine.getSellOrders();
    ASSERT_EQ(sellOrders.size(), 1);
    auto it = sellOrders.begin();
    EXPECT_EQ(it->first, 100.0);
    EXPECT_EQ(it->second.front().quantity, 4);
}

TEST(MatchingEngineTest, MarketOrderBuyFill) {
    MatchingEngine engine;
    engine.submitOrder(Side::SELL, OrderType::LIMIT, 100.0, 10);
    engine.submitOrder(Side::BUY, OrderType::MARKET, 90.0, 6);

    const auto& sellOrders = engine.getSellOrders();
    ASSERT_EQ(sellOrders.size(), 1);
    auto it = sellOrders.begin();
    EXPECT_EQ(it->first, 100.0);
    EXPECT_EQ(it->second.front().quantity, 4);
}

TEST(MatchingEngineTest, MarketOrderSellFill) {
    MatchingEngine engine;
    engine.submitOrder(Side::BUY, OrderType::LIMIT, 90.0, 10);
    engine.submitOrder(Side::SELL, OrderType::MARKET, 100.0, 6);

    const auto& buyOrders = engine.getBuyOrders();
    ASSERT_EQ(buyOrders.size(), 1);
    auto it = buyOrders.begin();
    EXPECT_EQ(it->first, 90.0);
    EXPECT_EQ(it->second.front().quantity, 4);
}

TEST(MatchingEngineTest, MarketOrderBetterPriceForBuy) {
    MatchingEngine engine;
    engine.submitOrder(Side::SELL, OrderType::LIMIT, 100.0, 10);
    engine.submitOrder(Side::BUY, OrderType::MARKET, 101.0, 6);

    const auto& sellOrders = engine.getSellOrders();
    ASSERT_EQ(sellOrders.size(), 1);
    auto it = sellOrders.begin();
    EXPECT_EQ(it->first, 100.0);
    EXPECT_EQ(it->second.front().quantity, 4);
}

TEST(MatchingEngineTest, MarketOrderNotMatched) {
    MatchingEngine engine;
    engine.submitOrder(Side::SELL, OrderType::LIMIT, 100.0, 10);
    engine.submitOrder(Side::SELL, OrderType::MARKET, 100.0, 6);

    const auto& sellOrders = engine.getSellOrders();
    ASSERT_EQ(sellOrders.size(), 1);
    auto it = sellOrders.begin();
    EXPECT_EQ(it->first, 100.0);
    EXPECT_EQ(it->second.front().quantity, 10);
}

TEST(MatchingEngineTest, MarketOrderMultipleOrders) {
    MatchingEngine engine;
    engine.submitOrder(Side::SELL, OrderType::LIMIT, 100.0, 10);
    engine.submitOrder(Side::BUY, OrderType::MARKET, 90.0, 6);
    engine.submitOrder(Side::BUY, OrderType::MARKET, 95.0, 4);

    const auto& sellOrders = engine.getSellOrders();
    ASSERT_EQ(sellOrders.size(), 0);
}