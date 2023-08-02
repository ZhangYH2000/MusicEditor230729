#include "fpn_global.h"
#include "ui.h"
#include "cur.h"
#include "bgr.h"
#include "draw_geo.h"

#define cur (*(Cur*)&app)
#include "my_def.h"

BtSave::BtSave() : BtLan(80, L"bt_save") {}
void BtSave::OnClick(App& app) { cur.Save(ui.fpn_global->tb_save->str); }
BtLoad::BtLoad() : BtLan(80, L"bt_load") {}
void BtLoad::OnClick(App& app) { cur.Load(ui.fpn_global->tb_load->str); }
void CbFollowPlay::Sync(App& app) { checked = bgr.follow_play; }
void CbFollowPlay::Upload(App& app) const { bgr.follow_play = checked; }
void CbAutoAlign::Sync(App& app) { checked = bgr.auto_align; }
void CbAutoAlign::Upload(App& app) const { bgr.auto_align = checked; }

FPnGlobal::FPnGlobal(App& app) : FPnLan(app, 400, 600, L"fpn_global") {
	vector<Control*> tmp;
	mkp(tb_save)(270); mkp(bt_save)();
	tmp = { &*tb_save, &*bt_save }; mkcl(clx_save);
	mkp(tb_load)(270); mkp(bt_load)();
	tmp = { &*tb_load, &*bt_load }; mkcl(clx_load);
	mkp(lb_follow_play)(L"cb_follow_play"); mkp(cb_follow_play)();
	tmp = { &*lb_follow_play, &*cb_follow_play }; mkcl(clx_follow_play);
	mkp(lb_auto_align)(L"cb_auto_align"); mkp(cb_auto_align)();
	tmp = { &*lb_auto_align, &*cb_auto_align }; mkcl(clx_auto_align);
	tmp = { &*clx_save, &*clx_load, &*clx_follow_play, &*clx_auto_align };
	mkcl(cly); c = &*cly; Init();
}
