#pragma once
#include "label.h"
#include "button.h"
#include "textbox.h"
#include "checkbox.h"
#include "ctrl_list.h"
#include "rich_label.h"
#include "float_panel.h"

struct BtSave : BtLan {
	BtSave();
	void OnClick(App& app) override;
};
struct BtLoad : BtLan {
	BtLoad();
	void OnClick(App& app) override;
};
struct CbFollowPlay : Checkbox {
	void Sync(App& app) override;
	void Upload(App& app) const override;
};
struct CbAutoAlign : Checkbox {
	void Sync(App& app) override;
	void Upload(App& app) const override;
};

struct FPnGlobal : FPnLan {
	ptr<Textbox> tb_save;
	ptr<BtSave> bt_save;
	ptr<CtrlListX> clx_save;
	ptr<Textbox> tb_load;
	ptr<BtLoad> bt_load;
	ptr<CtrlListX> clx_load;
	ptr<LbLan> lb_follow_play;
	ptr<CbFollowPlay> cb_follow_play;
	ptr<CtrlListX> clx_follow_play;
	ptr<LbLan> lb_auto_align;
	// 以后应该把这个 Label 整合到 Checkbox 里面去。
	ptr<CbAutoAlign> cb_auto_align;
	ptr<CtrlListX> clx_auto_align;
	ptr<CtrlListY> cly;
	FPnGlobal(App& app);
};
