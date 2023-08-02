#include "inst.h"
#include "cur.h"
#include "osc.h"
#include "env.h"
#include "lfo.h"

#include "my_def.h"
#define sp (cur.wv.sp)

Inst::Inst(Cur &cur) {
#define find(nm) sc.find(L#nm) != sc.end()

#define get(nm) if(find(nm)) { nm = Num(cur, *this, *sc.at(L#nm)); }
	get(vol);
#undef get

#define get(nm) if (find(nm)) { nm = sc.at(L#nm)->num; }
	get(fixed_tone);
#undef get

	if (find(lfo)) {
		auto& vs = sc[L"lfo"]->vec;
		for (auto v : vs) { lfos.push_back(msh<LFO>(cur, *this, *v)); }
	}
	if (find(env)) {
		auto& vs = sc[L"env"]->vec;
		for (auto v : vs) { envs.push_back(msh<Env>(cur, *this, *v)); }
	}
	if (find(osc)) {
		auto& vs = sc[L"osc"]->vec;
		for (auto v : vs) { oscs.push_back(msh<Osc>(cur, *this, *v)); }
	}

	// 懒得再去考虑主音量了。
	for (auto osc : oscs) if (osc->vol.type == TYP_ENV) 
	if (osc->vol.id < envs.size()) {
		auto& r = envs[osc->vol.id]->release;
		t_end_rel = max(t_end_rel, r.type == TYP_NUM ? r.num : r.a + r.b);
		// 这里隐含了 a 必须为正数。
	}
#undef find
}
Inst::Inst(Inst const& inst) {
#define copy(nm) nm = inst.nm;
	copy(tone); copy(t_end_rel); copy(t_release);
	copy(vol); copy(fixed_tone); copy(t); copy(end);
#undef copy
	for (auto a : inst.lfos) { lfos.push_back(msh<LFO>(*a)); }
	for (auto a : inst.envs) { envs.push_back(msh<Env>(*a)); }
	for (auto a : inst.oscs) { oscs.push_back(msh<Osc>(*a)); }
}
void Inst::Set(int tn, double tr) { tone = tn; t_release = tr; }

double Inst::Sample(Cur& cur) {
	if (end) { return 0; } t += 1.0 / sp;
	if (t > t_release + t_end_rel) { end = true; }
	double out = 0;
	for (auto o : oscs) { out += vol.val(cur, *this) * o->Sample(cur, *this); }
	return out;
}
