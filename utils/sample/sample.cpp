#include "sample.h"

void sampleNTT(const PolyRing& ring, PolyMatrix &A, uint8_t rho[32], int i, int j, int constant = 0)
{
    SHAKE128 ctx;

	uint8_t seed[35];
	memcpy(seed, rho, 32);
	seed[32] = i;
	seed[33] = j;
	seed[34] = constant;

	ctx.absorb(seed, 35);

	short counter = 0;

	int n = ring.n;
	int q = ring.q;

	while (counter < n) {
		uint8_t C[3];
		ctx.squeeze(C, 3);
		short d1 = C[0] + 256 * (C[1] % 16);
		short d2 = C[1] / 16 + 16 * C[2];

		if (d1 < q) {
			A(i, j)[counter] = d1;
			counter++;
		}

		if (d2 < q && counter < n) {
			A(i, j)[counter] = d2;
			counter++;
		}
	}
}

Poly samplePolyCBD(const PolyRing& ring, int noise, uint8_t *b)
{
    Poly result(ring);
	int n = ring.n;
	int q = ring.q;
	std::vector<uint8_t> bits = bytesToBits(b, 64 * noise);
	for (int i = 0; i < n; i++) {
		int x = 0;
		int y = 0;

		for (int j = 0; j < noise; j++) {
			x += bits[2 * i * noise + j];
			y += bits[2 * i * noise + noise + j];
		}

		result[i] = mod((x - y), q);
	}
	return result;
}

PolyVector samplePolyVector(const PolyRing &ring, int vectorSize, int noise, uint8_t seed[32], int constant)
{
	std::vector<uint8_t> B(64 * noise);

	PolyVector result(ring, vectorSize);

	int N = constant;

	uint8_t prfShake[36];
	memcpy(prfShake, seed, sizeof(uint8_t) * 32);

	for (int i = 0; i < vectorSize; i++) {
		memcpy(prfShake + 32, &N, sizeof(int));
		shake256_hash(prfShake, 36, B.data(), 64 * (int)noise);
		result[i] = samplePolyCBD(ring, noise, B.data());
		N++;
	}

    return result;
}
