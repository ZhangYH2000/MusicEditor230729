#include "cur.h"
#include "ui.h"
#include "fx.h"
#include "bgr.h"
#include "par.h"
#include "osc.h"
#include "playhead.h"
#include "inst.h"
#include "drum.h"
#include "nrect.h"
#include "track.h"
#include "draw_geo.h"

bool keytone(BYTE key, int& tone) {
	switch (key) {
	case 'Q': tone = 0; return true;
	case 'W': tone = 2; return true;
	case 'E': tone = 4; return true;
	case 'R': tone = 5; return true;
	case 'T': tone = 7; return true;
	case 'Y': tone = 9; return true;
	case 'U': tone = 11; return true;
	case 'I': tone = 12; return true;
	case 'O': tone = 14; return true;
	case 'P': tone = 16; return true;

	case '2': tone = 1; return true;
	case '3': tone = 3; return true;
	case '5': tone = 6; return true;
	case '6': tone = 8; return true;
	case '7': tone = 10; return true;
	case '9': tone = 13; return true;
	case '0': tone = 15; return true;

	case 'Z': tone = -12; return true;
	case 'X': tone = -10; return true;
	case 'C': tone = -8; return true;
	case 'V': tone = -7; return true;
	case 'B': tone = -5; return true;
	case 'N': tone = -3; return true;
	case 'M': tone = -1; return true;

	case 'S': tone = -11; return true;
	case 'D': tone = -9; return true;
	case 'G': tone = -6; return true;
	case 'H': tone = -4; return true;
	case 'J': tone = -2; return true;
	}
	return false;
}

Cur::Cur() {
	w = 1800; h = 900; set_locale(); wv.n_safe = 256;
	print_console(L"正在加载字体...");
	App::Init();
	print_console(L"字体加载完成.", true);
	print_console(L"正在加载控件...");
	// 涉及到传自己的指针，所以创建之后不能复制了。
	mkp(ui)(*this); ui->Init(*this);
	mkp(bgr)(*this); Reset();
	mkp(ph)();
	print_console(L"控件加载完成.", true);
}

#define cur (*this)
#include "my_def.h"
#undef dt
#define sp (cur.wv.sp)
#define nbt (cur.n_beat)

void Cur::Play() {
	double dt = tempo / (2 * nbt * sp);
	for (auto& out : wv.wvin) { 
		for (auto tk : tks) if (tk->show) {	out += vol * tk->Play(*this); }
		t_play += dt;

		if (t_play > t_stop && replay) {
			t_play = 0;
			for (auto tk : cur.tks) { tk->reset_np(cur); }
		}
	}
}
void Cur::Save(wstring const& nm) const{
	FILE* f = wfopen(nm, L"wb");
	if (!f) { return; } 
	int sz = 0; fwtv(cmd); 
	save_par_fun(f); 
	dm->save(f); sz = tks.size(); fwt(sz);
	for (auto tk : tks) { tk->save(f); }
	fclose(f);
}
void Cur::Load(wstring const& nm) {
	FILE* f = wfopen(nm, L"rb");
	if (!f) { return; } Reset(); 
	int sz = 0; frdv(cmd); tmp_cmd = cmd;
	load_par_fun(f);
	tks.clear();
	mkp(dm)(*this, f); frd(sz);
	rep(i, 0, sz) { tks.push_back(msh<Track>(*this, f)); }
	tksel = &*tks[0];
	Execute(sc, Compile(cmd)); fclose(f);
}
void Cur::Reset() {
	n_oct = 8; n_beat = 8; base_oct = 4;
	tempo = 128; vol = 1; transpose = 0;
	fft_scl = 1; t_stop = 0; replay = false;
	play = false; t_play = 0;

	tks.clear(); tkdm = NULL;
	tks.push_back(msh<Track>(loc(L"unnamed_track")));
	tksel = &*tks[0]; nrsel.clear();
	cs.clear(); cs.resize(1024 * 8); mkp(dm)(*this);

	sc.clear(); init_def_fun();
	pars.clear(); funs.clear();
}
void Cur::Update() {
	bgr.PreUpdate(*this);
	ph->PreUpdate(*this);
	ui.PreUpdate(*this);
	tksel->PreUpdate(*this);
	basic_update();

#define get(nm) if (sc.find(L#nm) != sc.end()) { nm = sc.at(L#nm)->num; }
	get(tempo); get(vol); get(transpose); get(fft_scl); get(t_stop); get(replay);
#undef get

	sc[L"t"] = msh<Var>(dur);
	sc[L"t_play"] = msh<Var>(t_play);

	if (!kb && kbd[L' ']) { play = !play; }

	rep(i, 0, 256) {
		int tone = 0;
		if (!keytone(i, tone)) { continue; }
		if (!kb && kbc(i) && !piano[i] && !kbd[VK_CONTROL]) {
			if (!tksel->drum) {
				auto tmp = msh<Inst>(*tksel->inst);
				tmp->Set(tone, DBL_MAX);
				tksel->pinsts.push_back(tmp);
				piano[i] = &*tmp;
			} else if (tksel == tkdm) {
				auto tmp = msh<Inst>(*dm->get(cur, tone)->inst);
				tmp->Set(tone, DBL_MAX);
				dm->get(cur, tone)->pinsts.push_back(tmp);
				piano[i] = &*tmp;
			}
		} else if (piano[i] && (kb || !kbd[i])) {
			piano[i]->t_release = piano[i]->t;
			piano[i] = NULL;
		}
	}
	for (auto& out : wv.wvin) 
	for (auto tk : tks) { out += vol * tk->PlayPiano(*this); }

	nrsel.erase(remove_if(nrsel.begin(), nrsel.end(),
		[](NRect* nr) { return nr->del; }), nrsel.end());
	for (auto tk : tks) for (auto& nrs : tk->sht) {
		for (auto nr : nrs) if (nr->del) { nr->Discard(*this); }
		nrs.erase(remove_if(nrs.begin(), nrs.end(),
			[](ptr<NRect> nr) { return nr->del; }), nrs.end());
	}

	if (tkdm && tkdm->del) { tkdm = NULL; dm->reset_np(*this); dm->reset_p(*this); }
	if (tksel->del) { tksel = NULL; }
	for (auto tk : tks) if (tk->del) { tk->Discard(*this); }
	tks.erase(remove_if(tks.begin(), tks.end(),
		[](ptr<Track> tk) {return tk->del; }), tks.end());
	if (tksel == NULL) { tksel = &*tks[0]; }

	if (play) { Play(); }
	for (auto v : wv.wvin) { cs.push_back(v); cs.pop_front(); }

	bgr.Update(*this);
	ph->Update(*this);
	tksel->Update(*this);
	ui.Update(*this);
	freq_analysis();
}

void Cur::zero_mean(wstring const& nm) {
	auto it = funs.find(nm);
	if (it != funs.end()) {
		double av = 0;
		for (auto& y : *it->second) { av += y; }
		av /= it->second->size();
		for (auto& y : *it->second) { y -= av; }
	}
}
void Cur::get_phase(int id) {
	wstring s = L"[";
	auto& os = tksel->inst->oscs;
	if (!insd<int>(id, 0, os.size())) { return; }
	for (auto& m : os[id]->ms) { s += tw(m.ph) + L","; }
	s += L"]"; set_clipboard(s);
}

void Cur::freq_analysis() {
	if (!show_freq) { return; }
	auto vs = vector<complex>(cs.begin(), cs.end());
	fft(vs, 0, vs.size());
	int n = cs.size() / 2;
	double lgmin = log2(1.0 / n);
	int x_old = -1; vector<dvec> plot;
	rep(i, 0, n) {
		double x = (1.0 + i) / n;
		x = (1 - log2(x) / lgmin) * w;
		double y = h - fft_scl * 8 * h * vs[i].len() / vs.size();
		if (x != x_old) { plot.push_back(dvec(x,y)); x_old = x; }
	} draw_px_segstrip(scr, dscr, 20000, plot, scr.rect(), dcol(255, 255, 0));
}
void Cur::save_par_fun(FILE* f) const {
	int sz = pars.size(); fwt(sz);
	for (auto p : pars) { p->save(f); }
	sz = funs.size(); fwt(sz);
	for (auto fun : funs) {
		fwtv(fun.first); fwtv(*fun.second);
	}
}
void Cur::load_par_fun(FILE* f) {
	int sz = 0; frd(sz);
	rep(i, 0, sz) { pars.push_back(msh<param>(f)); }
	wstring nm; vector<double> vs; frd(sz);
	rep(i, 0, sz) {
		// 虽然默认函数都是一个尺寸，但为了省事还是分别存上长度。
		frdv(nm); frdv(vs);
		funs[nm] = msh<vector<double>>(vs);
	}
}
void Cur::init_def_fun() {
	auto zm = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 1) { zero_mean(in[0]->str); }
		return msh<Var>();
	};
	sc[L"zero_mean"] = msh<Var>(zm);

	auto gp = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 1) { get_phase(in[0]->num); }
		return msh<Var>();
	};
	sc[L"get_phase"] = msh<Var>(gp);
}
void Cur::basic_update() {
	title = loc(L"title");
	if (sc.find(L"dbstr") != sc.end()) { dbstr = sc[L"dbstr"]->str; }
	if (sc.find(L"update") != sc.end()) { 
		auto f = sc[L"update"];  Execute(sc, f->procs);
	}

	pars.erase(remove_if(pars.begin(), pars.end(),
		[](ptr<param> p) { return p->del; }), pars.end());
	for (auto& p : pars) { sc[p->nm] = msh<Var>(p->val); }

	for (auto& f : funs) {
		auto pys = f.second;
		auto fun = [pys](vector<ptr<Var>> const& in) {
			if (in.empty()) { return msh<Var>(); }
			return msh<Var>(get_val(in[0]->num, *pys));
		}; 
		sc[f.first] = msh<Var>(fun);
	}
}
