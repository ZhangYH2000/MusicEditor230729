#include "checkbox.h"
#include "app.h"
#include "draw_geo.h"

#include "ctrl_def.h"

Checkbox::Checkbox(bool big) {
	h = big ? 30 : 20;
	gap = { 10, 10 }; dep = 1000;
	s_box = 20; s_box_in = 10;

	c_frame = { 150, 150, 150 };
	c_inner = { 0, 255, 0 };
	c_normal = { 80, 80, 80 };
	c_invalid = { 0, 0, 0 };
	c_hovered = { 20, 20, 255 };
}
int Checkbox::GetW() const { return s_box + gap.x; }
int Checkbox::GetH() const { return h + gap.y; }

dvec Checkbox::tl_box() const { return tl + dvec(0, h - s_box) / 2; }
void Checkbox::render(App &app) {
	dcol const& c =
		!enabled ? c_invalid :
		hovered ? c_hovered : c_normal;
	draw_rect_raw(scr, tl_box(), s_box, s_box, vp, c);

	if (checked) {
		int hs = (s_box - s_box_in) / 2;
		dvec tl_box_in = tl_box() + dvec(hs, hs);
		draw_rect_raw(scr, tl_box_in, s_box_in, s_box_in, vp, c_inner);
	}
}

void Checkbox::Update(App &app) {
	hovered = (hvd == this);
	bool click = enabled && hovered && msc(0);
	if (click) { checked = !checked; Upload(app); }
	else { Sync(app); } render(app);
}
void Checkbox::Discard(App& app) { rmv; hovered = false; }
void Checkbox::PreUpdate(App& app) {
	bool ok = dhv <= dep &&
		insd(msp, { tl_box(), s_box, s_box }) && insd(msp, vp);
	if (ok) { dhv = dep; hvd = this; }
}
