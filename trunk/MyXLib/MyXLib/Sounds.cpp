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

}

SoundSample SoundBank::hissing;
SoundSample SoundBank::satan_hissing;