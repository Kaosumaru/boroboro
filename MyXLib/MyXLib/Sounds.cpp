#include "stdafx.h"
#include "Sounds.h"

using namespace MX;

Sample SoundSample::CreateSample(wchar_t *file)
{
	Sample s;
	s.LoadSample(file);
	return s;
}


void SoundBank::Initialize()
{
	meat_hit.AddSound(SoundSample::CreateSample(L"sounds\\meathit.mp3"));

	augh.AddSound(SoundSample::CreateSample(L"sounds\\ouch.wav"));
	augh.AddSound(SoundSample::CreateSample(L"sounds\\ouch2.mp3"));
	augh.AddSound(SoundSample::CreateSample(L"sounds\\ouch3.mp3"));

	sword_hit.AddSound(SoundSample::CreateSample(L"sounds\\swordHit.wav"));
	sword_hit.AddSound(SoundSample::CreateSample(L"sounds\\swordHit2.wav"));
	sword_hit.AddSound(SoundSample::CreateSample(L"sounds\\swordHit3.wav"));
	sword_hit.AddSound(SoundSample::CreateSample(L"sounds\\swordHit4.wav"));
}

SoundSample SoundBank::meat_hit;
SoundSample SoundBank::sword_hit;
SoundSample SoundBank::augh;