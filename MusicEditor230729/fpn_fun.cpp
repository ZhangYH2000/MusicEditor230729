#include "fpn_fun.h"
#include "ui.h"
#include "cur.h"
#include "draw_geo.h"
#include "draw_comp.h"

#define cur (*(Cur*)&app)
#include "my_def.h"

OtFun::OtFun() {
	dep = 1000; w = 760; h = 470;
	gap = { 10, 10 }; ingap = { 10, 10 };

	c_edit = { 80, 40, 40 };
	c_frame = { 150, 150, 150 };
	c_normal = { 40, 40, 40 };
	c_hovered = { 40, 40, 80 };
	c_fun = { 255, 255, 255 };

	// 本来应该放在 init 里的，但为了省事放这。
	ys.resize(w - 2 * ingap.x);
}
int OtFun::GetW() const { return w + gap.x; }
int OtFun::GetH() const { return h + gap.y; }

void OtFun::render(App &app) {
	dcol c =
		edit ? c_edit :
		hovered ? c_hovered : c_normal;
	int w0 = w - 2 * ingap.x;
	int h0 = h - 2 * ingap.y;
	draw_px_rect_framed_raw(scr, tl, w, h, vp, c, c_frame);
	draw_px_rect_frame(scr, dscr, dep, tl + ingap, w0, h0, vp, c_frame);

	vector<dvec> tmp;
	rep(i, 0, ys.size()) {
		int x = i + tl.x + ingap.x;
		int y = (1 - ys[i]) / 2 * h0 + tl.y + ingap.y;
		tmp.push_back({ x, y });
	} draw_px_segstrip(scr, dscr, dep, tmp, vp, c_fun);
}
void OtFun::Update(App &app) {
	hovered = (this == hvd);
	dvec p = msp - (tl + ingap);
	if (edit) { 
		if (p.x >= 0) {
			double y = clmp(1 - 2.0 * p.y / (h - 2 * ingap.y), -1.0, 1.0);
			if (n_filled == 0) { ys[n_filled] = y; n_filled = 1; }
			int n0 = n_filled - 1, n1 = min<int>(p.x, ys.size() - 1);
			rep(i, n0 + 1, n1 + 1) {
				ys[i] = (y * (i - n0) + ys[n0] * (n1 - i)) / (n1 - n0);
			} n_filled = max(n_filled, n1 + 1);
		} edit = msd[0] && n_filled < ys.size();
	} else if (msc(0) && hovered) { edit = true; n_filled = max(0, p.x); } 
	render(app);
}
void OtFun::Discard(App &app) {
	rmv; n_filled = 0; edit = hovered = false;
}
void OtFun::PreUpdate(App &app) {
	bool ok = dhv <= dep &&
		insd(msp, { tl, w, h }) && insd(msp, vp);
	if (ok) { dhv = dep; hvd = this; }
}

#define nm (ui.fpn_fun->tb_nm_fun->str)
#define ys (ui.fpn_fun->ot_fun->ys)

BtLdFun::BtLdFun() : BtLan(80, L"bt_ld_fun") {}
void BtLdFun::OnClick(App& app) { 
	if (cur.funs.find(nm) != cur.funs.end()) { ys = *cur.funs[nm]; }
}
BtSvFun::BtSvFun() : BtLan(80, L"bt_sv_fun") {}
void BtSvFun::OnClick(App& app) { cur.funs[nm] = msh<vector<double>>(ys); }
BtDelFun::BtDelFun() : BtLan(80, L"bt_del_fun") {}
void BtDelFun::OnClick(App& app) { cur.funs.erase(nm); }
RLbFuns::RLbFuns() : RichLabel(760) {}
void RLbFuns::Sync(App& app) {
	txt = loc(L"funs_saved");
	for (auto& f : cur.funs) { txt += f.first + L","; }
}

FPnFun::FPnFun(App& app) : FPnLan(app, 800, 600, L"fpn_fun") {
	vector<Control*> tmp;
	mkp(ot_fun)();
	mkp(tb_nm_fun)(200); mkp(bt_ld_fun)();
	mkp(bt_sv_fun)(); mkp(bt_del_fun)();
	tmp = { &*tb_nm_fun, &*bt_ld_fun, &*bt_sv_fun, &*bt_del_fun };
	mkcl(clx_nm_fun); mkp(rlb_funs)();
	tmp = { &*ot_fun, &*clx_nm_fun, &*rlb_funs };
	mkcl(cly); c = &*cly; Init();
}
