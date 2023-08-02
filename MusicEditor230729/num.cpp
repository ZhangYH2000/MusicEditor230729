#include "num.h"
#include "cur.h"
#include "par.h"
#include "env.h"
#include "lfo.h"
#include "inst.h"

#define pars (cur.pars)

Num::Num(double num) : num(num) {}
Num::Num(Cur& cur, Var const& v) {
	if (v.typ == L"num") { type = TYP_NUM; num = v.num; }
	else if (v.typ == L"vec") {
		auto& vs = v.vec; double bg = 0;
		if (vs.size() < 2) { return; }
		if (vs[0]->str == L"par") { type = TYP_PAR; par = vs[1]->str; bg = 0; }
		if (vs.size() < 4) { return; }
		double c = vs[2]->num, d = vs[3]->num;
		a = (d - c) / (1 - bg); b = c - bg * a;
	}
}
Num::Num(Cur& cur, Inst& inst, Var const& v) {
	if (v.typ == L"num") { type = TYP_NUM; num = v.num; }
	else if (v.typ == L"vec") { 
		auto& vs = v.vec; double bg = 0;
		if (vs.size() < 2) { return; }
		if (vs[0]->str == L"par") { type = TYP_PAR; par = vs[1]->str; bg = 0; }
		if (vs[0]->str == L"env") { type = TYP_ENV; id = vs[1]->num; bg = 0; }
		if (vs[0]->str == L"lfo") { type = TYP_LFO; id = vs[1]->num; bg = -1; }
		if (vs.size() < 4) { return; }
		double c = vs[2]->num, d = vs[3]->num; 
		a = (d - c) / (1 - bg); b = c - bg * a;
	}
}

#define envs (inst.envs)
#define lfos (inst.lfos)

double Num::val(Cur& cur) const {
	switch (type) {
	case TYP_NUM: return num;
	case TYP_PAR: {
		auto it = find_if(pars.begin(), pars.end(),
			[this](ptr<param> p) { return p->nm == par; });
		return it == pars.end() ? 0 : a * (*it)->val + b;
	}
	} return 0;
}
double Num::val(Cur& cur, Inst &inst) const {
	switch (type) {
	case TYP_NUM: return num;
	case TYP_PAR: {
		auto it = find_if(pars.begin(), pars.end(),
			[this](ptr<param> p) { return p->nm == par; });
		return it == pars.end() ? 0 : a * (*it)->val + b;
	}
	case TYP_ENV: return id < envs.size() ?
		a * envs[id]->val(cur, inst) + b : 0;
	case TYP_LFO: return id < lfos.size() ?
		a * lfos[id]->val(cur, inst) + b : 0;
	} return 0;
}
