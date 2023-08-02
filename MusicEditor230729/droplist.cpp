#include "droplist.h"
#include "app.h"
#include "draw_geo.h"
#include "draw_str.h"
#include "draw_comp.h"

#include "ctrl_def.h"

wstring nm_none(int id) { return L"Пе"; }
Droplist::Droplist(int w) : w(w) {
	gap = { 10, 10 }; dep = 1000;
	h_item = 25; h = 30;

	sign_r = 5;
	sign_margin = 15;
	x_text_margin = 5;

	c_txt = { 255, 255, 255 };
	c_mark = { 255, 255, 255 };
	c_edit = { 150, 60, 60 };
	c_frame = { 150, 150, 150 };
	c_normal = { 40, 40, 40 };
	c_invalid = { 0, 0, 0 };
	c_hovered = { 60, 60, 150 };
}
int Droplist::GetW() const { return w + gap.x; }
int Droplist::GetH() const {
	return h + gap.y + (edit ? n_item * h_item : 0);
}

dvec Droplist::tl_item() const { return tl + dvec(0, h); }
void Droplist::render_main(App &app) {
	dcol const& c =
		!enabled ? c_invalid :
		edit ? c_edit :
		hovered ? c_hovered : c_normal;
	draw_px_rect_framed(scr, dscr, dep, tl, w, h, vp, c, c_frame);

	vec2 ct_sign = (vec2)tl + vec2(w - sign_margin, h / 2);
	draw_eclipse(scr, dscr, dep, ct_sign, sign_r, sign_r, vp, c_mark);

	dvec tl_txt = tl + dvec(x_text_margin, (h - ft.h) / 2);
	draw_str(scr, dscr, dep, nm(picked), c_txt, ft, tl_txt, 0, vp);
}
void Droplist::render_items(App& app) {
	if (edit) {
		draw_px_rect_framed(scr, dscr, dep, tl_item(),
			w, h_item * n_item, vp, c_normal, c_frame);
		if (item_hv != -1) {
			dvec tl_hovered = tl + dvec(0, h + h_item * item_hv);
			draw_px_rect_framed(scr, dscr, dep, 
				tl_hovered, w, h_item, vp, c_hovered, c_frame);
		}
		rep(i, 0, n_item) {
			dvec tl_txt_item = tl +
				dvec(0, h + h_item * i) +
				dvec(x_text_margin, (h_item - ft.h) / 2);
			draw_str(scr, dscr, dep, nm(i), c_txt, ft, tl_txt_item, 0, vp);
		}
	}
}
void Droplist::render(App& app) {
	render_main(app); render_items(app);
}

void Droplist::Update(App &app) {
	hovered = (hvd == this);
	if (edit) {
		if (hovered) {
			int tmp = msp.y - tl.y - h;
			item_hv = tmp >= 0 ? tmp / h_item : -1;
		}
		if (enabled && hovered && msc(0)) {
			if (item_hv != -1) { picked = item_hv; } 
			Upload(app);
		}
		if (!enabled || msc(0)) { item_hv = -1; edit = false; }
	} else { Sync(app);
		edit = enabled && hovered && msc(0);
	} render(app);
}
void Droplist::Discard(App &app) {
	rmv; edit = hovered = false; item_hv = -1;
}
void Droplist::PreUpdate(App &app) {
	int real_h = h + (edit ? n_item * h_item : 0);
	bool ok = dhv <= dep &&
		insd(msp, { tl, w, real_h }) && insd(msp, vp);
	if (ok) { dhv = dep; hvd = this; }
}

DlLan::DlLan(int w, function<wstring(int)> const& id_nm, int n) :
	Droplist(w), id_nm(id_nm) { n_item = n; }
void DlLan::Update(App& app) {
	nm = [this, &app](int i) { return loc(id_nm(i)); };
	Droplist::Update(app);
}
