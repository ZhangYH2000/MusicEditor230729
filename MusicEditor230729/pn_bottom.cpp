#include "pn_bottom.h"
#include "cur.h"

#define cur (*(Cur*)&app)
#include "my_def.h"

LbFPS::LbFPS() { w = 450; fixed_w = true; }
void LbFPS::Sync(App &app) {
	txt =
		loc(L"frm_tm") + tw2(cur.fps.frm_time * 1000) +
		L", FPS: " + tw2(cur.fps.fps);
}

PnBottom::PnBottom(App& app) : Panel(Panel::bottom(app)) {
	vector<Control*> tmp;
	mkp(lb_fps)();
	tmp = { &*lb_fps };
	mkcl(clx); c = &*clx; Init();
}
