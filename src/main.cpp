#include <stdio.h>
#include <iostream>
#include "application/application.h"
#include "application/window.h"
#include "game/ScriptInitializer.h"
#include "game/resources/Paths.h"
#include "game/resources/Resources.h"
#include "graphic/images/Image.h"
#include "graphic/opengl/Program.h"
#include "graphic/opengl/Buffer.h"
#include "graphic/opengl/Uniform.h"
#include "graphic/renderer/MVP.h"
#include "graphic/opengl/Utils.h"

#include "devices/Keyboard.h"

#include "graphic/renderer/InstancedRenderer.h"
#include "graphic/images/TextureImage.h"
#include "utils/ListFiles.h"

#include "Game/TestManager.h"
#include "Game/GameManager.h"
#include "Game/GameInitializer.h"

class Bootstrap : public MX::App
{
public:
	void SetResPath()
	{
#ifdef __EMSCRIPTEN__
		auto res_path = "/res/";
#else
#ifndef MX_GAME_RELEASE
		auto res_path = MX_DEBUG_RES_PATH;
#else
#if MX_PLATFORM_XCODE
		auto res_path = MX_DEBUG_RES_PATH;
#else
		auto res_path = "res/";
#endif
#endif

#ifdef MX_GAME_PERF_CHEATS
		res_path = MX_DEBUG_RES_PATH;
#endif
#endif

		MX::Paths::get().SetResourcePath(res_path);
		MX::Paths::get().SetImagePath(res_path);
		MX::Paths::get().SetSoundPath(res_path);
		MX::Paths::get().SetStreamPath(res_path);
	}

	void OnPrepare() override
	{
		SetResPath();
		OpenMainWindow(1280, 800, false);

		MX::Window::current().keyboard()->on_specific_key_down[SDL_SCANCODE_ESCAPE].static_connect([&]() { Quit(); });
		Boro::GameInitializer::Init();
		MX::ScriptInitializer::ReloadScripts();
	}

	void OnRender() override
	{
		MX::gl::Clear({ 0.0f, 0.0f, 0.0f, 1.0f });

		Boro::GameManager::get().Draw();
	}

	void OnLoop() override
	{
		Boro::GameManager::get().Run();
	}
};

int main(int argc, char* argv[])
{
	Bootstrap app;
	app.Run();
	return 0;
}
