#pragma once
#include "SocketWrapper.hpp"
#include <string>
#include <cstdint>


std::string buildJson(const std::string& side,
                    const std::string& type,
                    double price,
                    uint32_t quantity);

void sendOrder(SocketWrapper& client,
                    const std::string& side,
                    const std::string& type,
                    double price,
                    uint32_t quantity);

// void handleServerResponse(const std::string& responseStr);