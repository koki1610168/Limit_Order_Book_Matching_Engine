#include "TradeLogger.hpp"
#include <iomanip>
#include <iostream>
#include <chrono>

TradeLogger::TradeLogger(const std::string& filename)
    : logFile_(filename, std::ios::out | std::ios::app), running_(false) {
        if (!logFile_.is_open()) {
            std::cerr << "Failed to open log file\n";
            perror("Reason");
        }
    }

TradeLogger::~TradeLogger() {
    stop();
}

void TradeLogger::start() {
    running_ = true;
    worker_ = std::thread(&TradeLogger::run, this);
}

void TradeLogger::stop() {
    running_ = false;
    cv_.notify_all();
    if (worker_.joinable()) {
        worker_.join();
    }
    logFile_.close();
}

void TradeLogger::logTrade(const Trade& trade) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(trade);
    cv_.notify_one();
}

void TradeLogger::run() {
    while (running_ || !queue_.empty()) {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [&] { return !queue_.empty() || !running_; });

        while (!queue_.empty()) {
            Trade trade = queue_.front();
            queue_.pop();
            lock.unlock();

            auto now = std::chrono::system_clock::now();
            auto now_time_t = std::chrono::system_clock::to_time_t(now);
            auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()) % 1000;
            
            
            std::tm tm = *std::localtime(&now_time_t);

            logFile_ << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") 
                    << "." << std::setfill('0') << std::setw(3) << now_ms.count() << "]"
                    << "Trade | BuyID: " << trade.buy_order_id
                    << " | SellID: " << trade.sell_order_id
                    << " | Price: " << trade.price
                    << " | Qty: " << trade.quantity << "\n";
            lock.lock();
        }
    }
}