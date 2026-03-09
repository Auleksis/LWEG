#include "functions.h"

int mod(int x, int q)
{
    return (x % q + q) % q;
}

int modExp(int base, int exp, int modulo)
{
    int result = 1;
    base = mod(base, modulo);
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = mod((result * base), modulo);
        }
        base = mod((base * base), modulo);
        exp >>= 1;
    }

    return result;
}

int gcdExtended(int a, int b, int *x, int *y)
{
    if (a == 0) {
		*x = 0, * y = 1;
		return b;
	}

	int x1, y1;
	int gcd = gcdExtended(b % a, a, &x1, &y1);

	*x = y1 - (b / a) * x1;
	*y = x1;

	return gcd;
}

bool isPrime(int number)
{
	for (int i = 2; i < sqrt(number); i++) {
		if (number % i == 0) {
			return false;
		}
	}
	return true;
}
