#include "stdafx.h"
#include "TitleScreen.h"
#include "GameResources.h"
#include "Sounds.h"
#include "Game.h"
#include "../MXLib/MXAnimUtils.h"
#include "../MXLib/MXScript.h"
#include "../MXLib/MXSpriter.h"



namespace MX
{

class ShowGame : public Actor
{
public:
	ShowGame(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene)
	{
		draw = _draw;
		spriter = _spriter;
		scene = _scene;
	}

	void Show()
	{
		InitializeDemo(draw,spriter, scene);
	}

	shared_ptr<MX::Draw> draw; 
	shared_ptr<MX::Spriter> spriter;
	MX::Scene *scene;
};


class Background2 : public ActorSprite
{
public:
	Background2(MX::Scene *_scene, const shared_ptr<Animation> &anim = NULL) : ActorSprite(anim)
	{
		scene = _scene;
	}
	
	void Do()
	{
		ActorSprite::Do();
		if (World::Key[VK_SPACE] || World::Key[VK_RETURN])
			scene->KillAll();
	}

protected:
	MX::Scene *scene;

};


void InitializeTitle(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene)
{


	SoundBank::AmbientSound.Play();

	static auto title_background = MX::CreateAnimationFromFile(*_draw, L"images\\wielki_pixel.png");
	title_background->frames[0].centerX = 0.0f;
	title_background->frames[0].centerY = 0.0f;

	static auto title_background2 = MX::CreateAnimationFromFile(*_draw, L"images\\wielki_pixel2.png");
	title_background2->frames[0].centerX = 0.0f;
	title_background2->frames[0].centerY = 0.0f;

	auto splash1 = make_shared<ActorSprite>(title_background);
	splash1->color = 0x00FFFFFF;

	auto splash2 = make_shared<Background2>(_scene, title_background2);
	splash2->color = 0x00FFFFFF;


	splash1->OnDo.connect(q(wait(1000), lerp_color(0xFFFFFFFF, 5000), lerp_color(0x00FFFFFF, 5000), die()));
	splash2->OnDo.connect(q(wait(5000), lerp_color(0xFFFFFFFF, 1000), wait(3000), die()));
	
	_scene->AddActor(splash2);
	_scene->AddActor(splash1);
	
	auto show = make_shared<ShowGame>(_draw, _spriter, _scene);
	splash2->OnDeath.boost::signal<void (Actor&)>::connect(bind(&ShowGame::Show, show));

}

};