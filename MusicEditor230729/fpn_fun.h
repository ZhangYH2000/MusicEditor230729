#pragma once
#include "button.h"
#include "textbox.h"
#include "ctrl_list.h"
#include "rich_label.h"
#include "float_panel.h"

struct OtFun : Control {
	double dep = 0;
	int w = 0, h = 0;
	dvec gap, ingap;
	dcol c_edit, c_frame, c_normal, c_hovered, c_fun;

	int n_filled = 0;
	vector<double> ys;

	bool edit = false;
	bool hovered = false;

	OtFun();
	int GetW() const override;
	int GetH() const override;

	void render(App &app);
	void Update(App& app) override;
	void Discard(App& app) override;
	void PreUpdate(App& app) override;
};

struct BtLdFun : BtLan {
	BtLdFun();
	void OnClick(App& app) override;
};
struct BtSvFun : BtLan {
	BtSvFun();
	void OnClick(App& app) override;
};
struct BtDelFun : BtLan {
	BtDelFun();
	void OnClick(App& app) override;
};
struct RLbFuns : RichLabel {
	RLbFuns();
	void Sync(App& app) override;
};

struct FPnFun : FPnLan {
	ptr<OtFun> ot_fun;
	ptr<Textbox> tb_nm_fun;
	ptr<BtLdFun> bt_ld_fun;
	ptr<BtSvFun> bt_sv_fun;
	ptr<BtDelFun> bt_del_fun;
	ptr<CtrlListX> clx_nm_fun;
	ptr<RLbFuns> rlb_funs;
	ptr<CtrlListY> cly;
	FPnFun(App& app);
};
