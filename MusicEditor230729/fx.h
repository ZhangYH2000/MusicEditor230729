#pragma once
#include "utils.h"

int constexpr FX_NONE = 0;
int constexpr FX_REVERB = 1;

struct Var;
struct Cur;
struct Reverb;
struct FX {
	int type = 0;
	ptr<Reverb> reverb;

	FX(Cur& cur, Var const& v);
	void reset(Cur& cur);
	double loop(Cur& cur, double in);
};
