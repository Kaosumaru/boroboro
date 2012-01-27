#pragma once

#include "vector"
#include "../MXLib/MXScene.h"
#include "../MXLib/MXDraw.h"
#include "../MXLib/MXSpriter.h"
#include "Game.h"

using namespace MX;
using namespace std;

namespace FrameType{
	enum Type{
		TITLE,STORY1,STORY2,STORY3,STORY4,GAME,OVER,NUM_FRAMES
	};
}
class FrameManager{
	std::vector<Scene> scenes;
	FrameType::Type currentFrame;
	shared_ptr<Image> image;
public:
	FrameManager()
	{
		image = shared_ptr<Image>(new Image());
	}



	FrameType::Type getCurrentFrame(){
		return currentFrame;
	}
	void setCurrentFrame(const shared_ptr<MX::Draw> &_draw, FrameType::Type frame){
		currentFrame=frame;
		switch (frame)
		{
		case FrameType::STORY1:
			image->Load(*_draw,_T("images/fabula/1.png"));
			break;
		case FrameType::STORY2:
			image->Load(*_draw,_T("images/fabula/2.png"));
			break;
		case FrameType::STORY3:
			image->Load(*_draw,_T("images/fabula/3.png"));
			break;
		case FrameType::STORY4:
			image->Load(*_draw,_T("images/fabula/4.png"));
			break;
		case FrameType::GAME:
			image->Deinitialize();
			break;
		case FrameType::OVER:
			image->Load(*_draw,_T("images/fabula/game_over.png"));
			break;

		}
	}
	void initScenes(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter){
		{
			Scene titleScene;
			image->Load(*_draw,_T("images/menu/start_wzor.png"));
			shared_ptr<ImageSprite> actor=shared_ptr<ImageSprite>(new ImageSprite(image));
			actor->x=0;actor->y=0;actor->z=0;
			titleScene.AddActor(actor);
			scenes.push_back(titleScene);
		}
		{
			Scene storyScene;
			
			shared_ptr<ImageSprite> actor=shared_ptr<ImageSprite>(new ImageSprite(image));
			actor->x=0;actor->y=0;actor->z=0;
			storyScene.AddActor(actor);
			scenes.push_back(storyScene);
		}
		{
			Scene storyScene;
			shared_ptr<ImageSprite> actor=shared_ptr<ImageSprite>(new ImageSprite(image));
			actor->x=0;actor->y=0;actor->z=0;
			storyScene.AddActor(actor);
			scenes.push_back(storyScene);
		}
		{
			Scene storyScene;
			shared_ptr<ImageSprite> actor=shared_ptr<ImageSprite>(new ImageSprite(image));
			actor->x=0;actor->y=0;actor->z=0;
			storyScene.AddActor(actor);
			scenes.push_back(storyScene);
		}
		{
			Scene storyScene;
			shared_ptr<ImageSprite> actor=shared_ptr<ImageSprite>(new ImageSprite(image));
			actor->x=0;actor->y=0;actor->z=0;
			storyScene.AddActor(actor);
			scenes.push_back(storyScene);
		}
		{
			Scene gameScene;
			shared_ptr<ImageSprite> actor=shared_ptr<ImageSprite>(new ImageSprite(image));
			actor->x=0;actor->y=0;actor->z=-100;
			gameScene.AddActor(actor);

			InitializeGame(_draw,_spriter,&gameScene);
			scenes.push_back(gameScene);
		}
		{
			Scene loseScene;
			shared_ptr<ImageSprite> actor=shared_ptr<ImageSprite>(new ImageSprite(image));
			actor->x=0;actor->y=0;actor->z=0;
			loseScene.AddActor(actor);
			scenes.push_back(loseScene);
		}
	}
	Scene *getScene(FrameType::Type frame){
		return &(scenes[(int)frame]);
	}

	Scene *getScene(){
		return &(scenes[(int)currentFrame]);
	}
};