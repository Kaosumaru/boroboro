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

	put_shit.AddSound(SoundSample::CreateSample(L"sounds\\shit.wav"));
	no_ass.AddSound(SoundSample::CreateSample(L"sounds\\niemamdupy.wav"));

	bump.AddSound(SoundSample::CreateSample(L"sounds\\bing.wav"));

	flame.AddSound(SoundSample::CreateSample(L"sounds\\flamewind2.wav"));

	gulp.AddSound(SoundSample::CreateSample(L"sounds\\gulp.wav"));
	beep.AddSound(SoundSample::CreateSample(L"sounds\\beep.wav"));


//	AmbientSound = SoundSample::CreateSample(L"sounds\\AmbientSound.wav");
	AmbientSound.LoadStream(L"sounds\\AmbientSound.wav");
}

SoundSample SoundBank::hissing;
SoundSample SoundBank::satan_hissing;
SoundSample SoundBank::apple_bite;
SoundSample SoundBank::snake_bite;
SoundSample SoundBank::put_shit;
SoundSample SoundBank::no_ass;
SoundSample SoundBank::bump;
SoundSample SoundBank::flame;
SoundSample SoundBank::gulp;
SoundSample SoundBank::beep;


Stream SoundBank::AmbientSound;