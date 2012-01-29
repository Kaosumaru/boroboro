#include "stdafx.h"
#include "MXSound.h"

using namespace MX;


void Channel::Play(bool restart)
{
	BASS_ChannelPlay(handle, restart);
}
void Channel::Pause()
{
	BASS_ChannelPause(handle);
}

void Channel::Stop()
{
	BASS_ChannelStop(handle);
}
bool Channel::IsActive()
{
	return BASS_ChannelIsActive(handle) == BASS_ACTIVE_PLAYING;
}


void Channel::Loop(bool loop)
{
	BASS_ChannelFlags(handle, loop ? BASS_SAMPLE_LOOP : 0, BASS_SAMPLE_LOOP);
}



void Channel::SetVolume(float volume)
{
	BASS_ChannelSetAttribute(handle, BASS_ATTRIB_VOL, volume);
}
float Channel::GetVolume()
{
	float f = 0.0f;
	BASS_ChannelGetAttribute(handle, BASS_ATTRIB_VOL, &f);
	return f;
}

void Channel::SetBalance(float balance)
{
	BASS_ChannelSetAttribute(handle, BASS_ATTRIB_PAN, balance);
}

float Channel::GetBalance()
{
	float f = 0.0f;
	BASS_ChannelGetAttribute(handle, BASS_ATTRIB_PAN, &f);
	return f;
}


bool Sample::LoadSample(const wchar_t *file, int max)
{
	handle = BASS_SampleLoad(false, (void*)file, 0, 0, max, BASS_UNICODE | BASS_SAMPLE_OVER_POS);
	return IsValid();
}

Channel Sample::GetChannel(bool override)
{
	Channel c;
	c.handle = BASS_SampleGetChannel(handle, !override);
	return c;
}

void Sample::Play(bool override)
{

	Channel c;
	c.handle = BASS_SampleGetChannel(handle, !override);
	if (c.IsValid())
		c.Play();
}

void Sample::Stop()
{
	Channel c;
	c.handle = BASS_SampleGetChannel(handle, true);
	if (c.IsValid())
		c.Stop();
}

bool Sound::init = false;


bool Sound::Initialize(Window *w)
{
	if (init)
		return false;
	init = true;
	if (!BASS_Init(-1,44100,0,w ? w->GetHWND() : NULL,NULL))
		return false;
	return true;
}
void Sound::Deinitialize()
{
	if (!init)
		return;
	init = false;
	BASS_Free();

}


void Sound::Play()
{
	BASS_Start();
}

void Sound::Pause()
{
	BASS_Pause();
}

void Sound::Stop()
{
	BASS_Stop();
}

void Sound::SetVolume(float volume)
{
	BASS_SetVolume(volume);
}

float Sound::GetVolume()
{
	return BASS_GetVolume();
}

bool Music::LoadMusic(const wchar_t *file)
{
	handle = BASS_MusicLoad(false, (void*)file, 0, 0, BASS_UNICODE | BASS_MUSIC_RAMPS, 0);
	return IsValid();
}

bool Stream::LoadStream(const wchar_t *file)
{
	handle = BASS_StreamCreateFile(false, (void*)file, 0, 0, BASS_UNICODE);
	return IsValid();
}



