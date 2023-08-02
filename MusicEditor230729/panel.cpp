#include "panel.h"
#include "app.h"
#include "control.h"
#include "draw_comp.h"
#include "scrollbar.h"

#include "ctrl_def.h"

Panel::Panel() {
	dep = 1000;
	wheel_mtp = 40;
	tl_show_rel = { 10, 10 };

	c_frame = { 150, 150, 150 };
	c_panel = { 0, 0, 0 };
}
// �˵���ť�Ŀ���: 150.
Panel Panel::menu(App& app) {
	Panel pn; pn.vp = scr.rect();
	pn.w = 169; pn.h = app.h - 40;
	pn.h_show = pn.h - 20;

	drect tmp;
	tmp.tl = dvec(10 - 2, 10 - 2);
	tmp.w = pn.w - 20 + 4; tmp.h = pn.h - 20 + 4;
	pn.vp_show_raw_rel = tmp; return pn;
}
Panel Panel::bottom(App& app) {
	Panel pn; pn.vp = scr.rect();
	pn.w = app.w; pn.h = 40;
	pn.tl.y = app.h - pn.h;
	pn.h_show = pn.h;

	drect tmp;
	tmp.tl = dvec(10 - 2, 10 - 2);
	tmp.w = pn.w - 20 + 4; tmp.h = pn.h - 20 + 4;
	pn.vp_show_raw_rel = tmp; return pn;
}

int Panel::min_y() const {
	return tl.y + tl_show_rel.y + h_show - c->GetH();
}
drect Panel::vp_show() const {
	drect vp_show_raw = vp_show_raw_rel;
	vp_show_raw.tl += tl;
	return overlap(vp, vp_show_raw);
}
void Panel::set_c(App& app, Control* _c) {
	if (c != _c) {
		if (c) { c->Discard(app); } 
		if (_c) { c = _c;
			c->tl = tl + tl_show_rel;
			c->vp = vp_show();
		} sb->c = c;
	}
}
void Panel::init_c() {
	if (c) {
		c->tl = tl + tl_show_rel;
		c->vp = vp_show();
	}
}
void Panel::init_sb() {
	mkp(sb)();
	sb->c = c;
	sb->h = h;
	sb->tl = tl + dvec(w - sb->w, 0);
	sb->vp = vp;
	sb->h_show = h_show;
	sb->top_show = tl.y + tl_show_rel.y;
}
void Panel::render(App &app) {
	draw_px_rect_framed_raw(scr, tl, w, h, vp, c_panel, c_frame);
}

void Panel::Init() { init_c(); init_sb(); }
void Panel::Update(App &app) {
	full = (c != NULL) && h_show < c->GetH();
	hovered = (hvd == this);
	wheeled = (whd == this);

	if (full) {
		int cy = c->tl.y;
		if (wheeled) { cy += msw * wheel_mtp; }
		c->tl.y = clmp(cy, min_y(), tl.y + tl_show_rel.y);
	} else if (c) { c->tl.y = tl.y + tl_show_rel.y; }

	// ���� Update ˳����Ϊ�˷�����ƣ�����Ӧ�ÿؼ��ȸ��¸߶ȵġ�
	// �����Ϊ���ٶȵ���������֪����û�б�Ҫ��
	// һ�����ܵĸĽ���ʽ���Ƿ��� Update �� Render������δ���á�
	render(app);
	if (c) { c->Update(app); }
	if (use_sb) { sb->Update(app); }
}
void Panel::Discard(App& app) { rmv; }
void Panel::PreUpdate(App &app) {
	bool ok = dhv <= dep &&
		insd(msp, { tl, w, h }) && insd(msp, vp);
	if (ok) { dhv = dep; hvd = this; }
	ok = dwh <= dep &&
		insd(msp, { tl, w, h }) && insd(msp, vp);
	if (ok) { dwh = dep; whd = this; }

	if (c) { c->PreUpdate(app); }
	if (use_sb) { sb->PreUpdate(app); }
}