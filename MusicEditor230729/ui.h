#pragma once
#include "utils.h"

struct Cur;
struct FloatPanel;
struct FPnGlobal;
struct FPnCmd;
struct FPnPar;
struct FPnFun;
struct FPnLang;
struct FPnAbout;
struct FPnTrack;
struct FPnInst;
struct PnMenu;
struct PnBottom;
struct UI {
	vector<FloatPanel*> fpns;

	ptr<FPnGlobal> fpn_global;
	ptr<FPnInst> fpn_inst;
	ptr<FPnTrack> fpn_track;
	ptr<FPnCmd> fpn_cmd;
	ptr<FPnPar> fpn_par;
	ptr<FPnFun> fpn_fun;
	ptr<FPnLang> fpn_lang;
	ptr<FPnAbout> fpn_about;
	ptr<PnMenu> pn_menu;
	ptr<PnBottom> pn_bottom;

	UI(Cur& cur) {};
	void sort_fpns();

	// 这里用 init 的原因是控件会用 cur->ui 来访问 ui, 但这个时候 ui 尚未初始化。
	// 这也是为什么主程序不需要 Init。这应该是一个普遍的问题。
	void Init(Cur& cur);
	void Update(Cur& cur);
	void PreUpdate(Cur& cur);
};
