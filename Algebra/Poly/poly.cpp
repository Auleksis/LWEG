#include "poly.h"

Poly::Poly(const PolyRing& ring) : ring(ring)
{
	int n = ring.n;
	coeffs = std::vector<uint16_t>(n);
	for (int16_t i = 0; i < n; i++) {
		coeffs[i] = 0;
	}
}

Poly& Poly::operator+=(const Poly& right)
{
	if (ring != right.ring) {
		throw std::invalid_argument("Operands must have the same ring");
	}

	int n = ring.n;
	int q = ring.q;

	for (int i = 0; i < n; i++) {
		coeffs[i] = mod(coeffs[i] + right.coeffs[i], q);
	}

	return *this;
}

Poly& Poly::operator-=(const Poly& right)
{
	if (ring != right.ring) {
		throw std::invalid_argument("Operands must have the same ring");
	}

	int n = ring.n;
	int q = ring.q;

	for (int i = 0; i < n; i++) {
		coeffs[i] = mod(coeffs[i] - right.coeffs[i], q);
	}

	return *this;
}

const Poly& Poly::operator=(const Poly& right)
{
	if (this == &right) {
		return right;
	}

	coeffs = right.coeffs;
	return *this;
}

uint16_t& Poly::operator[](int i)
{
	if (i < 0 || i > coeffs.size()) {
		throw std::invalid_argument("Index is out of range");
	}

	return coeffs[i];
}

const uint16_t& Poly::operator[](int i) const
{
	if (i < 0 || i > coeffs.size()) {
		throw std::invalid_argument("Index is out of range");
	}

	return coeffs[i];
}

void Poly::ntt()
{
	int n = ring.n;
	int q = ring.q;
	int bitLength = log2(n) - 1;

	int i = 1;
	int zeta = 0;
	int t = 0;

	int temp1 = 0;
	int temp2 = 0;

	for (int len = n / 2; len >= 2; len >>= 1) {
		for (int start = 0; start < n; start += 2 * len) {
			zeta = nttZetas[i++];
			for (int j = start; j < start + len; j++) {
				t = mod(zeta * coeffs[j + len], q);
				temp1 = coeffs[j];
				temp2 = coeffs[j + len];
				coeffs[j + len] = mod(temp1 - t, q);
				coeffs[j] = mod(temp1 + t, q);
			}
		}
	}
}

void Poly::invntt()
{
	int n = ring.n;
	int q = ring.q;
	int invHalfN = ring.invHalfN;

	int i = n / 2 - 1;
	int zeta = 0;
	int t = 0;

	int temp1 = 0;
	int temp2 = 0;

	for (int len = 2; len <= n / 2; len *= 2) {
		for (int start = 0; start < n; start += 2 * len) {
			zeta = nttZetas[i--];
			for (int j = start; j < start + len; j++) {
				t = coeffs[j];
				temp1 = coeffs[j];
				temp2 = coeffs[j + len];
				coeffs[j] = mod(t + temp2, q);
				coeffs[j + len] = mod(zeta * (temp2 - t), q);
			}
		}
	}

	for (int i = 0; i < n; i++) {
		coeffs[i] = mod(coeffs[i] * invHalfN, q);
	}
}

void Poly::print(const char* msg) const
{
	printf("\n%s\n", msg);
	printf("[");
	for (int i = 0; i < ring.n; i++) {
		printf("%5d,", coeffs[i]);
	}
	printf("]");
	printf("\nEND OF POLYNOM\n\n");
}

const PolyRing& Poly::getRing()
{
	return ring;
}

//Requires left and right be in NTT form
const Poly operator*(const Poly& left, const Poly& right)
{
	if (!(left.ring == right.ring)) {
		throw std::invalid_argument("Operands must have the same ring");
	}

	Poly c(left.ring);
	int q = left.ring.q;
	int n = left.ring.n;

	int zeta = 0;

	int a = 0;
	int b = 0;

	//Poly: a + bx

	for (int i = 0; i < c.ring.n / 2; i++) {
		int leftA = left.coeffs[2 * i];
		int leftB = left.coeffs[2 * i + 1];

		int rightA = right.coeffs[2 * i];
		int rightB = right.coeffs[2 * i + 1];

		zeta = multiplyZetas[i];

		a = mod(mod(leftA * rightA, q) + mod(mod(leftB * rightB, q) * zeta, q), q);
		b = mod(mod(leftA * rightB, q) + mod(rightA * leftB, q), q);
		c.coeffs[2 * i] = a;
		c.coeffs[2 * i + 1] = b;
	}


	return c;
}

const Poly operator+(const Poly& left, const Poly& right)
{
	if (!(left.ring == right.ring)) {
		throw std::invalid_argument("Operands must have the same ring");
	}

	Poly c(left.ring);
	int q = left.ring.q;
	int n = left.ring.n;

	for (int i = 0; i < n; i++) {
		c.coeffs[i] = mod(left.coeffs[i] + right.coeffs[i], q);
	}

	return c;
}

const Poly operator^(const Poly &left, const Poly &right)
{
    Poly c(left.ring);
	int q = left.ring.q;
	int n = left.ring.n;

	for (int i = 0; i < n; i++) {
		c.coeffs[i] = mod(left.coeffs[i] ^ right.coeffs[i], q);
	}

	return c;
}

Poly &Poly::operator^=(const Poly &right)
{
	int n = ring.n;
	int q = ring.q;

	for (int i = 0; i < n; i++) {
		coeffs[i] = mod(coeffs[i] ^ right.coeffs[i], q);
	}

	return *this;
}

const Poly operator-(const Poly& left, const Poly& right)
{
	if (!(left.ring == right.ring)) {
		throw std::invalid_argument("Operands must have the same ring");
	}

	Poly c(left.ring);
	int q = left.ring.q;
	int n = left.ring.n;

	for (int i = 0; i < n; i++) {
		c.coeffs[i] = mod(left.coeffs[i] - right.coeffs[i], q);
	}

	return c;
}

bool operator==(const Poly& left, const Poly& right)
{
	if (left.ring != right.ring) {
		return false;
	}

	for (int i = 0; i < left.ring.n; i++) {
		if (left[i] != right[i]) {
			return false;
		}
	}

	return true;
}


bool operator!=(const Poly& left, const Poly& right)
{
	if (left.ring != right.ring) {
		return true;
	}

	for (int i = 0; i < left.ring.n; i++) {
		if (left[i] != right[i]) {
			return true;
		}
	}

	return false;
}
