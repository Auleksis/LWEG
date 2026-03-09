#pragma once

#include <cstdint>
#include <cmath>

int mod(int x, int q);

int modExp(int base, int exp, int modulo);

int gcdExtended(int a, int b, int* x, int* y);

bool isPrime(int number);
