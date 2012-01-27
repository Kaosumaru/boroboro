#include "stdafx.h"
#include "MXAnimation.h"
#include "MXUtils.h"
#include "MXScene.h"

#define RESY 768.0

using namespace MX;

void Actor::Do() 
{ 
	OnDo(*this);
}

 void Actor::Die()
 {
	 OnDeath(*this);
 }



ImageSprite::ImageSprite(const shared_ptr<Image> &im) : image(im)
{
	//cx = cy = 0.0f;
}

void ImageSprite::Do()
{
	Actor::Do();
	if (image->IsImageLoaded())
		ActorSprite::spriter->Draw(*image, pos.x, pos.y, z, cpos.x, cpos.y, rotation, scaleX, scaleY, color);
}



Spriter *ActorSprite::spriter = NULL;

ActorSprite::ActorSprite(const shared_ptr<Animation> &anim) 
{
	animation = shared_ptr<SpecificAnimation>(new SpecificAnimation(anim));
}

void ActorSprite::Do() 
{
	Actor::Do();
	DrawSprite();
	
}

void ActorSprite::DrawSprite()
{
	animation->AnimateElapse(*spriter, pos.x, pos.y, z == -1.0f ? 0.5f - (pos.y/RESY)*0.1f : z, rotation, scaleX, scaleY, color);
}

AnimationFrame &ActorSprite::GetCurrentFrame()
{
	return animation->GetCurrentFrame();
}

void ActorSprite::GetCurrentRect(float &_x, float &_y, float &_w, float &_h)
{
	AnimationFrame &frame = GetCurrentFrame();
	_x = pos.x - frame.centerX;
	_y = pos.y - frame.centerY;
	_w = frame.frame->Width();
	_h = frame.frame->Height();
}

bool ActorSprite::Collide(ActorSprite &other)
{
	float x1, y1, w1, h1;
	float x2, y2, w2, h2;
	GetCurrentRect(x1, y1, w1, h1);
	other.GetCurrentRect(x2, y2, w2, h2);

	if (x2 <= x1 + w1 && x2 + w2 >= x1)
		if (y2 <= y1 + h1 && y2 + h2 >= y1)
			return true;

	return false;
}


Scene::Scene()
{
}

void Scene::Draw()
{	
	auto it = lActors.begin();
	while ( it != lActors.end())
	{
		Actor &a = *(*it);
		if (!a.alive)
		{
			a.Die();
			it = lActors.erase(it);
		}
		else
		{
			a.Do();
			if (!a.alive)
			{
				a.Die();
				it = lActors.erase(it);
			}
			else 
				it ++;
		}
	}
}