#ifndef MXLIB_ANIMUTILS
#define MXLIB_ANIMUTILS

#include <windows.h>
#include <memory>
#include "MXAnimation.h"

namespace MX
{
	using namespace std;


	shared_ptr<MX::Animation> CreateAnimationFromFile(MX::Draw &draw, wchar_t* file);
	shared_ptr<MX::Animation> CreateAnimationFromFile(MX::Draw &draw, wchar_t* file, int number, DWORD speed, float cx, float cy);

};

#endif