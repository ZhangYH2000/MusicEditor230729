#pragma once
#include "vec2.h"
#include "tile.h"

struct Cur;
struct Bgr {
	dvec tl;
	double dep = 0;
	tile black;
	int w = 0, h = 0;

	bool follow_play = false;
	bool auto_align = true;
	int w_piano = 0, w_grid = 0, h_grid = 0;
	dcol c_num, c_mouse, c_sub_frame, c_main_frame;

	vec2 tl_sht_rel;
	int tone = 0;
	bool nr_crt = false;
	double t0 = 0, t1 = 0;
	bool begin_rect_sel = false;
	bool rect_sel = false;
	drect rc;
	bool nr_repos = false;
	bool sht_repos = false;

	Bgr(Cur& cur);
	drect vp() const { return { tl, w, h }; };
	drect vp_sht() const { 
		return { { tl.x + w_piano, tl.y }, w - w_piano, h }; 
	}

	int sdy(Cur& cur, double tn) const;
	int sdby(Cur& cur, int y) const;
	int sdx(Cur& cur, double t) const;
	double sdbx(Cur& cur, int x) const;
	double xtta(Cur& cur, int x) const;
	double xttb(Cur& cur, int x) const;

	void render_mouse(Cur& cur);
	void render_axis(Cur& cur);
	void render_piano(Cur& cur);
	void render_nr_crt(Cur& cur);
	void render_number(Cur& cur);
	void render(Cur& cur);

	void Update(Cur& cur);
	void PreUpdate(Cur& cur);
};
