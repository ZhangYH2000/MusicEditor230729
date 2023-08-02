#pragma once
#include "utils.h"

int constexpr TYP_NUM = 0;
int constexpr TYP_PAR = 1;
int constexpr TYP_ENV = 2;
int constexpr TYP_LFO = 3;

struct Var;
struct Cur;
struct Inst;
struct Num {
	int type = 0;

	double num = 0;
	double a = 0, b = 0;
	int id = 0;
	wstring par;

	Num(double num = 0);
	Num(Cur& cur, Var const& v);
	Num(Cur& cur, Inst &inst, Var const& v);
	double val(Cur& cur) const;
	double val(Cur& cur, Inst &inst) const;
};

