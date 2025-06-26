# C++ Limit & Market Order Book Matching Engine

A simple limit order book matching engine written in C++20

---

## Features
- Handles limit & market buy and sell orders
- Implements price-time priority
- Tracks partially filled and unmatched orders in an order book
- Asynchronous trade logging via a background thread
- Benchmarking with 1,000,000 orders
- Tested using GoogleTest

## Project Structure
<pre>
matching-engine/
|---include/
        |---MatchingEngine.hpp
        |---Order.hpp
        |---OrderBook.hpp
|---src/
        |---main.cpp
        |---MatchingEngine.cpp
        |---OrderBook.cpp
|---tests/
        |---MatchingEngineTest.cpp
        |---OrderBookTest.cpp
|---CMakeLists.txt
|---README.md
|---LICENSE.md
</pre>

---

## Getting Started

### Prerequisites

- C++20 compatible compiler
- CMake 3.14 or higher
- POSIX OS recommended

### Build Instructions
```bash
git clone https://github.com/koki1610168/Limit_Order_Book_Matching_Engine.git
cd Limit_Order_Book_Matching_Engine

mkdir build && cd build
cmake ..
make
```

### Running
You can eddit main.cpp to change orders

### Sample Input and Output
**Input**
```cpp
#include "MatchingEngine.hpp"
#include <iostream>

int main() {
    MatchingEngine engine;

    engine.submitOrder(Side::BUY, 100.0, 10);
    engine.submitOrder(Side::SELL, 99.0, 5);
    engine.submitOrder(Side::SELL, 100.0, 10);
    engine.submitOrder(Side::BUY, 98.0, 3);

    std::cout << "\nFinal Order Book:\n";
    engine.printOrderBook();
    
    return 0;
}
```
---
**Output**
```
New Order - ID: 1, Side: BUY, Qty: 10, Price: $100
  ---> Remaining Qty 10 added to buy order book.
--------------------------------------------------------

New Order - ID: 2, Side: SELL, Qty: 5, Price: $99
TRADE: BuyID 1, SellID 2, Qty: 5, Price: $100
--------------------------------------------------------

New Order - ID: 3, Side: SELL, Qty: 10, Price: $100
TRADE: BuyID 1, SellID 3, Qty: 5, Price: $100
  ---> Remaining Qty 5 added to sell order book.
--------------------------------------------------------

New Order - ID: 4, Side: BUY, Qty: 3, Price: $98
  ---> Remaining Qty 3 added to buy order book.
--------------------------------------------------------

Final Order Book:

ORDER BOOK
BUY:
  ID 4 | Qty: 3 @ 98
SELL:
  ID 3 | Qty: 5 @ 100
```

### Benchmark
**Input**
```
./benchmarking
```
**Output
```
Submitted 1000000 orders in 1.62821 seconds
Throughput: 614171 orders/sec
```

### Logging File Example
```
[2025-06-25 21:36:22.926]Trade | BuyID: 3 | SellID: 2 | Price: 93.12 | Qty: 5
[2025-06-25 21:36:22.926]Trade | BuyID: 3 | SellID: 1 | Price: 93.67 | Qty: 4
[2025-06-25 21:36:22.926]Trade | BuyID: 5 | SellID: 1 | Price: 93.67 | Qty: 4
[2025-06-25 21:36:22.926]Trade | BuyID: 5 | SellID: 4 | Price: 104.16 | Qty: 5
[2025-06-25 21:36:22.926]Trade | BuyID: 10 | SellID: 11 | Price: 99.12 | Qty: 1
[2025-06-25 21:36:22.926]Trade | BuyID: 8 | SellID: 12 | Price: 95.82 | Qty: 6
[2025-06-25 21:36:22.926]Trade | BuyID: 6 | SellID: 12 | Price: 93.64 | Qty: 3
[2025-06-25 21:36:22.926]Trade | BuyID: 23 | SellID: 20 | Price: 93.7 | Qty: 1
```
## License
MIT License 
MIT License © 2025 Koki Yahata