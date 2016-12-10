#ifndef BHTESTMANAGER
#define BHTESTMANAGER
#include<memory>
#include "scene/sprites/SpriteScene.h"
#include "scene/managers/SceneManager.h"
#include "utils/Utils.h"


class TestManager : public MX::DisplaySceneTimer, public DeinitSingleton<TestManager>, public MX::SignalTrackable
{
public:
    TestManager();

	void clearReloadScripts();
	void reloadScripts();


	void AddSomething();
	void Draw(float x = 0.0f, float y = 0.0f);
protected: 
    std::shared_ptr<struct CheatObject> _cheats;
};
    


#endif
