#pragma once

#include "../consts.h"
#include "string.h"
#include <vector>
#include "../Ring/polyRing.h"

class Poly
{
private:
	const PolyRing& ring;
	std::vector<uint16_t> coeffs;
public:
	Poly(const PolyRing& ring);

	void ntt();
	void invntt();
	void print(const char* msg) const;
	
	const PolyRing& getRing();

	friend const Poly operator* (const Poly& left, const Poly& right);
	friend const Poly operator+ (const Poly& left, const Poly& right);
	Poly& operator+= (const Poly& right);
	friend const Poly operator^ (const Poly& left, const Poly& right);
	Poly& operator^= (const Poly& right);
	friend const Poly operator- (const Poly& left, const Poly& right);
	Poly& operator-= (const Poly& right);
	const Poly& operator= (const Poly& right);
	uint16_t& operator[] (int i);
	const uint16_t& operator[] (int i) const;
	
	friend bool operator== (const Poly& left, const Poly& right);
	friend bool operator!= (const Poly& left, const Poly& right);
};

