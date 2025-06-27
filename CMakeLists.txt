cmake_minimum_required(VERSION 3.14)
project(MatchingEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(server
    src/SocketWrapper.cpp
    src/server.cpp
)

add_executable(client
    src/SocketWrapper.cpp
    src/client.cpp
)

add_executable(benchmarking
    src/benchmarking.cpp
    src/MatchingEngine.cpp
    src/OrderBook.cpp
    src/TradeLogger.cpp
)
add_executable(engine
    src/main.cpp
    src/MatchingEngine.cpp
    src/OrderBook.cpp
    src/TradeLogger.cpp
)

add_subdirectory(external/googletest)
enable_testing()


add_executable(tests 
    tests/MatchingEngineTest.cpp
    tests/OrderBookTest.cpp 
    src/OrderBook.cpp
    src/MatchingEngine.cpp
    src/TradeLogger.cpp
)
target_link_libraries(tests gtest gtest_main)
add_test(NAME OrderBookTests COMMAND tests)