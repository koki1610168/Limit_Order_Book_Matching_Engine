#include "SocketWrapper.hpp"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <vector>

TEST(SingleTCPConnectionTest, TCPBindAndListenDoesNotThrow) {
    SocketWrapper server;
    EXPECT_NO_THROW(server.bindAndListen(55555));
}

TEST(SingleTCPConnectionTest, CanConnectToServer) {
    SocketWrapper server;
    ASSERT_NO_THROW(server.bindAndListen(55556));

    std::thread server_thread([&]() {
        server.acceptClient();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    SocketWrapper client;
    EXPECT_NO_THROW(client.connectToServer("127.0.0.1", 55556));

    server_thread.detach();
}

