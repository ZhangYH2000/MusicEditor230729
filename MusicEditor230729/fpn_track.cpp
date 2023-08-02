#include "fpn_track.h"
#include "ui.h"
#include "fpn_inst.h"
#include "cur.h"
#include "track.h"

#define cur (*(Cur*)&app)
#include "my_def.h"
#define tks (cur.tks)
#define tk (*tks[id])
#define nrsel (cur.nrsel)
#define ptksel (cur.tksel)

TbTkNm::TbTkNm(int id) : id(id), Textbox(270) {}
void TbTkNm::Sync(App &app) { str = tk.nm; }
void TbTkNm::OnDone(App& app) const { tk.nm = str; }
BtDelTk::BtDelTk(int id) : id(id), BtLan(80, L"bt_del_tk") {}
void BtDelTk::OnClick(App& app) { tk.del = true; }
void BtDelTk::Update(App& app) { enabled = ptksel != &tk; BtLan::Update(app); }
BtTkUp::BtTkUp(int id) : id(id), BtLan(80, L"bt_tk_up") {}
void BtTkUp::Update(App& app) { enabled = id != 0; BtLan::Update(app); }
void BtTkUp::OnClick(App& app) { swap(tks[id], tks[id - 1]); }
BtTkDown::BtTkDown(int id) : id(id), BtLan(80, L"bt_tk_down") {}
void BtTkDown::Update(App& app) { enabled = id != tks.size() - 1; BtLan::Update(app); }
void BtTkDown::OnClick(App& app) { swap(tks[id], tks[id + 1]); }
CbTkShow::CbTkShow(int id) : id(id), Checkbox(true) {}
void CbTkShow::Sync(App& app) { checked = tk.show; }
void CbTkShow::Upload(App& app) const { 
	tk.show = checked; if (!tk.show) { tk.reset_np(cur); }
}
CbTkSel::CbTkSel(int id) : id(id), Checkbox(true) {}
void CbTkSel::Sync(App& app) { checked = ptksel == &tk; }
void CbTkSel::Upload(App& app) const {
	if (ptksel != &tk) { 
		ptksel->Discard(cur); ptksel->reset_p(cur);
		nrsel.clear();  ptksel = &tk; 
		ui.fpn_inst->rtb_inst->Discard(app);
	}
}
void CbTkSel::Update(App& app) {
	enabled = ptksel != &tk; Checkbox::Update(app);
}

ClYTrack::ClYTrack(int id) {
	vector<Control*> tmp;
	mkp(tb_nm)(id); mkp(bt_del)(id);
	tmp = { &*tb_nm, &*bt_del }; mkcl(clx_nm);
	mkp(bt_up)(id); mkp(bt_down)(id); 
	mkp(lb_show)(L"cb_tk_show"); mkp(cb_show)(id);
	mkp(lb_sel)(L"cb_tk_sel"); mkp(cb_sel)(id);
	tmp = { &*bt_up, &*bt_down, &*lb_show, &*cb_show, &*lb_sel, &*cb_sel }; 
	mkcl(clx_sel); mkp(spy)(400); 
	cs = { &*clx_nm, &*clx_sel, &*spy };
}
void ClYTracks::Update(App &app) { cs.clear();
	rep(i, mbs.size(), tks.size()) { mbs.push_back(msh<ClYTrack>(i)); }
	rep(i, 0, tks.size()) { cs.push_back(&*mbs[i]); }
	rep(i, tks.size(), mbs.size()) { mbs[i]->Discard(app); }
	CtrlListY::Update(app);
}

BtAddTk::BtAddTk() : BtLan(200, L"bt_add_tk") {}
void BtAddTk::OnClick(App &app) { tks.push_back(msh<Track>(loc(L"unnamed_track"))); }

FPnTrack::FPnTrack(App& app) : FPnLan(app, 400, 600, L"fpn_track") {
	vector<Control*> tmp;
	mkp(cly_tracks)();
	mkp(bt_add_tk)();
	tmp = { &*cly_tracks, &*bt_add_tk };
	mkcl(cly); c = &*cly; Init();
}
