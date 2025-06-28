#include "SocketWrapper.hpp"
#include "MatchingEngine.hpp"
#include <iostream>
#include <cstring>
#include "json.hpp"

constexpr int BUFFER_SIZE = 4096;
using json = nlohmann::json;

int main() {
    try {
        MatchingEngine engine;
        engine.startLogging();

        // Set up a server and listen on the port 12345
        SocketWrapper server;
        server.setMatchingEngine(&engine);
        server.bindAndListen(12345);
        std::cout << "Server listening on port 12345..." << std::endl;
        server.runEpollServerLoop();
        std::cout << "Client connected" << std::endl;
            
        engine.stopLogging();
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}