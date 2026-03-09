#pragma once

#include <openssl/evp.h>
#include <cmath>
#include <cstdint>

void shake256_hash(uint8_t* input, int input_len, uint8_t* output, int output_len);
