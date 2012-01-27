#include "stdafx.h"
#include "MXFont.h"

using namespace MX;

Font::Font(const shared_ptr<MX::Draw> &d) : draw(d)
{
	font = NULL;

}


Font::~Font()
{
	safe_release(font);
}

void Font::LostDevice()
{
	font->OnLostDevice();
}

void Font::ResetDevice()
{
	font->OnResetDevice();
}

shared_ptr<Font> Font::CreateFont(const shared_ptr<MX::Draw> &d, const wchar_t* font_name, int height, bool italic, int weight, int width, int mipmaps, DWORD precision, DWORD quality)
{
	shared_ptr<Font> font(new Font(d));
	if (S_OK == D3DXCreateFont(d->Device(), height, width, weight, mipmaps, italic, DEFAULT_CHARSET, precision, quality, DEFAULT_PITCH | FF_DONTCARE, font_name, &font->font))
	{

		weak_ptr<Font> weak(font);
		font->Connect(d, weak);
		return font;
	}
	return NULL;
}
