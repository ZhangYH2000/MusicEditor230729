#include "fpn_about.h"
#include "ui.h"
#include "cur.h"
#include "draw_geo.h"

#define cur (*(Cur*)&app)
#include "my_def.h"

FPnAbout::FPnAbout(App& app) : FPnLan(app, 800, 600, L"fpn_about") {
	vector<Control*> tmp;
	mkp(rtb_about)(760, L"about_txt");
	tmp = { &*rtb_about };
	mkcl(cly); c = &*cly; Init();
}
