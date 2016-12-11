#include "GameManager.h"
#include "application/Application.h"
#include "Game/Resources/Paths.h"
#include "Game/Resources/Resources.h"
#include "Script/Script.h"
#include "Script/ScriptObject.h"
#include "Application/Window.h"
#include <iostream>

#include "Widgets/Widget.h"
#include "Widgets/Layouters/StackWidget.h"
#include "Widgets/Label.h"
#include "Widgets/Controllers/ListController.h"
#include "Widgets/Layouters/SimpleLayouters.h"
#include "Widgets/Layouters/ScriptLayouters.h"

#include "Devices/Keyboard.h"

#include "Widgets/Animations/Animations.h"
#include "main/Player.h"
#include "main/Pickups.h"

using namespace MX;
using namespace Boro;
using namespace std;


class ZSortedGraphicSceneScriptable : public BaseGraphicSceneScriptable
{
public:
	using BaseGraphicSceneScriptable::BaseGraphicSceneScriptable;

	void Run() override
	{
		BaseGraphicSceneScriptable::Run();
		_actors.sort([](auto &a, auto &b)
		{
			if (!a)
				return false;
			if (!b)
				return true;
			return a->geometry.z > b->geometry.z;
		});
	}
};

class StandardScene : public MX::FullscreenDisplayScene, public MX::SignalTrackable
{
protected:
	StandardScene(const std::string& name) : _name(name)
	{
	}

	void CreateLayer(const std::string &name)
	{
		std::shared_ptr<SpriteActor> sprite;
		ScriptObjectString script(_name);
		script.load_property(sprite, name);
		AddActor(sprite);
	}
protected:
	std::string _name;
};

class GameScene : public StandardScene
{
public:
	GameScene() : StandardScene("Game")
	{
#ifdef _DEBUG	
		MX::Window::current().keyboard()->on_specific_key_down[SDL_SCANCODE_SPACE].connect([&]() { End(); }, this);
#endif

		CreateLayer("Background");
		
		_world = std::make_shared<ZSortedGraphicSceneScriptable>();
		AddActor(_world);
		auto scene_guard = Context<MX::SpriteScene, Boro::World_Tag>::Lock(*_world);

		CreateLayer("Game");
		CreateLayer("Foreground");

		Boro::InitLevel(_world.get());
	}

	void Run() override
	{
		auto scene_guard = Context<MX::SpriteScene, Boro::World_Tag>::Lock(*_world);
		MX::FullscreenDisplayScene::Run();
	}

	void End();
protected:
	std::shared_ptr<SpriteScene> _world;
};


class MenuScene  : public StandardScene
{
public:
	MenuScene() : StandardScene("Menu")
	{
		MX::Window::current().keyboard()->on_specific_key_down[SDL_SCANCODE_RETURN].connect([&]() { End(); }, this);

		CreateLayer("Menu.Background");
		_world = std::make_shared<ZSortedGraphicSceneScriptable>();
		AddActor(_world);
		auto scene_guard = Context<MX::SpriteScene, Boro::World_Tag>::Lock(*_world);
		CreateLayer("Menu.Snakes");
		CreateLayer("Menu.Foreground");
	}

	void Run() override
	{
		auto scene_guard = Context<MX::SpriteScene, Boro::World_Tag>::Lock(*_world);
		MX::FullscreenDisplayScene::Run();
	}

	void End()
	{
		Sound::Sample::StopAll();
		auto game = std::make_shared<GameScene>();
		SpriteSceneStackManager::manager_of(this)->SelectScene(game);
	}
protected:
	std::shared_ptr<SpriteScene> _world;
};


void GameScene::End()
{
	auto menu = std::make_shared<MenuScene>();
	SpriteSceneStackManager::manager_of(this)->SelectScene(menu);
}


class IntroScene : public MX::FullscreenDisplayScene, public MX::SignalTrackable
{
public:
	IntroScene()
    {
		MX::Window::current().keyboard()->on_specific_key_down[SDL_SCANCODE_RETURN].connect([&]() { End(); }, this);
		MX::Window::current().keyboard()->on_specific_key_down[SDL_SCANCODE_SPACE].connect([&]() { End(); }, this);

		std::shared_ptr<SpriteActor> sprite;
		ScriptObjectString script("Intro");
		script.load_property(sprite, "Intro");
		AddActor(sprite);
    }

	void Run() override
	{
		MX::FullscreenDisplayScene::Run();
		if (empty())
			End();
	}

	void End()
	{
		auto menu = std::make_shared<MenuScene>();
		SpriteSceneStackManager::manager_of(this)->SelectScene(menu);
	}
protected:
};




GameManager::GameManager()
{
#ifdef _DEBUG
	auto game = std::make_shared<GameScene>();
	SelectScene(game);
#else
    auto intro = std::make_shared<IntroScene>();
	SelectScene(intro);
#endif

#ifndef MX_GAME_RELEASE
//    menu->OnGame(1, false);
#endif
}









