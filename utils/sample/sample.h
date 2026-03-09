#pragma once
#include <openssl/evp.h>
#include "../../Algebra/Poly/polyMatrix.h"
#include "../../HashUtils/shake128.h"
#include "../../HashUtils/shake256.h"
#include "../bytesUtils/bytesUtils.h"

void sampleNTT(const PolyRing& ring, PolyMatrix& A, uint8_t rho[32], int i, int j, int constant);

Poly samplePolyCBD(const PolyRing& ring, int noise, uint8_t* b);

PolyVector samplePolyVector(const PolyRing& ring, int vectorSize, int noise, uint8_t prfShake[32], int constant);
