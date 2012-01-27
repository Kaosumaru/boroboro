#pragma once

#include "../MXLib/MXImage.h"
#include "../MXLib/MXScene.h"

using namespace MX;

class LifeBar{
private:
	shared_ptr<ImageSprite> bar,portrait;
	int maxLife,currentLife;

public:
	void init(Draw &draw,TCHAR portraitFile,TCHAR barFile){
		{
			shared_ptr<Image> image=shared_ptr<Image>(new Image());
			image->Load(*draw,portraitFile);
			shared_ptr<ImageSprite> actor=shared_ptr<ImageSprite>(new ImageSprite(image));
			portrait->x=0;portrait->y=0;portrait->z=0;
		}
		{
			shared_ptr<Image> image=shared_ptr<Image>(new Image());
			image->Load(*draw,barFile);
			bar=shared_ptr<ImageSprite>(new ImageSprite(image));
			bar->x=0;bar->y=0;bar->z=0;
		}
	}
	void addToScene(Scene *scene){
		scene->AddActor(portrait);
		scene->AddActor(bar);
	}
	void setMaxLife(int life){
		maxLife = life;
		bar->scaleX=(float)currentLife/(float)maxLife;
	}
	void setCurrentLife(int life){
		currentLife=life;
		bar->scaleX=(float)currentLife/(float)maxLife;
	}
};