#pragma once
#include "utils.h"

struct complex;
inline complex operator*(complex a, complex b);
inline complex operator/(complex a, complex b);
struct complex {
	double x = 0, y = 0;

	complex(double x = 0, double y = 0) : x(x), y(y) {}
	complex operator+() const { return *this; }
	complex operator-() const { return { -x, -y }; }
	complex& operator+=(complex a) { x += a.x; y += a.y; return *this; }
	complex& operator-=(complex a) { return *this += -a; }
	complex& operator*=(complex a) { return *this = *this * a; }
	complex& operator/=(complex a) { return *this = *this / a; }

	bool zero() const { return x == 0 && y == 0; }
	complex unit() const {
		return zero() ? complex{} : *this / len();
	}
	complex conj() const { return { x, -y }; }
	double len() const { return sqrt(lensqr()); }
	double lensqr() const { return x * x + y * y; }
};
inline complex operator+(complex a, complex b) { return a += b; }
inline complex operator-(complex a, complex b) { return a -= b; }
inline complex operator*(complex a, complex b) {
	return {
		a.x * b.x - a.y * b.y,
		a.x * b.y + a.y * b.x
	};
}
inline complex operator/(complex a, complex b) {
	double c = 1 / b.lensqr();
	return {
		c * (+a.x * b.x + a.y * b.y),
		c * (-a.x * b.y + a.y * b.x)
	};
}
inline bool operator==(complex a, complex b) {
	return memcmp(&a, &b, sizeof(complex)) == 0;
}
inline bool operator!=(complex a, complex b) { return !(a == b); }

void fft(vector<complex>& cs, int a, int n);
