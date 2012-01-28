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

	hissing.AddSound(SoundSample::CreateSample(L"sounds\\sss1.wav"));
	hissing.AddSound(SoundSample::CreateSample(L"sounds\\sss2.wav"));
	hissing.AddSound(SoundSample::CreateSample(L"sounds\\sss3.wav"));

	satan_hissing.AddSound(SoundSample::CreateSample(L"sounds\\sssatan.wav"));

	apple_bite.AddSound(SoundSample::CreateSample(L"sounds\\apple-crunch.wav"));
	snake_bite.AddSound(SoundSample::CreateSample(L"sounds\\bite.wav"));

	bump.AddSound(SoundSample::CreateSample(L"sounds\\bing.wav"));

	flame.AddSound(SoundSample::CreateSample(L"sounds\\flamewind2.wav"));
}

SoundSample SoundBank::hissing;
SoundSample SoundBank::satan_hissing;
SoundSample SoundBank::apple_bite;
SoundSample SoundBank::snake_bite;
SoundSample SoundBank::put_shit;
SoundSample SoundBank::no_ass;
SoundSample SoundBank::bump;
SoundSample SoundBank::flame;