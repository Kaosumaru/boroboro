#ifndef MXLIB_TIMER
#define MXLIB_TIMER
#include<windows.h>

namespace MX
{

	class TimerInit;

	class Timer {
	public:
		Timer();
		virtual ~Timer();

		void Start();
		void Stop();
		void Reset();

		unsigned int GetMili();
		unsigned int GetNano();

		friend class TimerInit;
	protected:
		__int64 _start, _end;
		static __int64 _freq;
	};

};

#endif /* Timer_H_ */
