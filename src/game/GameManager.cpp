#include "GameManager.h"
#include "application/Application.h"
#include "Game/Resources/Paths.h"
#include "Game/Resources/Resources.h"
#include "Script/Script.h"
#include "Script/ScriptObject.h"
#include "Application/Window.h"
#include <iostream>
#include <iomanip>

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

#include "sound/Stream.h"
#include "Game/Resources/Resources.h"
#include "Game/DebugCheats.h"

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
	}

	void Draw(float x, float y)
	{
		Sort();
		BaseGraphicSceneScriptable::Draw(x, y);
	}

	void Sort()
	{
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

class GameOverScene  : public StandardScene
{
public:
	GameOverScene(int mode) : StandardScene("GameOver")
	{
#ifndef _DEBUG
		MX::Window::current().keyboard()->on_specific_key_down[SDL_SCANCODE_SPACE].connect([&]() { End(); }, this);
#endif
        MX::Window::current().keyboard()->on_specific_key_down[SDL_SCANCODE_RETURN].connect([&]() { End(); }, this);


        std::vector<std::string> images = { "images/end_remis.png", "images/end_player1.png", "images/end_player2.png" };
        auto image = Resources::get().loadImage(images[mode]);
        AddActor( std::make_shared<ImageSpriteActor>(image) );
	}

    void End();
protected:
};

class ItemWidget : public MX::Widgets::ButtonWidget
{
public:
	ItemWidget(const std::shared_ptr<Player> &player)
	{
		_player = player;
		AddStrategy(_iconStrategy);
	}

	void Run()
	{
		if (_player && _player->item())
			_iconStrategy.SetImage(_player->item()->item_image);
		else
			_iconStrategy.SetImage(nullptr);
		MX::Widgets::Widget::Run();
	}
protected:
	std::shared_ptr<Player> _player;
	MX::Strategies::Drawable::Image    _iconStrategy;
};

class GameScene : public StandardScene
{
#ifdef _DEBUG
	std::shared_ptr<CheatObject> _cheats = CreateCheats();
#endif
	std::shared_ptr<MX::Widgets::ScriptLayouterWidget> _bgLayouter;
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

        AddPlayers();

		CreateLayer("Foreground");
        AddOverlay();

		Boro::InitLevel(_world.get());

        _time.onValueChanged.static_connect( [&]( auto a, auto b ) { if (a != b) onTick(); } );

		Context<BaseGraphicScene>::SetCurrent(*_world);

		auto stream = MX::Resources::get().loadStream("music/game.ogg");
		MX::Sound::StreamManager::get().SetCurrent(stream);
	}

	~GameScene()
	{
		MX::Sound::StreamManager::get().SetCurrent(nullptr);
		MX::Sound::Sample::StopAll();
	}

    void AddPlayers()
    {
        ScriptObjectString script("Game");
        script.load_property( _players, "Players" );
        for ( auto& player : _players )
			_world->AddActor( player );
        //
    }

    void AddOverlay()
    {
        {
            auto bg = std::make_shared<MX::Widgets::ScriptLayouterWidget>();
            bg->AddStrategy(std::make_shared<MX::Strategies::FillInParent>());
            bg->SetLayouter("GUI.Game.Layouter");
            AddActor(bg);
            _bgLayouter = bg;
        }

        {
            auto time = std::make_shared<MX::Widgets::AutoLabel>();
            time->SetStringBuilder( [&]() 
            {
                std::wstringstream ss;
                ss << std::setfill(L'0') << std::setw(1) << _time.directValueAccess() / 60;
                ss << ":";
                ss << std::setfill(L'0') << std::setw(2) << _time.directValueAccess() % 60;
                return ss.str();
            });
            time->connect_signal( _time.onValueChanged );
            _bgLayouter->AddNamedWidget("Label.Time", time);
        }


		{
			auto createPoints = []( auto p )
			{
				auto points = std::make_shared<MX::Widgets::AutoLabel>();
				points->SetStringBuilder( [x = p.get()]() 
				{
					return std::to_wstring(x->score);
				});
				points->connect_signal( p->score.onValueChanged );
				return points;
			};

			_bgLayouter->AddNamedWidget("Label.Player1.Points", createPoints(_players[0]));
			_bgLayouter->AddNamedWidget("Label.Player2.Points", createPoints(_players[1]));
		}

		_bgLayouter->AddNamedWidget("Player1.Item", std::make_shared<ItemWidget>(_players[0]));
		_bgLayouter->AddNamedWidget("Player2.Item", std::make_shared<ItemWidget>(_players[1]));
    }

	void Run() override
	{
        if (_time > 0 )
            _time = maxTime - (int)_timer.total_seconds();

		auto scene_guard = Context<MX::SpriteScene, Boro::World_Tag>::Lock(*_world);
		MX::FullscreenDisplayScene::Run();

        if ( _time == 0 )
            End();
	}

    void End()
    {
        int mode = 0;
        if ( _players[0]->score > _players[1]->score )
            mode = 1;
        if ( _players[0]->score > _players[1]->score )
            mode = 2;

	    auto menu = std::make_shared<GameOverScene>(mode);
	    SpriteSceneStackManager::manager_of(this)->SelectScene(menu);
    }
protected:
    void onTick()
    {
		if (_time <= (maxTime * 2) / 3)
		{
			float speed = 150.0f + (float)_time/(float)maxTime * 100.0f;
            for ( auto &p : _players ) p->speed = speed;
        }

        for ( auto &p : _players )
        {
            p->score = p->score + p->GetLength();
        }
    }

    const int maxTime = 180;
    SignalizingVariable<int> _time = maxTime;
	std::shared_ptr<BaseGraphicScene> _world;
    std::vector<std::shared_ptr<Player>> _players;
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


void GameOverScene::End()
{
	Sound::Sample::StopAll();
	auto game = std::make_shared<MenuScene>();
	SpriteSceneStackManager::manager_of(this)->SelectScene(game);
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









