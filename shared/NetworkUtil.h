#pragma once

#include <string>
#include <stdint.h>

void logNetworkError(const char* message);
int getAddress(std::string hostname, uint64_t* address);