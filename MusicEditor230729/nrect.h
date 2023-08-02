#pragma once
#include "tile.h"

struct Cur;
struct NRect {
	int tone = 0;
	double dep = 0;
	double t0 = 0, t1 = 0;
	dcol c_frame, c_normal, c_hovered, c_selected, c_hv_sel, c_overlap, c_rect_sel;

	dvec msp_rel = {};
	bool del = false;
	bool hovered = false;
	bool selected = false;

	NRect(int tone, double t0, double t1);
	void save(FILE* f) const;
	NRect(FILE* f);
	double d() const;
	int w(Cur& cur) const;
	dvec tl(Cur& cur) const;
	bool rect_seled(Cur& cur) const;
	bool overlapped(Cur &cur) const;

	void render(Cur& cur);
	void Update(Cur& cur);
	void Discard(Cur& cur);
	void PreUpdate(Cur& cur);
};
