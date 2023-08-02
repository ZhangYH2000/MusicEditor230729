#include "filter.h"
#include "var.h"
#include "cur.h"
#include "inst.h"

#define sp (cur.wv.sp)

Filter::Filter(Cur& cur, Var const& v) {
#define find(nm) v.dic.find(L#nm) != v.dic.end()

#define get(nm) if(find(nm)) { nm = v.dic.at(L#nm)->num; }
	get(type);
#undef get

#define get(nm) if(find(nm)) { nm = Num(cur, *v.dic.at(L#nm)); }
	get(freq); get(resonance);
#undef get

#undef find
}
Filter::Filter(Cur& cur, Inst& inst, Var const& v) {
#define find(nm) v.dic.find(L#nm) != v.dic.end()

#define get(nm) if(find(nm)) { nm = v.dic.at(L#nm)->num; }
	get(type);
#undef get
	
#define get(nm) if(find(nm)) { nm = Num(cur, inst, *v.dic.at(L#nm)); }
	get(freq); get(resonance);
#undef get

#undef find
}
double Filter::loop(Cur& cur, double in) {
	double f = 2 * sin(PI * freq.val(cur) / sp);
	double rs = resonance.val(cur);
	double fb = rs + rs * (1 - f);

	double hp = in - buf0;
	double bp = buf0 - buf1;
	buf0 = buf0 + f * (hp + fb * bp);
	buf1 = buf1 + f * (buf0 - buf1);

	switch (type) {
	case FLT_LP: return buf1;
	case FLT_HP: return hp;
	case FLT_BP: return bp;
	} return 0;
}
double Filter::loop(Cur& cur, Inst& inst, double in) {
	double f = 2 * sin(PI * freq.val(cur, inst) / sp);
	double rs = resonance.val(cur, inst);
	double fb = rs + rs * (1 - f);

	double hp = in - buf0;
	double bp = buf0 - buf1;
	buf0 = buf0 + f * (hp + fb * bp);
	buf1 = buf1 + f * (buf0 - buf1);

	switch (type) {
	case FLT_LP: return buf1;
	case FLT_HP: return hp;
	case FLT_BP: return bp;
	case FLT_ALT_LP: return buf0;
	case FLT_ALT_HP: return in - buf1;
	} return 0;
}
