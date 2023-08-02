#pragma once
#include "num.h"

struct Var;
struct Cur;
struct Inst;
struct Env {
	Num attack, hold, decay, sustain, release;
	Num a_exp, d_exp, r_exp;

	Env(Cur& cur, Inst& inst, Var const& v);
	double val(Cur& cur, Inst& inst) const;
};
