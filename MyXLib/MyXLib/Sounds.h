#ifndef ORZEL1_SOUNDS
#define ORZEL1_SOUNDS

#include <memory>
#include "../MXLib/MXSound.h"

namespace MX
{

class SoundSample
{
public:
	SoundSample(){};

	void AddSound(const Sample& sound) { sounds.push_back(sound); }

	void Play() { sounds[rand()%sounds.size()].Play(); }

	static Sample CreateSample(wchar_t *file);

protected:
	vector<Sample> sounds;
};

class SoundBank
{
public:
	static void Initialize();

	static SoundSample hissing;
	static SoundSample satan_hissing;
	static SoundSample apple_bite;
	static SoundSample snake_bite;
	static SoundSample put_shit;
	static SoundSample no_ass;
	static SoundSample bump;
	static SoundSample flame;
};

};


#endif