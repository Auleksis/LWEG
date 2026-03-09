#pragma once
#include "polyVector.h"

class PolyMatrix
{
private:
	const PolyRing& ring;
	std::vector<std::vector<Poly>> matrix;
	int size;
public:
	PolyMatrix(const PolyRing& ring, int size);

	void ntt();
	void invntt();
	void transpose();
	void print(const char* prefix);

	Poly& operator() (int i, int j);

	const Poly& operator() (int i, int j) const;

	int getSize() const;
	const PolyRing& getRing() const;
};

PolyVector operator* (const PolyMatrix& left, const PolyVector& right);
