#pragma once
#include "button.h"
#include "textbox.h"
#include "ctrl_list.h"
#include "float_panel.h"
#include "rich_textbox.h"

struct BtSvDrum : BtLan {
	BtSvDrum();
	void OnClick(App& app) override;
};
struct BtLdDrum : BtLan {
	BtLdDrum();
	void OnClick(App& app) override;
};

struct RTbInst : RichTextbox {
	RTbInst();
	void Sync(App& app) override;
	void OnDone(App& app) const override;
};
struct BtInst : BtLan {
	BtInst();
	void OnClick(App& app) override;
};


struct FPnInst : FPnLan {
	ptr<Textbox> tb_drum;
	ptr<BtSvDrum> bt_sv_drum;
	ptr<BtLdDrum> bt_ld_drum;
	ptr<CtrlListX> clx_drum;
	ptr<RTbInst> rtb_inst;
	ptr<BtInst> bt_inst;
	ptr<CtrlListY> cly;
	FPnInst(App& app);
};

