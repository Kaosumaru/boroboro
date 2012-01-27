#pragma once
#pragma warning( disable:4996 )

#include "stdio.h"
#include "../MXLib/MXImage.h"
#include "../MXLib/MXDraw.h"
#include "../mxlib/MXScene.h"

using namespace MX;

namespace TileType{
	enum Type{
		WALL,FLOOR
	};
}

namespace TileLayer{
	enum Type{
		FLOOR,WALL
	};
}

struct Tile{
	TileType::Type type;
	int image;
	shared_ptr<ImageSprite> actor;
};

class GameMap{
private:
	int width, height;
	Tile *tiles;
	int imageCount;
	shared_ptr<Image> *images;

public:
	~GameMap(){
		delete[] images;
		delete[] tiles;
	}
	void load(const wchar_t* filename,Draw &draw){
		FILE *file = _wfopen(filename,_T("rt"));
		if(!file)
			return;
		fscanf(file,"%i %i %i\n",&width,&height,&imageCount);
		images=new shared_ptr<Image>[imageCount];
		wchar_t imageFile[100];
		for(int i=0;i<imageCount;i++){
			fwscanf(file,_T("%s\n"),imageFile);
			images[i]=shared_ptr<Image>(new Image());
			images[i]->Load(draw,imageFile);
		}

		// read images
		tiles=new Tile[width*height];
		for(int i=0;i<height;i++){
			for(int j=0;j<width;j++){
				int c;
				fscanf(file,"%i",&c);
				tiles[i*width+j].image=c;
				ImageSprite *sprite=new ImageSprite(images[tiles[i*width+j].image]);
				tiles[i*width+j].actor=shared_ptr<ImageSprite>(sprite);
			}
		}

		// read type
		for(int i=0;i<height;i++){
			for(int j=0;j<width;j++){
				int c;
				fscanf(file,"%i",&c);
				tiles[i*width+j].type=(TileType::Type)c;
			}
		}
	}
	Tile &get(int w, int h){
		return tiles[h*width+w];
	}
	int getHeight() const { return height; }
	int getWidth() const { return width; }
	shared_ptr<Image> getImage(int i){
		return images[i];
	}
	int getImageCount(){
		return imageCount;
	}

	void addToScene(MX::Scene *scene){
		for(int i=0;i<height;i++){
			for(int j=0;j<width;j++){
				shared_ptr<ImageSprite> tile = tiles[i*width+j].actor;
				tile->x=(float)j*tile->GetImage()->Width();
				tile->y=(float)i*tile->GetImage()->Height();
				tile->z=1.0f;
				scene->AddActor(tile);
			}
		}
	}
};