// MyXLib.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MyXLib.h"
#include <memory>
#include "../MXLib/MXDraw.h"
#include "../MXLib/MXWorld.h"
#include "../MXLib/MXSound.h"

#include "../MXLib/MXSpriter.h"
#include "../MXLib/MXParticles.h"
#include "../MXLib/MXAnimation.h"
#include "../MXLib/MXAnimUtils.h"

#include "../MXLib/MXFont.h"
#include "../MXLib/MXLine.h"
#include "GameMap.h"
#include "game.h"

using namespace std;

#define MAX_LOADSTRING 100






// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
MX::Stream menu1Music,menu2Music,game1Music,winMusic;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

shared_ptr<MX::Draw> draw1;



int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	auto General_Scene = make_shared<MX::Scene>();
	
	
	draw1 = shared_ptr<MX::Draw>(new MX::Draw);
	shared_ptr<MX::Spriter> spriter(new MX::Spriter(draw1));
	
	if (!draw1->Initialize())
		return -1;


	if (!draw1->ChangeView(1280, 800, false))
		return -1;


	if (!spriter->Init())
		return -1;
	
	bool bs = MX::Sound::Initialize();


	initGame(draw1,spriter,&(*General_Scene));

	
	/*
	shared_ptr<MX::Image> image(new MX::Image), image2(new MX::Image), image3(new MX::Ima0ge);
	image->Load(*draw1, L"images\\plansza_1_layer1.png");
	image2->Load(*draw1, L"images\\plansza_1_layer2.png");
	image3->Load(*draw1, L"images\\interface_main.png");
	*/


	/*
	menu1Music.LoadStream(L"music\\menu.mp3");
	menu2Music.LoadStream(L"music\\menu2.mp3");
	game1Music.LoadStream(L"music\\game.mp3");
	winMusic.LoadStream(L"music\\win.mp3");
	menu1Music.Play();*/

	
	
	
	shared_ptr<MX::Liner> liner = shared_ptr<MX::Liner>(MX::Liner::Create(draw1));
	liner->SetAntialias(true);
	liner->SetWidth(1.2f);

	MX::ActorSprite::spriter = spriter.get();
	MX::World::Initialize();
	srand(MX::World::GetTick());



	InitializeGame(draw1,spriter, &(*General_Scene));



/*
	auto part = make_shared<MX::ParticleGenerator<MX::SimpleParticleCreator, MX::SimpleParticleDispatcher<3,10>>>(General_Scene);
	part->creator.SetAnimation(MX::CreateAnimationFromFile(L"images\\serce1.png"));
	part->x = 200;
	part->y = 200;

	shared_ptr<MX::Command> com = MX::q(wait(1000), die());
	part->OnDo.connect(com);
	General_Scene->AddActor(part);*/






	while (MX::Window::Loop())
	{
		if (MX::World::Key[VK_ESCAPE])
			PostQuitMessage(0);

		MX::World::ElapseTime();
		if (draw1->IsDeviceReset())
		{

	
			draw1->Clear(0xFF007F0E);
			if (draw1->Begin())
			{
			
				if (spriter->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK))
				{


					//spriter->Draw(*image, 0.0f, 0.0f, 1.0f);
					General_Scene->Draw();
				//	spriter->Draw(*image2, 0.0f, 0.0f, 0.2f);
					//spriter->Draw(*image3, 0.1f, 768.0f - image3->Height(), 0.0f);
	
					spriter->ResetTransform();
					spriter->End();
				}

				draw1->End();
			}

			draw1->Present();
		}
		else draw1->Reset();
		//Sleep(1);
		
	};

	MX::Sound::Deinitialize();
	return 0;
}


