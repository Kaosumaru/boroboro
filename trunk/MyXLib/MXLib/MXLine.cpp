#include "stdafx.h"
#include "MXDraw.h"
#include "MXLine.h"
#include <D3dx9math.h>

using namespace MX;

shared_ptr<Liner> Liner::Create(const shared_ptr<MX::Draw> &d)
{
	shared_ptr<Liner> liner(new Liner(d));
	if (S_OK == D3DXCreateLine(d->Device(), &liner->line))
	{
		weak_ptr<Liner> weak(liner);
		liner->Connect(d, weak);
		return liner;
	}
	return NULL;
}

bool Liner::Begin()
{
	return line->Begin() == D3D_OK;
}

void Liner::End()
{
	line->End();
}

void Liner::Draw(float x1, float y1, float x2, float y2, D3DCOLOR color)
{
	D3DXVECTOR2 p[2];
	p[0].x = x1;
	p[0].y = y1;
	p[1].x = x2;
	p[1].y = y2;

	line->Draw(&p[0], 2, color);
}


void Liner::Draw(float x1, float y1, float z1, float x2, float y2, float z2, D3DCOLOR color)
{

	D3DXVECTOR3 p[2];
	p[0].x = x1;
	p[0].y = y1;
	p[0].z = z1;
	
	p[1].x = x2;
	p[1].y = y2;
	p[1].z = z2;

	D3DXMATRIX worldTrans, view, proj;

	draw->Device()->GetTransform( D3DTS_WORLD, &worldTrans );
	draw->Device()->GetTransform( D3DTS_VIEW, &view );
	draw->Device()->GetTransform( D3DTS_PROJECTION, &proj );


	D3DXMATRIX m = worldTrans * view * proj;
	line->DrawTransform(&p[0], 2, &m, color);
}

void Liner::Draw(const D3DXVECTOR2 *points, DWORD count, D3DCOLOR color)
{
	line->Draw(points, count, color);
}

void Liner::DrawTransform(const D3DXVECTOR3 *points, DWORD count, const D3DXMATRIX *transform, D3DCOLOR color)
{
	line->DrawTransform(points, count, transform, color);
}

void Liner::SetAntialias(bool anti)
{
	line->SetAntialias(anti ? 1 : 0);
}

bool Liner::GetAntialias()
{
	return line->GetAntialias() != 0;
}

void Liner::SetWidth(float width)
{
	line->SetWidth(width);
}

float Liner::GetWidth()
{
	return line->GetWidth();
}

void Liner::SetPattern(DWORD pattern)
{
	line->SetPattern(pattern);
}

DWORD Liner::GetPattern()
{
	return line->GetPattern();
}

void Liner::SetPatternScale(float width)
{
	line->SetPatternScale(width);
}

float Liner::GetPatternScale()
{
	return line->GetPatternScale();
}

void Liner::SetGLLines(bool bGL)
{
	line->SetGLLines(bGL);
}


void Liner::LostDevice()
{
	if (line)
		line->OnLostDevice();
}

void Liner::ResetDevice()
{
	if (line)
		line->OnResetDevice();
}

