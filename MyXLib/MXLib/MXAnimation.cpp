#include "stdafx.h"
#include "MXAnimation.h"
#include "MXUtils.h"
#include "MXWorld.h"

using namespace MX;


void Animation::Animate(Spriter &spriter, float x, float y, float z, float rotation, float scaleX, float scaleY, D3DCOLOR color, float speed)
{
	int frame = (World::GetCurrentTime() / (DWORD)((float)frame_duration/speed)) % frames.size();
	spriter.Draw(*(frames[frame].frame), x, y, z, frames[frame].centerX, frames[frame].centerY, rotation, scaleX, scaleY, color);
}

AnimationFrame &Animation::GetCurrentFrame(float speed)
{
	int frame = (World::GetCurrentTime() / (DWORD)((float)frame_duration/speed)) % frames.size();
	return frames[frame];
}

SpecificAnimation::SpecificAnimation(const shared_ptr<Animation> &anim, bool loop) : animation(anim)
{
	looped = loop;
	current_frame = 0;
	last_tick = 0;
	time = 0;
	animating = false;
}

void SpecificAnimation::Animate(Spriter &spriter, float x, float y, float z, float rotation, float scaleX, float scaleY, D3DCOLOR color, float speed)
{
	spriter.Draw(*(animation->frames[current_frame].frame), x, y, z, animation->frames[current_frame].centerX, animation->frames[current_frame].centerY, rotation, scaleX, scaleY, color);
}

AnimationFrame &SpecificAnimation::GetCurrentFrame()
{
	return animation->frames[current_frame];
}

bool SpecificAnimation::AnimateElapse(Spriter &spriter, float x, float y, float z, float rotation, float scaleX, float scaleY, D3DCOLOR color, float speed)
{
	bool end = false;
	Animate(spriter, x, y, z, rotation, scaleX, scaleY, color, speed);
	if (last_tick != World::GetTick())
	{
		last_tick = World::GetTick();
		end = ElapseTime();
		if (end)
			return true;
	}
	return false;
}

void SpecificAnimation::Start()
{
	animating = true;
}

void SpecificAnimation::Stop()
{
	animating = false;
}

void SpecificAnimation::Reset()
{
	current_frame = 0;
	time = 0;
}

void SpecificAnimation::ChangeFrame(int f)
{
	current_frame = f;
	time = 0;
}

bool SpecificAnimation::ElapseTime()
{
	 if (!animating)
		 return false;
	 bool end = false;
	 time += World::GetElapsedTime();
	 while(time >= animation->frames[current_frame].duration)
	 {
		 time -= animation->frames[current_frame].duration;
		 if (++ current_frame >= (int)animation->frames.size())
		 {
			 if (looped)
				 current_frame = 0;
			 else
			 {
				 current_frame --;
				 Stop();
			 }


			 end = true;
			 
		 }
	 }
	 return end;
}
