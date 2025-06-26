#pragma once
#include "OrderBook.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <thread>
#include <atomic>

class TradeLogger {
    public:
        TradeLogger(const std::string& filename);
        ~TradeLogger();

        void start();
        void stop();
        void logTrade(const Trade& trade);

    private:
        void run();

        std::queue<Trade> queue_;
        std::mutex mutex_;
        std::condition_variable cv_;
        std::thread worker_;
        std::ofstream logFile_;
        std::atomic<bool> running_;
};