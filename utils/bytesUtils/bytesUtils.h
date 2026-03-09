#pragma once
#include <vector>
#include <cstdint>

std::vector<uint8_t> bytesToBits(const uint8_t* b, int bLen);

std::vector<uint8_t> bitsToBytes(uint8_t* b, int bLen);
