#include "nrect.h"
#include "cur.h"
#include "bgr.h"
#include "track.h"
#include "draw_comp.h"

NRect::NRect(int tone, double t0, double t1) : 
	tone(tone), t0(t0), t1(t1) { dep = 1; 
	c_frame = { 150, 150, 150 };
	c_normal = { 80, 80, 80 };
	c_hovered = { 60, 60, 150 };
	c_selected = { 150, 60, 60 };
	c_hv_sel = { 150, 60, 150 };
	c_overlap = { 150, 150, 60 };
	c_rect_sel = { 60, 150, 60 };
}
void NRect::save(FILE* f) const { fwt(tone); fwt(t0); fwt(t1); }
NRect::NRect(FILE* f) : NRect(0, 0, 0) { frd(tone); frd(t0); frd(t1); }

#include "my_def.h"
#define sdx(x) (bgr.sdx(cur, x))
#define sdy(x) (bgr.sdy(cur, x))
#define sdbx(x) (bgr.sdbx(cur, x))
#define sdby(x) (bgr.sdby(cur, x))
#define nbt (cur.n_beat)
#define vpsht (bgr.vp_sht())
#define nrsel (cur.nrsel)
#define tksel (*cur.tksel)

double NRect::d() const { return selected ? dep + 1 : dep; }
int NRect::w(Cur& cur) const { return (t1 - t0) * bgr.w_grid; }
dvec NRect::tl(Cur& cur) const { return { sdx(t0), sdy(tone) }; }
bool NRect::rect_seled(Cur& cur) const {
	auto const& rc = bgr.rc;
	return bgr.rect_sel && insd({ sdx(t0), sdy(tone) }, rc);
}
bool NRect::overlapped(Cur& cur) const {
	// 不知道搞这么多优化有没有用。
	if (t0 > sdbx(vpsht.right()) || t1 < sdbx(vpsht.left())) { return false; }
	// 默认了所在音轨是被选中的。2 是随便的一个数字。
	int a = max<int>(0, floor(t0 / nbt) - 2);
	int b = min<int>(ceil(t1 / nbt) + 2, tksel.sht.size());
	rep(i, a, b) for (auto nr : tksel.sht[i]) if (&*nr != this)
	if (nr->t0 < t1 && nr->t1 > t0 && nr->tone == tone) { return true; } 
	return false;
}

void NRect::render(Cur& cur) {
	dcol const& c = 
		selected ? (hovered ? c_hv_sel : c_selected) : 
		hovered ? c_hovered : 
		rect_seled(cur) ? c_rect_sel :
		overlapped(cur) ? c_overlap : c_normal;
	draw_px_rect_framed(scr, dscr, d(), tl(cur), w(cur), bgr.h_grid, vpsht, c, c_frame);
}
void NRect::Update(Cur& cur) {
	hovered = (hvd == this);
	selected = find(nrsel.begin(), nrsel.end(), this) != nrsel.end();
	if (hovered && msc(2)) { del = true; }
	if (hovered && msc(0) && !bgr.nr_repos) {
		if (selected) { msp_rel = {};
			nrsel.erase(remove(nrsel.begin(), nrsel.end(), this), nrsel.end());
		} else { nrsel.push_back(this); }
	} render(cur);
}
void NRect::Discard(Cur& cur) { rmv; hovered = selected = false; msp_rel = {}; }
void NRect::PreUpdate(Cur& cur) {
	bool ok = dhv <= d() &&
		insd(msp, { tl(cur), w(cur), bgr.h_grid }) && insd(msp, vpsht);
	if (ok) { dhv = dep; hvd = this; }
}
