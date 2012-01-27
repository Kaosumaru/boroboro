#ifndef MXLIB_SOUND
#define MXLIB_SOUND

#include <windows.h>
#include "bass.h"
#include <memory>
#include "MXUtils.h"
#include "MXWindow.h"
namespace MX
{

	class Channel;

	class Sample
	{
	public:
		Sample()  { handle = 0; }
		bool IsValid() { return handle != 0; }
		void Free() { BASS_SampleFree(handle); handle = 0; }

		bool LoadSample(const wchar_t *file, int max = 3);
		Channel GetChannel(bool override = true);
	
		void Play(bool override = true);

	protected:
		DWORD handle;
	};

	class Channel
	{
	public:
		Channel() { handle = 0; }
		bool IsValid() { return handle != 0; }

		void Play(bool restart = false);
		void Pause();
		void Stop();
		bool IsActive();
		void Loop(bool loop = true);

		void SetVolume(float volume);
		float GetVolume();

		void SetBalance(float balance);
		float GetBalance();

		friend MX::Sample;
	protected:
		DWORD handle;
	};




	class Music;

	class Sound
	{
	public:
		static bool Initialize(Window *w = 0);
		static void Deinitialize();
		static bool IsInitialized() { return init; }

		static void Play();
		static void Pause();
		static void Stop();

		static void SetVolume(float volume);
		static float GetVolume();

	protected:

		static bool init;
	};

	class Music : public Channel
	{
	public:
		Music(){};

		bool LoadMusic(const wchar_t *file);
		void Free() { BASS_MusicFree(handle); handle = 0; }

		friend MX::Sound;
	};

	class SharedMusic : public Music, public enable_shared_from_this<SharedMusic> 
	{
		SharedMusic() {}
		~SharedMusic(){ Free(); };
	};

	class Stream : public Channel
	{
	public:
		Stream(){};

		bool LoadStream(const wchar_t *file);
		void Free() { BASS_StreamFree(handle); handle = 0; }

		friend MX::Sound;
	};

	class SharedStream : public Stream, public enable_shared_from_this<SharedStream> 
	{
		SharedStream() {}
		~SharedStream(){ Free(); };
	};


};

#endif