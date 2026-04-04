#include "LWEG.h"

LWEG::LWEG(const PolyRing& ring, const LWEGContext& lwegContext):
	ring(ring), lwegContext(lwegContext),
	s(ring, lwegContext.polySize),
	e(ring, lwegContext.polySize)
{
	vectorA = std::vector<PolyMatrix>(lwegContext.m, PolyMatrix(ring, lwegContext.polySize));
}

void LWEG::initialize(std::vector<uint8_t>& d)
{
    std::vector<uint8_t> gHash(64);
	std::vector<uint8_t> gHashInput(33);
	memcpy(gHashInput.data(), d.data(), 32);
	gHashInput[32] = lwegContext.polySize;

	if (!EVP_Digest(gHashInput.data(), gHashInput.size(), gHash.data(), nullptr, EVP_sha3_512(), nullptr)) {
		printf("Error while G hash calculation in LWEG::initialize\n");
        return;
	}

	uint8_t rho[32];
	memcpy(rho, gHash.data(), 32);
	uint8_t sigma[32];
	memcpy(sigma, &gHash[32], 32);

	for (int m = 0; m < lwegContext.m; m++) {
		for (int i = 0; i < lwegContext.polySize; i++) {
			for (int j = 0; j < lwegContext.polySize; j++) {
				sampleNTT(ring, vectorA[m], rho, i, j, m);
			}
		}
	}

	uint8_t prfShakeInput[32];
	memcpy(prfShakeInput, sigma, 32);

	uint8_t N = 0;

	s = samplePolyVector(ring, lwegContext.polySize, lwegContext.noise, prfShakeInput, N);
	s.ntt();
	N += lwegContext.polySize;
	e = samplePolyVector(ring, lwegContext.polySize, lwegContext.noise, prfShakeInput, N);
	e.ntt();
}

void LWEG::prng(std::vector<uint8_t> &bytes)
{
	std::vector<uint8_t> output;
	uint8_t prfShakeInput[32];
	
	int counter = 0;
	uint8_t N = 0;

	while (counter < bytes.size()) {
		for (int t = 0; t < lwegContext.m - 1; t++) {
			PolyMatrix A = vectorA[t];
			PolyVector b = A * s + e;
			b.invntt();
			
			for(int i = 0; i < lwegContext.polySize; i++) {
				for(int j = 0; j < ring.n; j++) {
					if(b[i][j] < lwegContext.qS) {
						output.push_back(b[i][j] & 255);
						counter++;
					}
				}
			}


			Poly xorResult = b[0];
			for (int k = 1; k < lwegContext.polySize; k++) {
					xorResult ^= b[k];
			}
			for (int i = 0; i < 32; i++) {
				prfShakeInput[i] = xorResult[i] & 255;
			}

			e = samplePolyVector(ring, lwegContext.polySize, lwegContext.noise, prfShakeInput, N);
			e.ntt();
			N += lwegContext.polySize;
		}

		PolyMatrix A = vectorA[lwegContext.m - 1];
		PolyVector b = A * s + e;
		b.invntt();

		Poly xorResult = b[0];
		for (int k = 1; k < lwegContext.polySize; k++) {
				xorResult ^= b[k];
		}
		for (int i = 0; i < 32; i++) {
			prfShakeInput[i] = xorResult[i] & 255;
		}

		s = samplePolyVector(ring, lwegContext.polySize, lwegContext.noise, prfShakeInput, N);
		s.ntt();
		N += lwegContext.polySize;
		e = samplePolyVector(ring, lwegContext.polySize, lwegContext.noise, prfShakeInput, N);
		e.ntt();
		N += lwegContext.polySize;
	}

	for (int i = 0; i < bytes.size(); i++) {
		bytes[i] = output[i];
	}
}
