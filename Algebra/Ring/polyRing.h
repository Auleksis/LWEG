#pragma once
#include <cmath>
#include <stdexcept>
#include "../../Math/functions.h"

class PolyRing
{
private:
	void findFirstPrimitiveRootOfUnity();

public:
	int q; // Coeffs modulo
	int n; // defines poly modulo X^n + 1
	int zeta; // first nth root of unity
	int invHalfN; // (N/2)^(-1) in q ring
	int invN;
	int qBitLength;

	PolyRing(int q, int n);
	void print(const char* prefix = "");

	friend bool operator== (const PolyRing& left, const PolyRing& right);
	friend bool operator!= (const PolyRing& left, const PolyRing& right);
};

