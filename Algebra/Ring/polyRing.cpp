#include "polyRing.h"

void PolyRing::findFirstPrimitiveRootOfUnity()
{
	bool foundRoot;
	for (int candidate = 2; candidate < q; candidate++) {
		foundRoot = true;
		for (int i = 1; i < n; i++) {
			if (modExp(candidate, i, q) == 1) {
				foundRoot = false;
				break;
			}

		}
		if (foundRoot && modExp(candidate, n, q) == 1) {
			zeta = candidate;
			return;
		}
	}
}

PolyRing::PolyRing(int q, int n): q(q), n(n)
{
	if (!isPrime(q)) {
		throw std::invalid_argument("q must be prime");
	}

	if ((q - 1) % n != 0) {
		throw std::invalid_argument("n must divide q - 1");
	}

	qBitLength = (int)(log2(q)) + 1;

	int x = 0, y = 0;
	gcdExtended(n, q, &x, &y);
	invN = mod(x, q);

	findFirstPrimitiveRootOfUnity();

	x = y = 0;
	gcdExtended(n / 2, q, &x, &y);
	invHalfN = mod(x, q);
}

void PolyRing::print(const char* prefix)
{
	printf("%s\nPolyRing info:\n\tQ = %d (%d bits)\n\tN = %d\n\tZeta = %d\n\tN^-1 = %d\n\t(N / 2)^(-1) = %d\n\n", prefix, q, qBitLength, n, zeta, invN, invHalfN);
}

bool operator==(const PolyRing &left, const PolyRing &right)
{
	return left.n == right.n && left.q == right.q;
}

bool operator!=(const PolyRing &left, const PolyRing &right)
{
	return left.n != right.n || left.q != right.q;
}
