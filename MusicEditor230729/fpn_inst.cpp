#include "fpn_inst.h"
#include "ui.h"
#include "cur.h"
#include "drum.h"
#include "track.h"

#define cur (*(Cur*)&app)
#include "my_def.h"
#define tksel (*cur.tksel)
#define tna (cur.tone_a())
#define tnb (cur.tone_b())
#define pdm (cur.dm->get(cur, id))

BtSvDrum::BtSvDrum() : BtLan(120, L"bt_sv_drum") {}
void BtSvDrum::OnClick(App& app) {
	int id = wtof(ui.fpn_inst->tb_drum->str);
	if (insd(id, tna, tnb) && !tksel.drum) {
		pdm = msh<Track>();
		pdm->str = tksel.str;
		pdm->Build(cur); 
		pdm->drum_member = true;
		// 脚本语言是不可预测的，所以这里不做判断。
	}
}
BtLdDrum::BtLdDrum() : BtLan(120, L"bt_ld_drum") {}
void BtLdDrum::OnClick(App& app) {
	int id = wtof(ui.fpn_inst->tb_drum->str);
	if (insd(id, tna, tnb) && !tksel.drum) {
		tksel.str = tksel.tmp_str = pdm->str;
		tksel.Build(cur);
	}
}

RTbInst::RTbInst() : RichTextbox(360, 460) {}
void RTbInst::Sync(App &app) { str = tksel.tmp_str; }
void RTbInst::OnDone(App &app) const { tksel.tmp_str = str; }

BtInst::BtInst() : BtLan(70, L"bt_inst") {}
void BtInst::OnClick(App& app) { tksel.str = tksel.tmp_str; tksel.Build(cur); }

FPnInst::FPnInst(App& app) : FPnLan(app, 400, 600, L"fpn_inst") {
	vector<Control*> tmp;
	mkp(tb_drum)(100); mkp(bt_sv_drum)(); mkp(bt_ld_drum)();
	tmp = { &*tb_drum, &*bt_sv_drum, &*bt_ld_drum }; mkcl(clx_drum);
	mkp(rtb_inst)(); mkp(bt_inst)();
	tmp = { &*clx_drum, &*rtb_inst, &*bt_inst };
	mkcl(cly); c = &*cly; Init();
}
