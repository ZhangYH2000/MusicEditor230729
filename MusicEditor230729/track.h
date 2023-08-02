#pragma once
#include "utils.h"

struct FX;
struct Cur;
struct Inst;
struct NRect;
struct Track {
	bool drum = false;
	bool drum_member = false;
	wstring nm;
	ptr<Inst> inst;
	wstring str, tmp_str;
	vector<ptr<Inst>> insts, pinsts;
	vector<ptr<FX>> fxs, pfxs;
	vector<vector<ptr<NRect>>> sht;

	bool del = false;
	bool show = true;

	Track(wstring const& nm = L"");
	void save(FILE* f) const;
	Track(Cur &cur, FILE* f);
	void reset_p(Cur& cur);
	void reset_np(Cur& cur);
	void add_nr(Cur &cur, ptr<NRect> nr);
	
	double Play(Cur& cur);
	double PlayPiano(Cur& cur);
	void Build(Cur& cur);
	void Update(Cur& cur);
	void Discard(Cur& cur);
	void PreUpdate(Cur& cur);
};
