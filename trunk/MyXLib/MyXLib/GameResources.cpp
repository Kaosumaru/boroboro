#include "stdafx.h"
#include "GameResources.h"
#include "../MXLib/MXAnimUtils.h"


GraphicResources::GraphicResources()
{
}


void GraphicResources::Init(MX::Draw &draw)
{
	snake_head = MX::CreateAnimationFromFile(draw, L"images\\wonsz\\WonszGlowa.png");
	snake_body = MX::CreateAnimationFromFile(draw, L"images\\wonsz\\WonszBody.png");

	flower1 = MX::CreateAnimationFromFile(draw, L"images\\flower\\flower1.png");
	flower2 = MX::CreateAnimationFromFile(draw, L"images\\flower\\flower2.png");


	background =  MX::CreateAnimationFromFile(draw, L"images\\background.png");
	
}


GraphicResources GraphicRes;