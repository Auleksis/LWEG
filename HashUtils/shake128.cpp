#include "shake128.h"

SHAKE128::SHAKE128()
{
    ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_shake128(), nullptr); 
}

SHAKE128::~SHAKE128()
{
    EVP_MD_CTX_free(ctx);
}

void SHAKE128::absorb(const uint8_t *input, int input_len)
{
    EVP_DigestUpdate(ctx, input, input_len);
}

void SHAKE128::squeeze(uint8_t *output, int output_len)
{
    EVP_DigestSqueeze(ctx, output, output_len);
}

void SHAKE128::reset()
{
    EVP_DigestInit_ex(ctx, EVP_shake128(), nullptr);
}
