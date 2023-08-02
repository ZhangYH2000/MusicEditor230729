#include "complex.h"

void fft(vector<complex>& cs, int a, int n) {
	if (n == 1) { return; }
	int m = n / 2;
	vector<complex> tmp = vector<complex>(cs.begin() + a, cs.begin() + a + n);
	rep(i, 0, m) {
		cs[a + i] = tmp[2 * i];
		cs[a + m + i] = tmp[2 * i + 1];
	}
	fft(cs, a, m); fft(cs, a + m, m);
	tmp = vector<complex>(cs.begin() + a, cs.begin() + a + n);
	rep(i, 0, m) {
		double phi = -2 * PI * i / n;
		complex c(cos(phi), sin(phi));
		cs[a + i] = tmp[i] + c * tmp[m + i];
		cs[a + m + i] = tmp[i] - c * tmp[m + i];
	}
}
