#include "env.h"
#include "var.h"
#include "inst.h"

Env::Env(Cur& cur, Inst& inst, Var const& v) {
#define find(nm) v.dic.find(L#nm) != v.dic.end()
	a_exp = 1; d_exp = 1; r_exp = 1;
#define get(nm) if(find(nm)) { nm = Num(cur, *v.dic.at(L#nm)); }
	get(attack); get(hold); get(decay); get(sustain); get(release);
	get(a_exp); get(d_exp); get(r_exp);
#undef get

#undef find
}

double Env::val(Cur& cur, Inst& inst) const {
	double a = attack.val(cur);
	double h = hold.val(cur);
	double d = decay.val(cur);
	double s = sustain.val(cur);
	double r = release.val(cur);
	double t = inst.t, tr = inst.t_release;
	double ae = a_exp.val(cur);
	double de = d_exp.val(cur);
	double re = r_exp.val(cur);
	if (t >= tr) { t -= tr;
		double c = max(0.0, 1 - pow(t / r, re));
		if (tr < a) { return pow(tr / a, ae) * c; ; } tr -= a;
		if (tr < h) { return c;} tr -= h;
		if (tr < d) { return (1 + (s - 1) * pow(tr / d, de)) * c; } return s * c;
	}
	if (t < a) { return pow(t / a, ae); } t -= a;
	if (t < h) { return 1; } t -= h;
	if (t < d) { return 1 + (s - 1) * pow(t / d, de); } return s;
}
