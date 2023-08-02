#pragma once
#include "num.h"

int constexpr WV_SIN = 0;
int constexpr WV_SQR = 1;
int constexpr WV_TRI = 2;
int constexpr WV_SAW = 3;
int constexpr WV_NOI = 4;

int constexpr FM_OSC = 0;
int constexpr FM_ENV = 1;
int constexpr FM_LFO = 2;

struct Cur;
struct Var;
struct Inst;
struct Filter;
struct Mouth {
	double ph = 0;
	double lv = 0;
	Mouth(double ph, double lv);
	double Sample(double dph, int type);
	double Sample(double dph, vector<double> const& ys);
};
struct FM {
	int type = 0;
	int id = 0;
	Num amplitude;

	FM(Cur& cur, Inst& inst, Var const& v);
};

struct Osc {
	int type = 0;
	bool use = true;
	bool user_shape = false;
	wstring nm_fun;
	Num vol, transpose;
	Num detune, detune_power;

	double val = 0;
	vector<FM> fms;
	vector<Mouth> ms;
	vector<ptr<Filter>> fs;

	Osc(Cur &cur, Inst& inst, Var const& v);
	Osc(Osc const& osc);
	double Sample(Cur& cur, Inst& inst);
};
