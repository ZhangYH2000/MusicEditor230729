#include "playhead.h"
#include "cur.h"
#include "bgr.h"
#include "draw_geo.h"
#include "draw_comp.h"
#include "track.h"

#include "my_def.h"
#define sdx(x) (bgr.sdx(cur, x))
#define sdy(x) (bgr.sdy(cur, x))
#define sdbx(x) (bgr.sdbx(cur, x))
#define sdby(x) (bgr.sdby(cur, x))
#define t_play (cur.t_play)
#define vpsht (bgr.vp_sht())

PlayHead::PlayHead() {
	w = 15; h = 30; dep = 998;
}
dvec PlayHead::tl(Cur& cur) const {
	return { sdx(t) - w / 2, (bgr.h - h) / 2 };
}
void PlayHead::render(Cur& cur) {
	dcol c_normal = { 60, 150, 60 };
	dcol c_frame = { 150, 150, 150 };
	dcol c_hovered = { 20, 20, 255 };
	dcol c_dragged = { 255, 20, 20 };
	dcol const& c =
		dragged ? c_dragged :
		hovered ? c_hovered : c_normal;

	draw_rect(scr, dscr, 998, { sdx(t), 0 }, 2, bgr.h, vpsht, { 60, 150, 60 });
	if (cur.replay) {
		draw_rect(scr, dscr, 998, { sdx(cur.t_stop), 0 }, 2, bgr.h, vpsht, { 150, 60, 60 });
	}
	draw_px_rect_framed(scr, dscr, dep, tl(cur), w, h, vpsht, c, c_frame);
}
void PlayHead::Update(Cur& cur) {
	hovered = (this == hvd);
	if (dragged) {
		int a = vpsht.left() + 200;
		int b = vpsht.right() - 200;
		bgr.tl_sht_rel.x += max(0, a - msp.x) * dt * 8;
		bgr.tl_sht_rel.x += min(0, b - msp.x) * dt * 8;
		t += sdbx(msp.x) - t;
		if (!msd[0]) {
			dragged = false; t_play = t;
			for (auto tk : cur.tks) { tk->reset_np(cur); }
		}
	}
	else { 
		// follow_play 现在作为 bgr 成员不合适了。
		if (bgr.follow_play && cur.play) {
			bgr.tl_sht_rel.x -= sdx(t) - (bgr.tl.x + bgr.w / 2);
		}
		t = t_play; dragged = hovered && msc(0); 
	}



	render(cur);
}
void PlayHead::PreUpdate(Cur& cur) {
	bool ok = dhv <= dep &&
		insd(msp, { tl(cur), w, h}) && insd(msp, vpsht);
	if (ok) { dhv = dep; hvd = this; }
}
