#include "stdafx.h"
#include "TitleScreen.h"
#include "GameResources.h"
#include "../MXLib/MXAnimUtils.h"
#include "../MXLib/MXScript.h"
#include "../MXLib/MXSpriter.h"



namespace MX
{



void InitializeTitle(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene)
{

	static auto title_background = MX::CreateAnimationFromFile(*_draw, L"images\\splash1.png");
	title_background->frames[0].centerX = 0.0f;
	title_background->frames[0].centerY = 0.0f;

	auto splash1 = make_shared<ActorSprite>(title_background);

	splash1->color = 0x00FFFFFF;

	splash1->OnDo.connect(q(wait(1000), lerp_color(0xFFFFFFFF, 400000), die()));

	_scene->AddActor(splash1);
	
}

};