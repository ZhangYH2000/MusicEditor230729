#pragma once
#include "label.h"
#include "button.h"
#include "textbox.h"
#include "checkbox.h"
#include "separator.h"
#include "ctrl_list.h"
#include "float_panel.h"

struct TbTkNm : Textbox {
	int id = 0;
	TbTkNm(int id);
	void Sync(App& app) override;
	void OnDone(App& app) const override;
};
struct BtDelTk : BtLan {
	int id = 0;
	BtDelTk(int id);
	void OnClick(App& app) override;
	void Update(App& app) override;
};
struct BtTkUp : BtLan {
	int id = 0;
	BtTkUp(int id);
	void Update(App& app) override;
	void OnClick(App& app) override;
};
struct BtTkDown : BtLan {
	int id = 0;
	BtTkDown(int id);
	void Update(App& app) override;
	void OnClick(App& app) override;
};
struct CbTkShow : Checkbox {
	int id = 0;
	CbTkShow(int id);
	void Sync(App& app) override;
	void Upload(App& app) const override;
};
struct CbTkSel : Checkbox {
	int id = 0;
	CbTkSel(int id);
	void Sync(App& app) override;
	void Upload(App& app) const override;
	void Update(App& app) override;
};

struct ClYTrack : CtrlListY {
	ptr<TbTkNm> tb_nm;
	ptr<BtDelTk> bt_del;
	ptr<CtrlListX> clx_nm;
	ptr<BtTkUp> bt_up;
	ptr<BtTkDown> bt_down;
	ptr<LbLan> lb_show;
	ptr<CbTkShow> cb_show;
	ptr<LbLan> lb_sel;
	ptr<CbTkSel> cb_sel;
	ptr<CtrlListX> clx_sel;
	ptr<SeparatorY> spy;
	ClYTrack(int id);
};
struct ClYTracks : CtrlListY {
	vector<ptr<ClYTrack>> mbs;
	void Update(App &app) override;
};

struct BtAddTk : BtLan {
	BtAddTk();
	void OnClick(App &app) override;
};

struct FPnTrack : FPnLan {
	ptr<ClYTracks> cly_tracks;
	ptr<BtAddTk> bt_add_tk;
	ptr<CtrlListY> cly;
	FPnTrack(App& app);
};

