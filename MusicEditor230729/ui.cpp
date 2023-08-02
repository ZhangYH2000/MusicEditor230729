#include "ui.h"
#include "cur.h"
#include "float_panel.h"
#include "fpn_global.h"
#include "fpn_par.h"
#include "fpn_fun.h"
#include "fpn_cmd.h"
#include "fpn_lang.h"
#include "fpn_about.h"
#include "fpn_track.h"
#include "fpn_inst.h"
#include "pn_menu.h"
#include "pn_bottom.h"

#include "my_def.h"
#define mkfp(x) mkp(x)(cur); fpns.push_back(&*x);

void UI::sort_fpns() { sort(fpns.begin(), fpns.end(), less_fpn); }
void UI::Init(Cur &cur) {
	mkfp(fpn_global);
	mkfp(fpn_cmd);
	mkfp(fpn_par);
	mkfp(fpn_fun);
	mkfp(fpn_lang);
	mkfp(fpn_about);
	mkfp(fpn_track);
	mkfp(fpn_inst);

	// pn_menu 放在后面是因为要等 float_panel 先加载完。
	// 这主要是 BtFPn 的原因。
	// 可以用模板来规避这个，但是我觉得太麻烦了。
	mkp(pn_menu)(cur);
	mkp(pn_bottom)(cur);
}
void UI::Update(Cur &cur) {
	pn_menu->Update(cur);
	pn_bottom->Update(cur);
	for (auto fp : fpns) { fp->Update(cur); }
}
void UI::PreUpdate(Cur &cur) {
	pn_menu->PreUpdate(cur);
	pn_bottom->PreUpdate(cur);
	sort_fpns();
	for (auto fp : fpns) { fp->PreUpdate(cur); }
}
