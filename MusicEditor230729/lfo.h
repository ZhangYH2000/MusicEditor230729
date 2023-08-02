#pragma once
#include "num.h"

int constexpr LFO_SIN = 0;
int constexpr LFO_SQR = 1;
int constexpr LFO_TRI = 2;
int constexpr LFO_SAW = 3;

struct Var;
struct Cur;
struct Inst;
struct LFO {
	Num freq;
	int type = 0;
	bool user_shape = false;
	wstring nm_fun;
	bool sheet_time = false;

	LFO(Cur& cur, Inst& inst, Var const& v);
	double val(Cur& cur, Inst& inst) const;
};
