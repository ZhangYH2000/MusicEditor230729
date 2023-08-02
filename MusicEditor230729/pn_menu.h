#pragma once
#include "panel.h"
#include "button.h"
#include "ctrl_list.h"

struct BtPlay : Button {
	BtPlay();
	void OnClick(App& app) override;
	void Update(App& app) override;
};
struct BtStop : BtLan {
	BtStop();
	void OnClick(App& app) override;
};
struct BtRectSel : BtLan {
	BtRectSel();
	void OnClick(App &app) override;
	void Update(App& app) override;
};
struct BtShowFreq : Button {
	BtShowFreq();
	void OnClick(App& app) override;
	void Update(App& app) override;
};

struct PnMenu : Panel {
	ptr<BtPlay> bt_play;
	ptr<BtStop> bt_stop;
	ptr<BtRectSel> bt_rect_sel;
	ptr<BtShowFreq> bt_show_freq;
	ptr<BtFPn> bt_global;
	ptr<BtFPn> bt_track;
	ptr<BtFPn> bt_inst;
	ptr<BtFPn> bt_cmd;
	ptr<BtFPn> bt_par;
	ptr<BtFPn> bt_fun;
	ptr<BtFPn> bt_lang;
	ptr<BtFPn> bt_about;
	ptr<CtrlListY> cly;

	PnMenu(App& app);
};
