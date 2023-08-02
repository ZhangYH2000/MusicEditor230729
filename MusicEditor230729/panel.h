#pragma once
#include "tile.h"

struct App;
struct Control;
struct ScrollbarY;
struct Panel {
	dvec tl;
	drect vp;
	double dep = 0;
	Control* c = NULL;
	int w = 0, h = 0;
	double wheel_mtp = 0;
	dvec tl_show_rel;
	drect vp_show_raw_rel;
	dcol c_frame, c_panel;

	// ctrl_comp 里面用 ptr，这里为了统一也用 ptr。
	bool use_sb = false;
	ptr<ScrollbarY> sb;

	int h_show = 0;
	bool full = false;
	bool hovered = false;
	bool wheeled = false;

	Panel();
	static Panel menu(App& app);
	static Panel bottom(App& app);

	int min_y() const;
	drect vp_show() const;
	// 不用 delegate 主要是希望能重置滚动。
	void set_c(App &app, Control* _c);
	void init_c();
	void init_sb();
	void render(App& app);

	void Init();
	virtual void Update(App& app);
	// 如果有可删除的 Panel 就用这个。
	virtual void Discard(App& app);
	virtual void PreUpdate(App& app);
};
