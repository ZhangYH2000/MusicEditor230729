#include "dvec.h"

dvec dvec::from_str(wstring const& s) {
	dvec v; int i = 0;
	v.x = fetch_num(i, s);
	v.y = fetch_num(i, s); return v;
}
wstring tw(dvec v) {
	return L"(" + tw(v.x) + L"," + tw(v.y) + L")";
}

bool insd(dvec v, drect r) {
	return
		insd(v.x, r.left(), r.right()) &&
		insd(v.y, r.top(), r.bottom());
}
drect overlap(drect a, drect b) {
	int left = max(a.left(), b.left());
	int right = min(a.right(), b.right());
	int top = max(a.top(), b.top());
	int bottom = min(a.bottom(), b.bottom());
	return drect({ left, top }, right - left, bottom - top);
}
