#pragma once
#include "dvec.h"

struct Cur;
struct PlayHead {
	double dep = 0;
	double t = 0;
	int w = 0, h = 0;
	bool hovered = false;
	bool dragged = false;

	PlayHead();
	dvec tl(Cur& cur) const;
	void render(Cur& cur);
	void Update(Cur& cur);
	void PreUpdate(Cur& cur);
};
