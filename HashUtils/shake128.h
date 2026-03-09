#pragma once

#include <openssl/evp.h>

class SHAKE128
{
private:
    EVP_MD_CTX* ctx;

public:
    SHAKE128();

    ~SHAKE128();

    void absorb(const uint8_t* input, int input_len);

    void squeeze(uint8_t* output, int output_len);

    void reset();
};
