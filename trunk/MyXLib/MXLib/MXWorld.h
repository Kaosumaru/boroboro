#ifndef MXLIB_WORLD
#define MXLIB_WORLD

#include "MXTimer.h"
#include <boost/signal.hpp>

namespace MX
{

	class World
	{
	public:
		static void Initialize();
		static void ResetTime();
		static DWORD GetCurrentTime();
		static DWORD GetElapsedTime();
		static void ElapseTime();
		static DWORD GetTick();
		static float GetElapsedFloat();

		static void KeyDown(unsigned short nKey);
		static void KeyUp(unsigned short nKey);
		static void MouseKeyUp(char nKey);
		static void MouseKeyDown(char nKey);
		static void MouseMove(int x, int y);

		static bool Key[256*256];
		static int MouseX;
		static int MouseY;
		static bool MouseKey[3];

		static boost::signal<void (short)> OnSomeKeyDown;
		static boost::signal<void (short)> OnSomeKeyUp;
		static boost::signal<void (char)> OnSomeMouseKeyDown;
		static boost::signal<void (char)> OnSomeMouseKeyUp;


		static boost::signal<void ()> OnKeyDown[256];
		static boost::signal<void ()> OnKeyUp[256];
		static boost::signal<void ()> OnMouseKeyDown[3];
		static boost::signal<void ()> OnMouseKeyUp[3];




	protected:
		static float sec;
		static DWORD tick;
		static DWORD time;
		static DWORD elapsed;
		static Timer timer;

	};

};

#endif