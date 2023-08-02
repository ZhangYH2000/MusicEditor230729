#include "fx.h"
#include "cur.h"
#include "var.h"
#include "reverb.h"

FX::FX(Cur& cur, Var const& v) {
	if (v.vec.size() >= 2) {
		type = v.vec[0]->num;
		switch (type) {
		case FX_REVERB: mkp(reverb)(cur, *v.vec[1]); return;
		}
	}
}
void FX::reset(Cur& cur) {
	if (reverb) { reverb->reset(cur); }
}
double FX::loop(Cur& cur, double in) {
	switch (type) {
	case FX_REVERB: return reverb->loop(cur, in);
	} return 0;
}
