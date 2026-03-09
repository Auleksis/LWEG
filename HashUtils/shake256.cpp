#include "shake256.h"

void shake256_hash(uint8_t *input, int input_len, uint8_t *output, int output_len)
{
    for (int i = 0; i < output_len; i++) {
        output[i] = 0;
    }

    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    const EVP_MD* md = EVP_shake256();

    EVP_DigestInit_ex(mdctx, md, nullptr);
    EVP_DigestUpdate(mdctx, input, input_len);
    EVP_DigestFinalXOF(mdctx, output, output_len);

    EVP_MD_CTX_free(mdctx);
}