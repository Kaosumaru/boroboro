#include "stdafx.h"
#include "MXWorld.h"
#include <windows.h>

using namespace MX;

DWORD World::elapsed = 0;
DWORD World::time = 0;
DWORD World::tick = 0;
float World::sec = 0.0f;
Timer World::timer;

void World::Initialize()
{
	timer.Start();
	ZeroMemory(&Key, sizeof(Key));
	ZeroMemory(&MouseKey, sizeof(MouseKey));
}

void World::ResetTime()
{
	timer.Start();
	elapsed = time = 0;
}

DWORD World::GetCurrentTime()
{
	return time;
}

DWORD World::GetElapsedTime()
{
	return elapsed;
}

void World::ElapseTime()
{
	timer.Stop();
	tick ++;
	elapsed = timer.GetMili();
	if (elapsed > 100.0f)
		elapsed = 100.0f;
	sec = (float)elapsed / 1000.0f;
	time += elapsed;
	timer.Reset();
}

float World::GetElapsedFloat()
{
	return sec;
}

DWORD World::GetTick()
{
	return tick;
}


void World::KeyDown(unsigned short nKey)
{
	Key[nKey] = true;
	OnSomeKeyDown(nKey);
	OnKeyDown[nKey%256]();
}

void World::KeyUp(unsigned short nKey)
{
	Key[nKey] = false;
	OnSomeKeyUp(nKey);
	OnKeyUp[nKey%256]();
}

void World::MouseKeyUp(char nKey)
{
	MouseKey[nKey] = false;
	OnSomeMouseKeyUp(nKey);
	OnMouseKeyUp[nKey%3]();
}

void World::MouseKeyDown(char nKey)
{
	MouseKey[nKey] = true;
	OnSomeMouseKeyDown(nKey);
	OnMouseKeyDown[nKey%3]();
}

void World::MouseMove(int x, int y)
{
	MouseX = x;
	MouseY = y;
}

bool World::Key[256*256];
int World::MouseX = 0;
int World::MouseY = 0;
bool World::MouseKey[3];


boost::signal<void (short)> World::OnSomeKeyDown;
boost::signal<void (short)> World::OnSomeKeyUp;
boost::signal<void (char)> World::OnSomeMouseKeyDown;
boost::signal<void (char)> World::OnSomeMouseKeyUp;


boost::signal<void ()> World::OnKeyDown[256];
boost::signal<void ()> World::OnKeyUp[256];
boost::signal<void ()> World::OnMouseKeyDown[3];
boost::signal<void ()> World::OnMouseKeyUp[3];

