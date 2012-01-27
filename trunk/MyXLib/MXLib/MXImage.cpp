#include "stdafx.h"
#include "MXImage.h"
#include <d3d9.h>
#include <D3dx9tex.h>
using namespace MX;


Image::Image()
{
	W = H = 0;
	texture = NULL;
}

Image::~Image()
{
	Deinitialize();
}

bool Image::Load(Draw &draw, const wchar_t *path, D3DCOLOR ColorKey)
{
	Deinitialize();
	D3DXIMAGE_INFO info;
	W = H = 0;
	ZeroMemory(&info,sizeof(D3DXIMAGE_INFO));
	HRESULT  hr = D3DXCreateTextureFromFileEx(draw.Device(), 
			path, 
			D3DX_DEFAULT_NONPOW2, 
			D3DX_DEFAULT_NONPOW2, 
			0, 
			0, 
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			ColorKey,
			&info,
			NULL,
			&texture);

	if (hr == D3D_OK)
	{
		W = info.Width;
		H = info.Height;
		return true;
	}

	texture = NULL;
	return false;
}

bool Image::IsImageLoaded()
{
	return texture != NULL;
}

void Image::Deinitialize()
{
	safe_release(texture);
}



/*
shared_ptr<RenderTarget> RenderTarget::Create(Draw &draw, int w, int h)
{
	shared_ptr<RenderTarget> render(new RenderTarget);
	if (draw.Device()->CreateTexture(w,h,1,  D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &(render->texture) , NULL) == D3D_OK)
		return  render;
	return NULL;
}

void RenderTarget::Begin(Draw &draw)
{
	LPDIRECT3DSURFACE9 render_target = NULL;
	texture->GetSurfaceLevel(0, &render_target); 
	draw.Device()->SetRenderTarget(0, render_target);
	render_target->Release();
}

void RenderTarget::End(Draw &draw)
{
	draw.ResetRenderTarget();
}
*/

Frame::Frame()
{
	src.left = src.bottom = src.right = src.top = 0;
}

Frame::Frame(const shared_ptr<Image> &im) : image(im)
{
	if (image)
	{
		src.left = src.top = 0;
		src.right = image->Width();
		src.bottom = image->Height();
	}
	else
		src.left = src.bottom = src.right = src.top = 0;
}

Frame::Frame(const shared_ptr<Image> &im, int x, int y, int w, int h) : image(im)
{
	src.left = x;
	src.top = y;
	src.right = src.left + w;
	src.bottom = src.top + h;
}

Frame::Frame(const shared_ptr<Image> &im, RECT s) : image(im), src(s)
{
}


shared_ptr<Image> ImageFolder::LoadImage(const wchar_t *strPath)
{
	shared_ptr<Image> img(new Image);
	if (!img->Load(*draw, strPath))
		img = NULL;
	return img;
}


