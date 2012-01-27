#include "stdafx.h"
#include "MXUtils.h"

using namespace MX;

namespace MX
{

RECT MakeR(int x, int y, int w, int h)
{
	RECT r;
	r.left = x;
	r.top = y;
	r.right = x + w;
	r.bottom = y + h;
	return r;
}

float GetDistance(float x1, float y1, float x2, float y2)
{
	return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}


D3DXMATRIX Utils::Identity;

class __InitUtils
{
public:
	__InitUtils()
	{
		D3DXMatrixIdentity(&Utils::Identity);
	}
}a;



};