#include "bgr.h"
#include "cur.h"
#include "nrect.h"
#include "track.h"
#include "draw_str.h"
#include "draw_geo.h"
#include "draw_tile.h"
#include "draw_comp.h"

Bgr::Bgr(Cur& cur) {
	w = 1630; h = 860;
	tl = { 170, 0 }; dep = -100000;
	black = tile(w, h, dcol{}, 255);

	w_piano = 40; w_grid = 15; h_grid = 15;
	c_num = { 255, 255, 255 };
	c_mouse = { 40, 40, 80 };
	c_sub_frame = { 60, 60, 60 };
	c_main_frame = { 150, 150, 150 };
}

#include "my_def.h"
#define no (cur.n_oct)
#define bo (cur.base_oct)
#define sp (cur.wv.sp)
#define tp (cur.tempo)
#define ntn (cur.n_tone())
#define nbt (cur.n_beat)
#define tna (cur.tone_a())
#define tnb (cur.tone_b())
#define nrsel (cur.nrsel)
#define tksel (*cur.tksel)
#define vpsht (bgr.vp_sht())
#define hvsht (bghvd && insd(msp, vpsht))
#define t_play (cur.t_play)

int Bgr::sdy(Cur& cur, double tn) const {
	return tl.y + (int)tl_sht_rel.y + (tnb - 1 - tn) * h_grid;
}
int Bgr::sdby(Cur& cur, int y) const {
	return tnb - 1 - floor((y - (int)tl_sht_rel.y - tl.y) / h_grid);
}
int Bgr::sdx(Cur& cur, double t) const {
	return tl.x + w_piano + (int)tl_sht_rel.x + t * w_grid;
}
double Bgr::sdbx(Cur& cur, int x) const {
	return double(x - tl.x - w_piano - (int)tl_sht_rel.x) / w_grid;
}
double Bgr::xtta(Cur& cur, int x) const {
	return max(0.0, auto_align ? floor(sdbx(cur, x)) : sdbx(cur, x));
}
double Bgr::xttb(Cur& cur, int x) const {
	return max(0.0, auto_align ? ceil(sdbx(cur, x)) : sdbx(cur, x));
}


#define xta(x) (bgr.xtta(cur, x))
#define xtb(x) (bgr.xttb(cur, x))
#define sdx(x) (bgr.sdx(cur, x))
#define sdy(x) (bgr.sdy(cur, x))
#define sdbx(x) (bgr.sdbx(cur, x))
#define sdby(x) (bgr.sdby(cur, x))

// 之前把 tl_sht_rel 改成了 vec2，导致这里很多麻烦，以后要注意。
void Bgr::render_mouse(Cur& cur) {
	if (hvsht && !msd[0] && !msd[1] && !msd[2]) {
		double t = sdbx(msp.x); int tn = sdby(msp.y);
		if (t >= 0 && insd(tn, tna, tnb)) {
			draw_rect_raw(scr, { tl.x, sdy(tn) }, w, h_grid, vpsht, c_mouse);
			draw_rect_raw(scr, { sdx(floor(t)), tl.y }, w_grid, h, vpsht, c_mouse);
		}
	}
}
void Bgr::render_axis(Cur& cur) {
	rep(i, 0, ntn + 1) {
		dvec p = tl + dvec(w_piano, (int)tl_sht_rel.y + i * h_grid);
		dcol c = i % 12 == 0 ? c_main_frame : c_sub_frame;
		draw_rect_raw(scr, p, w, 1, vpsht, c);
	}
	rep(i, 0, w / w_grid + 1) {
		int tmp = mod((int)tl_sht_rel.x, w_grid) + i * w_grid;
		dvec p = tl + dvec(w_piano + tmp, 0);
		dcol c = (tmp - (int)tl_sht_rel.x) / w_grid % nbt == 0 ? c_main_frame : c_sub_frame;
		if (tmp >= (int)tl_sht_rel.x) { draw_rect_raw(scr, p, 1, h, vpsht, c); }
	}
}
void Bgr::render_piano(Cur& cur) {
	vector<int> wt = { 11, 9, 7, 6, 4, 2, 0 };
	rep(i, 0, ntn) {
		dvec p = tl + dvec(0, (int)tl_sht_rel.y + i * h_grid);
		dcol c = find(wt.begin(), wt.end(), i % 12) != wt.end() ? dcol(100) : dcol();
		// 画边框效率不高，但省事，也会有一些一个像素的细节问题。
		draw_px_rect_framed_raw(scr, p, w_piano - 1, h_grid, scr.rect(), c, c_main_frame);
	}
}
void Bgr::render_nr_crt(Cur& cur) {
	if (nr_crt && t1 > t0) {
		draw_px_rect_framed(scr, dscr, 3, { sdx(t0), sdy(tone) },
			(t1 - t0) * w_grid, h_grid, vpsht, dcol{}, dcol(150));
	}
}
void Bgr::render_number(Cur& cur) {
	rep(i, 0, no + 1) {
		wstring s = tw((no - i - bo) * 12); dvec p;
		p.x = cur.w - str_wh(s, ft, 0).x - 5;
		p.y = tl.y + (int)tl_sht_rel.y + i * h_grid * 12 + 1;
		draw_str(scr, dscr, 999, s, c_num, ft, p, 0, vpsht);
	}
	rep(i, 0, w / w_grid / nbt + 1) {
		int tmp = mod((int)tl_sht_rel.x, w_grid * nbt) + i * w_grid * nbt;
		if (tmp >= (int)tl_sht_rel.x) {
			// 可能有更简单的做法，但这样省脑子。
			wstring s = tw((tmp - (int)tl_sht_rel.x) / w_grid); dvec p;
			p.x = tl.x + w_piano + tmp + 1; p.y = h - str_wh(s, ft, 0).y - 5;
			draw_str(scr, dscr, 999, s, c_num, ft, p, 0, vp_sht());
		}
	}
}

void Bgr::render(Cur& cur) {
	draw_tile_raw(scr, tl, scr.rect(), black, black.rect());
	render_mouse(cur); render_axis(cur); render_piano(cur);
	render_nr_crt(cur); render_number(cur);

	if (rect_sel) { 
		draw_px_rect_frame(scr, dscr, 4, rc.tl, rc.w, rc.h, vpsht, dcol(255, 0, 0)); 
	}

	draw_str(scr, dscr, 999, dbstr,
		dcol(255), ft, tl + dvec(10, 10), w - 20, bgr.vp());
}

// 遇到一个问题，就是拖动时鼠标无意间会被其它东西捕获。
// 是否可以添加一个 MouseOwner, 我觉得其实可以。但有点麻烦了。
// hovered 动画鼠标没有键按下时播放，我觉得这个最好。
void Bgr::Update(Cur& cur) {
	bool nr_selected = !nrsel.empty();
	if (sht_repos) { tl_sht_rel += vec2(msp - msp_old); sht_repos = msd[1]; }
	else { sht_repos = msc(1) && bgwhd && !nr_selected; }
	if (nr_repos) {
		int a = vpsht.left() + 200;
		int b = vpsht.right() - 200;
		int c = vpsht.top() + 200;
		int d = vpsht.bottom() - 200;
		tl_sht_rel.x += max(0, a - msp.x) * dt * 8;
		tl_sht_rel.x += min(0, b - msp.x) * dt * 8;
		tl_sht_rel.y += max(0, c - msp.y) * dt * 8;
		tl_sht_rel.y += min(0, d - msp.y) * dt * 8;
		// 看起来好复杂，不知道怎么改进。
		for (auto& nr : nrsel) { dvec p = nr->msp_rel + msp;
			nr->tone = clmp(sdby(p.y), tna, tnb); 
			double t0 = xta(p.x);
			nr->t1 += t0 - nr->t0; auto old = nr->t0; nr->t0 = t0;
			auto tmp = msh<NRect>(*nr);
			int id = max(0.0, floor(old / nbt)); auto& nrs = tksel.sht[id];
			nrs.erase(remove_if(nrs.begin(), nrs.end(),
				[nr](ptr<NRect> _nr) { return nr == &*_nr; }), nrs.end());
			nr = &*tmp; tksel.add_nr(cur, tmp);
		} nr_repos = msd[1];
	} else if (msc(1) && bgwhd && nr_selected) { nr_repos = true;
		for (auto nr : nrsel) { nr->msp_rel = nr->tl(cur) - msp; }
	}
	if (nr_crt) { t1 = xtb(msp.x); 
		if (!msd[0]) {
			if (t1 > t0) { tksel.add_nr(cur, msh<NRect>(tone, t0, t1)); }
			nr_crt = false; tone = t0 = t1 = 0;
		}
	} else { int tn = sdby(msp.y);
		double _t0 = xta(msp.x), _t1 = xtb(msp.x);
		if (hvsht && _t0 >= 0 && insd(tn, tna, tnb) 
			&& msc(0) && !nr_selected && !begin_rect_sel) {
			nr_crt = true; tone = tn; t0 = _t0; t1 = _t1;
		}
	} 
	if (msc(0) && bghvd && nr_selected && !begin_rect_sel) {
		for (auto nr : nrsel) { nr->msp_rel = {}; } nrsel.clear();
	}
	if (rect_sel) {
		rc.w = max(0, msp.x - rc.tl.x);
		rc.h = max(0, msp.y - rc.tl.y);
		if (!msd[0]) { rect_sel = false; 
			int tn0 = sdby(rc.bottom());
			int tn1 = sdby(rc.top()) + 1;
			double t0 = sdbx(rc.left());
			double t1 = sdbx(rc.right());
			int a = max<int>(0, floor(t0 / nbt));
			int b = min<int>(ceil(t1 / nbt), tksel.sht.size());
			rep(i, a, b) for (auto nr : tksel.sht[i])  
			if (insd(nr->t0, t0, t1) && insd(nr->tone, tn0, tn1))
			if (find(nrsel.begin(), nrsel.end(), &*nr) == nrsel.end()) {
				nrsel.push_back(&*nr);
			} rc = {};
		}
	} else if (msc(0) && begin_rect_sel) {
		// 这也是要注意的，因为它的值改了，所以在它之后不能检查它了。
		begin_rect_sel = false; rect_sel = true;
		rc = drect(msp, 0, 0);
	}
	if (kbd[VK_CONTROL] && kbc(L'V')) {
		for (auto nr : nrsel) { tksel.add_nr(cur, msh<NRect>(*nr)); }
	}
	if (kbd[VK_DELETE]) {
		for (auto nr : nrsel) { nr->del = true; }
	}

	render(cur);
}
void Bgr::PreUpdate(Cur& cur) {
	bool ok = dhv <= dep && insd(msp, vp());
	if (ok) { dhv = dep; hvd = this; }
	ok = dwh <= dep && insd(msp, vp());
	if (ok) { dwh = dep; whd = this; }
}
