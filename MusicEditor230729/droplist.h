#pragma once
#include "font.h"
#include "control.h"

wstring nm_none(int id);
struct Droplist : Control {
	dvec gap;
	double dep = 0;
	int picked = 0;
	int h_item = 0;
	int w = 0, h = 0;
	int n_item = 0;
	function<wstring(int)> nm = nm_none;

	double sign_r = 0;
	int sign_margin = 0;
	int x_text_margin = 0;
	dcol c_txt, c_mark, c_edit, c_frame, c_normal, c_invalid, c_hovered;

	int item_hv = -1;
	bool edit = false;
	bool enabled = true;
	bool hovered = false;

	Droplist(int w = 0);
	int GetW() const override;
	int GetH() const override;

	dvec tl_item() const;
	void render_main(App &ap);
	void render_items(App& ap);
	void render(App& ap);
	virtual void Sync(App& ap) {}
	// 现在默认的是 Upload 的校正不改变控件本身值，只改变目标值。
	virtual void Upload(App& ap) const {}

	void Update(App& ap) override;
	void Discard(App& ap) override;
	void PreUpdate(App& ap) override;
};
struct DlLan : Droplist {
	function<wstring(int)> id_nm;

	DlLan() = default;
	DlLan(int w, function<wstring(int)> const& id_nm, int n);
	void Update(App &app) override;
};
