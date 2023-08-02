#include "pn_menu.h"
#include "ui.h"
#include "cur.h"
#include "bgr.h"
#include "track.h"
#include "fpn_global.h"
#include "fpn_cmd.h"
#include "fpn_par.h"
#include "fpn_fun.h"
#include "fpn_lang.h"
#include "fpn_about.h"
#include "fpn_track.h"
#include "fpn_inst.h"

#define cur (*(Cur*)&app)
#include "my_def.h"
#define mkbt(nm) mkp(bt_##nm)(&*ui.fpn_##nm);

BtPlay::BtPlay() : Button(150) {}
void BtPlay::OnClick(App& app) { cur.play = !cur.play; }
void BtPlay::Update(App& app) {
	txt = cur.play ? loc(L"pause") : loc(L"play");
	Button::Update(app);
}
BtStop::BtStop() : BtLan(150, L"bt_stop") {}
void BtStop::OnClick(App& app) { 
	cur.play = false; cur.t_play = 0; 
	for (auto tk : cur.tks) { tk->reset_np(cur); }
}
BtRectSel::BtRectSel() : BtLan(150, L"bt_rect_sel") {}
void BtRectSel::OnClick(App& app) { bgr.begin_rect_sel = true; }
void BtRectSel::Update(App& app) { enabled = !bgr.begin_rect_sel; BtLan::Update(app); }
BtShowFreq::BtShowFreq() : Button(150) {}
void BtShowFreq::OnClick(App& app) { cur.show_freq = !cur.show_freq; }
void BtShowFreq::Update(App& app) {
	txt = cur.show_freq ? loc(L"hide_freq") : loc(L"show_freq");
	Button::Update(app);
}

PnMenu::PnMenu(App& app) : Panel(Panel::menu(app)) {
	vector<Control*> tmp;
	mkp(bt_play)(); mkp(bt_stop)(); mkp(bt_rect_sel)(); mkp(bt_show_freq)();
	mkbt(global); mkbt(track); mkbt(inst);
	mkbt(cmd); mkbt(par); 
	mkbt(fun); mkbt(lang); mkbt(about);
	tmp = { &*bt_play, &*bt_stop, &*bt_rect_sel, &*bt_show_freq,
		&*bt_global, &*bt_track, &*bt_inst,
		&*bt_cmd, &*bt_par, &*bt_fun, &*bt_lang, &*bt_about };
	mkcl(cly); c = &*cly; Init();
}
