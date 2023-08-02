#pragma once
#include "num.h"

// 我觉得不将鼓机特殊化，而是搞成专门的一类，也挺好的。
// 现在这样只是懒得重构了。

struct Cur;
struct Inst;
struct Track;
struct DrumMachine {
	Num vol = 0;
	vector<ptr<Track>> ds;

	DrumMachine(Cur& cur);
	void save(FILE *f) const;
	DrumMachine(Cur& cur, FILE* f);

	ptr<Track>& get(Cur& cur, int tn);
	void reset_p(Cur& cur);
	void reset_np(Cur& cur);

	double Play(Cur& cur);
	double PlayPiano(Cur& cur);
};
