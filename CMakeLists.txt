cmake_minimum_required(VERSION 3.14)
project(MatchingEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(engine ${SOURCES})

add_subdirectory(external/googletest)
enable_testing()


add_executable(tests 
    tests/MatchingEngineTest.cpp
    tests/OrderBookTest.cpp 
    src/OrderBook.cpp
    src/MatchingEngine.cpp
)
target_link_libraries(tests gtest gtest_main)
add_test(NAME OrderBookTests COMMAND tests)