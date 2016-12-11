#pragma once
#include<memory>
#include "Scene/Sprites/SpriteScene.h"
#include "Scene/Managers/SceneStackManager.h"
#include "Utils/Utils.h"

namespace Boro
{
    
class GameManager : public MX::SpriteSceneStackManager, public DeinitSingleton<GameManager>
{
public:
	GameManager();
};
    
}


