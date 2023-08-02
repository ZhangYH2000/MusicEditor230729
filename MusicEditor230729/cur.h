#pragma once
#include "app.h"
#include "var.h"
#include "complex.h"

bool keytone(BYTE key, int& tone);

struct UI;
struct Bgr;
struct Inst;
struct param;
struct Track;
struct NRect;
struct DrumMachine;
struct PlayHead;
struct Cur : App {
	ptr<UI> ui;
	ptr<Bgr> bgr;

	Scope sc;
	wstring dbstr, cmd, tmp_cmd;
	vector<ptr<param>> pars;
	map<wstring, ptr<vector<double>>> funs;

	int n_oct = 0, n_beat = 0, base_oct = 0;
	double vol = 0, tempo = 0, transpose = 0;
	double fft_scl = 0;

	ptr<PlayHead> ph;
	ptr<DrumMachine> dm;
	double t_play = 0, t_stop = 0;
	bool play = false, replay = false;
	bool show_freq = false;
	deque<complex> cs;
	Track* tkdm = NULL;
	Track* tksel = NULL;
	vector<NRect *> nrsel;
	vector<ptr<Track>> tks;
	array<Inst*, 255> piano{};

	Cur();
	void Play();
	void Save(wstring const& nm) const;
	void Load(wstring const& nm);
	void Reset();
	void Update() override;

	int n_tone() const { return 12 * n_oct; }
	int tone_a() const { return -12 * base_oct; }
	int tone_b() const { return tone_a() + n_tone(); }

	void zero_mean(wstring const& nm);
	void get_phase(int id);

	void freq_analysis();
	void save_par_fun(FILE *f) const;
	void load_par_fun(FILE *f);
	void init_def_fun();
	void basic_update();
};
