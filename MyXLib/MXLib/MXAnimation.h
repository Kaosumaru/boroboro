#ifndef MXLIB_ANIMATION
#define MXLIB_ANIMATION

#include <list>
#include "MXUtils.h"
#include "MXImage.h"
#include "MXSpriter.h"

namespace MX
{

class AnimationFrame
{
public:
	AnimationFrame(const shared_ptr<Frame> &f, unsigned dur = 0, float cx = 0.0f, float cy = 0.0f) : frame(f), duration(dur), centerX(cx), centerY(cy) {}
	~AnimationFrame(){};

	unsigned duration;
	float centerX, centerY;
	shared_ptr<Frame> frame;
};

class Animation
{
public:
	Animation(unsigned frame_dur = 100) : frame_duration(frame_dur) {}

	void AddFrame(const AnimationFrame &frame) { frames.push_back(frame); if (frames.back().duration == 0) frames.back().duration = frame_duration; }

	void Animate(Spriter &spriter, float x, float y, float z = 0.0f, float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f, D3DCOLOR color = 0xFFFFFFFF, float speed = 1.0f);

	AnimationFrame &GetCurrentFrame(float speed);

	unsigned frame_duration;
	vector<AnimationFrame> frames;
};

class SpecificAnimation
{
public:
	SpecificAnimation(const shared_ptr<Animation> &anim, bool loop = true);

	void Animate(Spriter &spriter, float x, float y, float z = 0.0f, float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f, D3DCOLOR color = 0xFFFFFFFF, float speed = 1.0f);
	AnimationFrame &GetCurrentFrame();

	//automatically calls ElapseTime,
	bool AnimateElapse(Spriter &spriter, float x, float y, float z = 0.0f, float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f, D3DCOLOR color = 0xFFFFFFFF, float speed = 1.0f);

	void Start();
	void Stop();
	void Reset();
	void ChangeFrame(int f);

	//return true, if loop ended
	bool ElapseTime();

protected:
	bool looped;
	bool animating;

	shared_ptr<Animation> animation;
	int   current_frame;
	int   last_tick;
	DWORD time;
};


}

#endif
