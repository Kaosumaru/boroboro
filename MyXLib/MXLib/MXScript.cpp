#include "stdafx.h"
#include "MXScript.h"

using namespace MX;

shared_ptr<Command> MX::wait(unsigned time)
{
	return shared_ptr<Command>(new WaitCommand(time));
}

shared_ptr<Command> MX::move_to(float x, float y, float speed)
{
	return shared_ptr<Command>(new MoveToCommand(x,y,speed));
}

shared_ptr<TeleportToCommand> MX::teleport_to(float x, float y)
{
	return shared_ptr<TeleportToCommand>(new TeleportToCommand(x,y));
}

shared_ptr<Command> MX::change_color(D3DXCOLOR c)
{
	return shared_ptr<Command>(new ChangeColorCommand(c));
}

shared_ptr<Command> MX::die()
{
	return shared_ptr<Command>(new DieCommand());
}

shared_ptr<Command> MX::lerp_color(D3DXCOLOR c, unsigned time)
{
	return shared_ptr<Command>(new LerpColorCommand(c, time));
}

shared_ptr<Command> MX::warp_scale(float scaleX, float scaleY, unsigned mili)
{
	return shared_ptr<Command>(new WarpScaleCommand(scaleX, scaleY, mili));
}

shared_ptr<Command> MX::To_Command(const shared_ptr<Command> &t1)
{
	return t1;
}