#include "stdafx.h"
#include "GameResources.h"
#include "../MXLib/MXAnimUtils.h"

using namespace std;

GraphicResources::GraphicResources()
{
}


void GraphicResources::Init(MX::Draw &draw)
{
	snake_head = MX::CreateAnimationFromFile(draw, L"images\\wonsz\\WonszGlowa.png");

	shared_ptr<MX::Image> cross(new MX::Image());
	bool b = cross->Load( draw, L"images\\wonsz\\WonszGlowa2.png");
	shared_ptr<MX::Frame> frame(new MX::Frame(cross));
	snake_head->AddFrame(MX::AnimationFrame(frame, 100, cross->Width()/2.0f, cross->Height()/2.0f));


	snake_body = MX::CreateAnimationFromFile(draw, L"images\\wonsz\\WonszBody.png");

	flower1 = MX::CreateAnimationFromFile(draw, L"images\\flower\\flower1.png");
	flower2 = MX::CreateAnimationFromFile(draw, L"images\\flower\\flower2.png");


	background =  MX::CreateAnimationFromFile(draw, L"images\\background.png");
	background->frames[0].centerX = 0.0f;
	background->frames[0].centerY = 0.0f;

	
	grass[0] = MX::CreateAnimationFromFile(draw, L"images\\Background\\kepka_01.png");
	grass[1] = MX::CreateAnimationFromFile(draw, L"images\\Background\\kepka_02.png");
	grass[2] = MX::CreateAnimationFromFile(draw, L"images\\Background\\kepka_03.png");
	grass[3] = MX::CreateAnimationFromFile(draw, L"images\\Background\\kepka_04.png");

	grass[0]->frames[0].centerX = 256.0f;
	grass[0]->frames[0].centerY = 128.0f;

	grass[1]->frames[0].centerX = 256.0f;
	grass[1]->frames[0].centerY = 128.0f;

	grass[2]->frames[0].centerX = 256.0f;
	grass[2]->frames[0].centerY = 128.0f;
}


GraphicResources GraphicRes;