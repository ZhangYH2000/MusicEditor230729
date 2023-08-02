#include "drum.h"
#include "fx.h"
#include "cur.h"
#include "inst.h"
#include "track.h"

#define ntn (cur.n_tone())
#define tnb (cur.tone_b())


DrumMachine::DrumMachine(Cur& cur) {
	rep(i, 0, ntn) { 
		ds.push_back(msh<Track>()); 
		ds.back()->drum_member = true; 
	} vol = 1;
}
void DrumMachine::save(FILE* f) const {
	for (auto d : ds) { d->save(f); }
}
DrumMachine::DrumMachine(Cur& cur, FILE* f) {
	rep(i, 0, ntn) { 
		ds.push_back(msh<Track>(cur, f)); 
		ds.back()->drum_member = true; 
	} vol = 1;
}
ptr<Track>& DrumMachine::get(Cur& cur, int tn) { return ds[tnb - 1 - tn]; }

void DrumMachine::reset_p(Cur& cur) {
	for (auto d : ds) { d->reset_p(cur); d->pinsts.clear(); }
}
void DrumMachine::reset_np(Cur& cur) {
	for (auto d : ds) { d->reset_np(cur); }
}
double DrumMachine::Play(Cur& cur) {
	double out = 0;
	for (auto d : ds) { out += d->Play(cur); } return vol.val(cur) * out;
}
double DrumMachine::PlayPiano(Cur& cur) {
	double out = 0;
	for (auto d : ds) { out += d->PlayPiano(cur); } return vol.val(cur) * out;
}
