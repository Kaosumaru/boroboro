#include "stdafx.h"
#include "MXSpriter.h"
#include "MXUtils.h"

using namespace MX;

Spriter::Spriter(const shared_ptr<MX::Draw> &d) : draw(d) 
{
	sprite = NULL;
}

bool Spriter::Init()
{
	if (!draw)
		return false;
	if (D3DXCreateSprite(draw->Device(), &sprite) != S_OK)
		return false;

	weak_ptr<Spriter> weak(shared_from_this());
	Connect(draw, weak);

	//sprite->SetWorldViewRH(NULL, &draw->GetView());
	sprite->SetWorldViewRH(NULL, NULL);

	return sprite != NULL;
}

Spriter::~Spriter()
{
	safe_release(sprite);
}


bool Spriter::Begin(DWORD flags)
{
	if (sprite == NULL)
		return false;
	return sprite->Begin(flags) == S_OK;
}

void Spriter::Draw(Image &image, float x, float y, float z, float cx, float cy, float rotation, float scaleX, float scaleY, D3DCOLOR color, RECT *src, D3DXMATRIX *custom_transform)
{

	D3DXMATRIX matrix, m2;
	D3DXVECTOR2 center(x, y);
	D3DXMatrixTransformation2D(&matrix, &center, 0.0f, &D3DXVECTOR2(scaleX, scaleY), &center, rotation, &D3DXVECTOR2(0.0f, 0.0f));


	if (custom_transform)
	{
		D3DXMatrixMultiply(&m2, &matrix, custom_transform);
		SetTransform(&m2);
	}
	else
		SetTransform(&matrix);
		
	
	Draw(image, D3DXVECTOR3(x-cx,y-cy,z), D3DXVECTOR3(0.0f,0.0f,0.0f), color, src);
}

void Spriter::DrawCenter(Image &image, float x, float y, float z, float rotation, float scaleX, float scaleY, D3DCOLOR color, RECT *src)
{
	Draw(image, x, y, z, (float)image.Width()/2, (float)image.Height()/2, rotation, scaleX, scaleY, color, src);
}

void Spriter::Draw(Image &image, const D3DXVECTOR3 &position, const D3DXVECTOR3 &center, D3DCOLOR color, RECT *src)
{
	sprite->Draw(image.GetTexture(), src, &center, &position, color);
}


void Spriter::Draw(Frame &frame, float x, float y, float z, float cx, float cy, float rotation, float scaleX, float scaleY, D3DCOLOR color, D3DXMATRIX *custom_transform)
{
	Draw(*frame.image, x, y, z, cx, cy, rotation, scaleX, scaleY, color, &frame.src, custom_transform);
}

void Spriter::DrawCenter(Frame &frame, float x, float y, float z, float rotation, float scaleX, float scaleY, D3DCOLOR color)
{
	Draw(*frame.image, x, y, z, x+(float)frame.Width()/2, y+(float)frame.Height()/2, rotation, scaleX, scaleY, color, &frame.src);
}

void Spriter::Draw(Frame &frame, const D3DXVECTOR3 &position, const D3DXVECTOR3 &center, D3DCOLOR color)
{
	Draw(*frame.image, position, center, color, &frame.src);
}


void Spriter::DrawText(Font &font, const wchar_t *text, LPRECT rect, D3DCOLOR color, DWORD format, int count)
{
	int r = font.font->DrawText(sprite,	text, count, rect, format, color);
	r ++;
}

void Spriter::SetTransform(const D3DXMATRIX *pTransform)
{
	sprite->SetTransform(pTransform);
}

void Spriter::End()
{
	sprite->End();
}

void Spriter::ResetTransform()
{
	sprite->SetTransform(&Utils::Identity);
}

void Spriter::LostDevice()
{
	if (sprite)
	{
		sprite->OnLostDevice();
	}
}

void Spriter::ResetDevice()
{
	if (sprite)
		sprite->OnResetDevice();
}
