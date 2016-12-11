#pragma once
#include <memory>
#include "Collidable.h"
#include "devices/Keyboard.h"
#include "application/window.h"
using namespace std;

namespace Boro
{
	class Controller : public MX::ScriptObject
	{
	public:
		Controller() {}
		virtual ~Controller() {}

		virtual float direction() { return 0.f; }
		virtual bool useItem() { return false; }
	};

	class KeyController : public Controller
	{
	public:
		KeyController(MX::LScriptObject& script) 
		{
			script.load_property(KeyLeft, "Left");
			script.load_property(KeyRight, "Right");
			script.load_property(KeyUse, "Use");
		}

		float direction() override 
		{ 
			auto &keyboard = MX::Window::current().keyboard();
			if (keyboard->key(KeyLeft))
				return -1.0f;
			if (keyboard->key(KeyRight))
				return 1.0f;
			return 0.f; 
		}
		bool useItem() override 
		{ 
			auto &keyboard = MX::Window::current().keyboard();
			return keyboard->key(KeyUse); 
		}
	protected:
		char KeyLeft = 0;
		char KeyRight = 0;
		char KeyUse = 0;
	};

};

