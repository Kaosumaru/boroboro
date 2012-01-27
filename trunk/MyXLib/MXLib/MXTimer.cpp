#include "stdafx.h"
#include "MXTimer.h"
#include <windows.h>

using namespace MX;

__int64 Timer::_freq;

namespace MX
{

	class TimerInit
	{
	public:
		TimerInit()
		{
			QueryPerformanceFrequency((LARGE_INTEGER*)&Timer::_freq);
		}
	} _TimerInit;

}

Timer::Timer() {
	_start = _end = 0;

}

Timer::~Timer() {
	// TODO Auto-generated destructor stub
}

void Timer::Start()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&_start);
}

void Timer::Stop()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&_end);
}
void Timer::Reset()
{
	_start = _end;
}


unsigned int Timer::GetMili()
{
	__int64 diff = ((_end - _start) * 1000) / _freq;
	return (unsigned int)(diff & 0xffffffff);
}

unsigned int Timer::GetNano()
{
	__int64 diff = ((_end - _start) * 10000) / (_freq);
	return (unsigned int)(diff & 0xffffffff);
}
