#include "track.h"
#include "fx.h"
#include "cur.h"
#include "var.h"
#include "num.h"
#include "drum.h"
#include "inst.h"
#include "nrect.h"

Track::Track(wstring const& nm) : nm(nm) { mkp(inst)(); }

#include "my_def.h"
#define nbt (cur.n_beat)

// 理想的是把 Track, Singer, Inst, DrumMachine 分开。
void Track::save(FILE* f) const {
	int sz = 0; fwtv(nm); fwtv(str); 
	sz = 0; for (auto& a : sht) { sz += a.size(); } fwt(sz);
	for (auto& a : sht) for (auto nr : a) { nr->save(f); }
}
Track::Track(Cur &cur, FILE* f) {
	int sz = 0; frdv(nm); frdv(str);
	tmp_str = str; frd(sz); Build(cur);
	rep(i, 0, sz) { add_nr(cur, msh<NRect>(f)); }
}

void Track::reset_p(Cur& cur) {
	if (drum) { cur.dm->reset_p(cur); }
	else { 
		pinsts.clear(); 
		for (auto f : pfxs) { f->reset(cur); }
	}
}
void Track::reset_np(Cur &cur) {
	if (drum) { cur.dm->reset_np(cur); }
	else { 
		insts.clear();
		for (auto f : fxs) { f->reset(cur); }
	}
}
void Track::add_nr(Cur &cur, ptr<NRect> nr) {
	int id = max<int>(0, floor(nr->t0 / nbt));
	rep(i, sht.size(), id + 1) { sht.push_back({}); }
	sht[id].push_back(nr);
}

#define sp (cur.wv.sp)
#define tp (cur.tempo)
#define t_play (cur.t_play)

double Track::Play(Cur& cur) {
	if (!drum_member) {
		double c = tp / (2 * nbt), t_next = t_play + c / sp;
		int a = max<int>(0, floor(t_play / nbt));
		int b = ceil(t_play / nbt) + 2;
		rep(i, a, min<int>(b, sht.size())) for (auto nr : sht[i])
		if (t_play <= nr->t0 && nr->t0 < t_next) {
			if (!drum) {
				auto tmp = msh<Inst>(*inst);
				tmp->Set(nr->tone, (nr->t1 - nr->t0) / c); insts.push_back(tmp);
			} else if (cur.tkdm == this) {
				auto d = cur.dm->get(cur, nr->tone);
				auto tmp = msh<Inst>(*d->inst);
				tmp->Set(nr->tone, (nr->t1 - nr->t0) / c); d->insts.push_back(tmp);
			}
		}
	}

	if (drum) { return cur.tkdm == this ? cur.dm->Play(cur) : 0; }
	double out = 0;
	for (auto inst : insts) if (!inst->end) { out += inst->Sample(cur); }
	for (auto f : fxs) { out = f->loop(cur, out); } 
	insts.erase(remove_if(insts.begin(), insts.end(),
		[](ptr<Inst> inst) { return inst->end; }), insts.end()); return out;
}
double Track::PlayPiano(Cur& cur) {
	if (drum) { return cur.tkdm == this ? cur.dm->PlayPiano(cur) : 0; }
	double out = 0;
	for (auto inst : pinsts) { out += inst->Sample(cur); }
	for (auto f : pfxs) { out = f->loop(cur, out); }
	pinsts.erase(remove_if(pinsts.begin(), pinsts.end(),
		[](ptr<Inst> inst) { return inst->end; }), pinsts.end()); return out;
}
void Track::Build(Cur& cur) {
	insts.clear(); pinsts.clear();
	fxs.clear(); pfxs.clear(); drum = false;
	if (cur.tkdm == this) { 
		cur.tkdm = NULL;
		cur.dm->reset_np(cur); cur.dm->reset_p(cur);
	}

	sc[L"fx"] = msh<Var>();
	sc[L"osc"] = msh<Var>();
	sc[L"lfo"] = msh<Var>();
	sc[L"env"] = msh<Var>();
	sc[L"vol"] = msh<Var>(1.0);
	sc[L"drum"] = msh<Var>(0);
	sc[L"fixed_tone"] = msh<Var>();
	Execute(sc, Compile(str));
	mkp(inst)();
	
	

#define find(nm) sc.find(L#nm) != sc.end()
	if (find(fx)) {
		auto& vs = sc[L"fx"]->vec;
		for (auto v : vs) { 
			fxs.push_back(msh<FX>(cur, *v)); 
			pfxs.push_back(msh<FX>(cur, *v));
		}
	} 
#define get(nm) if(find(nm)) { nm = sc.at(L#nm)->num; }
	get(drum);
#undef get
	if (drum && !cur.tkdm) { 
		cur.tkdm = this;
		if (find(vol)) { cur.dm->vol = Num(cur, *sc.at(L"vol")); }
	}

#undef find
	if (!drum) { mkp(inst)(cur); }
	sc[L"vol"] = msh<Var>(cur.vol);
}
void Track::Update(Cur& cur) { 
	for (auto &nrs : sht) for (auto nr : nrs) { nr->Update(cur); }
}
void Track::Discard(Cur& cur) {
	for (auto& nrs : sht) for (auto nr : nrs) { nr->Discard(cur); }
}
void Track::PreUpdate(Cur& cur) { 
	for (auto& nrs : sht) for (auto nr : nrs) { nr->PreUpdate(cur); }
}
