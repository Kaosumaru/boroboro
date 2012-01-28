#include "stdafx.h"
#include "MXAnimUtils.h"

using namespace MX;
using namespace std;

namespace MX
{


shared_ptr<MX::Animation> CreateAnimationFromFile(MX::Draw &draw, wchar_t* file)
{
	shared_ptr<MX::Image> cross(new MX::Image());
	bool b = cross->Load(draw, file);

	shared_ptr<MX::Animation> anim(new MX::Animation(500));
	shared_ptr<MX::Frame> frame(new MX::Frame(cross));
	anim->AddFrame(MX::AnimationFrame(frame, 3000, cross->Width()/2.0f, cross->Height()/2.0f));
	return anim;
}


shared_ptr<MX::Animation> CreateAnimationFromFile(MX::Draw &draw, wchar_t* file, int number, DWORD speed, float cx, float cy)
{
	wchar_t tfile[1024];
	shared_ptr<MX::Animation> anim(new MX::Animation(speed));

	for (int i = 0; i < number; i++)
	{
		shared_ptr<MX::Image> cross(new MX::Image());
		StringCbPrintf(tfile, 1024, L"%s%d.png", file, i+1);
		bool b = cross->Load(draw, tfile);
		shared_ptr<MX::Frame> frame(new MX::Frame(cross));
		anim->AddFrame(MX::AnimationFrame(frame, speed, cx, cy));
	}

	return anim;
}

};