#include "stdafx.h"
#include "GameBackground.h"
#include "GameResources.h"
#include "Highscore.h"
#include "Game.h"

namespace MX
{
MX::Scene *scene;
shared_ptr<MX::Spriter> spriter;
shared_ptr<Image> digits;

class DigitsDrawer
{
public:
	static void DrawDigit(Spriter &spriter, const shared_ptr<Image> &image, float x, float y, float z, int digit, DWORD color = 0xFFFFFFFF,  float scale = 1.0f)
	{
		//int w = image->Width() / 10;
		int w = 40;
		RECT r;
		r.top = 0;
		r.bottom = image->Height();
		r.left = w * digit;
		r.right = r.left + w;
		spriter.Draw(*image, x, y, z, 0.0f, 0.0f, 0.0f, scale, scale, color, &r);
		
	}

	static void DrawText(Spriter &spriter, const shared_ptr<Image> &image, float x, float y, float z, const char *strText, DWORD color = 0xFFFFFFFF,  float scale = 1.0f)
	{
		int len = strlen(strText);
		//int w = image->Width() / 10;
		int w = 40;
	
		for (int i = 0; i < len; i ++)
		{
			int number = strText[i]-L'0';
			if (number >= 0 && number < 10)
				DrawDigit(spriter, image, x + (float)(i*w)*scale, y, z, strText[i]-L'0', color, scale);
			else
				DrawDigit(spriter, image, x + (float)(i*w)*scale, y, z, 10, color, scale);

		}
	}

	static void DrawTextRight(Spriter &spriter, const shared_ptr<Image> &image, float x, float y, float z, const char *strText, DWORD color = 0xFFFFFFFF,  float scale = 1.0f)
	{
		int len = strlen(strText);
		//int w = image->Width() / 10;
		int w = 40;
		x -= (float)(len * w) * scale;
		DrawText(spriter, image, x, y, z, strText, color, scale);
	}
};


class HiPoints : public Actor
{
public:
	HiPoints(const shared_ptr<MX::Player> &p, bool right = false) : cooldown(1000)
	{
		align_right = right;
		strcpy(pointstext, "0");
		points = 0;
		player = p;
	}

	void Do()
	{
		if (cooldown.DoThis())
		{
			points += player->GetLength();
			itoa(points, pointstext, 10);
		}

		if (align_right)
			DigitsDrawer::DrawTextRight(*spriter, digits, pos.x, pos.y, z, pointstext, color, 0.5f);
		else
			DigitsDrawer::DrawText(*spriter, digits, pos.x, pos.y, z, pointstext, color, 0.5f);
		Actor::Do();
	}

	bool align_right;
	EffectWithGivenCooldown cooldown;
	int points;
	char pointstext[256];
	shared_ptr<MX::Player> player;
};


class TimeLimit : public Actor
{
public:
	TimeLimit() : cooldown(1000)
	{
		strcpy(pointstext, "2:00");
		elapsed_time = 0;
		max_time = 120;
	}

	void Do()
	{
		GetCurrentTime();
		if (cooldown.DoThis())
		{
			elapsed_time ++;
			int remaining = max_time - elapsed_time;
			if (remaining > 0)
				sprintf(pointstext, "%02d:%02d", remaining/60, remaining%60);
		}
		DigitsDrawer::DrawText(*spriter, digits, pos.x, pos.y, z, pointstext, color);
		Actor::Do();
	}

	
	int elapsed_time;
	int max_time;
	EffectWithGivenCooldown cooldown;
	char pointstext[256];
};


};
void InitHighscore(const shared_ptr<MX::Draw> &draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene, const shared_ptr<MX::Player> &player1, const shared_ptr<MX::Player> &player2)
{
	MX::scene = _scene;
	MX::spriter = _spriter;
	MX::digits = make_shared<MX::Image>();
	MX::digits->Load( *draw, L"images\\digits.png");

	auto hi1 = make_shared<MX::HiPoints>(player1);
	hi1->pos.x = 35;
	hi1->pos.y = 745;
	hi1->z = 0.0f;
	hi1->color = 0xFF00FF00;
	MX::scene->AddActor(hi1);


	auto hi2 = make_shared<MX::HiPoints>(player2);
	hi2->pos.x = 1160;
	hi2->pos.y = 745;
	hi2->z = 0.0f;
	hi2->color = 0xFFFF0000;
	MX::scene->AddActor(hi2);

	auto time = make_shared<MX::TimeLimit>();
	time->pos.x = 500;
	time->pos.y = 5;
	time->z = 0.0f;
	time->color = 0xFFFFFFFF;
	MX::scene->AddActor(time);
}
