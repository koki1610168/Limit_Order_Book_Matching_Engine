#include "SocketWrapper.hpp"
#include "ClientUtils.hpp"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <vector>

TEST(MultiTCPConnectionTest, TCPBindAndListenDoesNotThrow) {
    SocketWrapper server;
    EXPECT_NO_THROW(server.bindAndListen(55555));
}

TEST(MultiTCPConnectionTest, TCPSingleClientConnection) {
    SocketWrapper server;
    ASSERT_NO_THROW(server.bindAndListen(55556));

    std::thread server_thread([&]() {
        server.runEpollServerLoop();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    SocketWrapper client;
    EXPECT_NO_THROW(client.connectToServer("127.0.0.1", 55556));

    server_thread.detach();
}

TEST(MultiTCPConnectionTest, TCPMultiClientConnections) {
    SocketWrapper server;
    ASSERT_NO_THROW(server.bindAndListen(55557));

    std::thread server_thread([&]() {
        server.runEpollServerLoop();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    constexpr int client_count = 5;
    std::vector<std::thread> clients;

    for (int i = 0; i < client_count; ++i) {
        clients.emplace_back([i]() {
            try {
                SocketWrapper client;
                EXPECT_NO_THROW(client.connectToServer("127.0.0.1", 55557));
            } catch (...) {
                FAIL() << "Client " << i << "failed to connect" << std::endl;
            }
        });
    }
    for (auto& t : clients) {
        t.join();
    }
 
    server_thread.detach();
}

TEST(MultiTCPConnectionTest, TCPMultiClientOrders) {
    MatchingEngine engine;
    SocketWrapper server;
    server.setMatchingEngine(&engine);
    ASSERT_NO_THROW(server.bindAndListen(55558));

    std::thread server_thread([&]() {
        server.runEpollServerLoop();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    constexpr int client_count = 2;
    std::vector<std::thread> clients;

    for (int i = 0; i < client_count; ++i) {
        clients.emplace_back([i]() {
            try {
                SocketWrapper client;
                ASSERT_NO_THROW(client.connectToServer("127.0.0.1", 55558));

                EXPECT_NO_THROW(sendOrder(client, "BUY", "LIMIT", 100.0, 10));
                EXPECT_NO_THROW(sendOrder(client, "SELL", "MARKET", 99.0, 5));
                EXPECT_NO_THROW(sendOrder(client, "SELL", "LIMIT", 100.0, 10));
                EXPECT_NO_THROW(sendOrder(client, "BUY", "MARKET", 98.0, 3));

                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        });
    }
    for (auto& t: clients) {
        t.join();
    }

    server_thread.detach();
}