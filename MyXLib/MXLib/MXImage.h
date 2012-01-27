#ifndef MXLIB_IMAGE
#define MXLIB_IMAGE

#include <memory>
#include <list>
#include "MXUtils.h"
#include "MXDraw.h"

namespace MX
{

class Image
{
public:
	Image();
	~Image();

	bool Load(Draw &draw, const wchar_t *path, D3DCOLOR ColorKey = 0);
	bool IsImageLoaded();
	void Deinitialize();

	unsigned Width() { return W; }
	unsigned Height() { return H; }

	LPDIRECT3DTEXTURE9 GetTexture() { return texture; }
protected:
	unsigned W, H;
	LPDIRECT3DTEXTURE9 texture;
};
/*
class RenderTarget : public Image
{
public:
	static shared_ptr<RenderTarget> Create(Draw &draw, int w, int h);
	void Begin(Draw &draw);
	void End(Draw &draw);
protected:
	LPDIRECT3DTEXTURE9 texture;
};
*/
class Frame
{
public:
	Frame();
	Frame(const std::shared_ptr<Image> &im);
	Frame(const std::shared_ptr<Image> &im, int x, int y, int w, int h);
	Frame(const std::shared_ptr<Image> &im, RECT s);

	int Width() { return src.right - src.left; }
	int Height() { return src.bottom - src.top; }

	std::shared_ptr<Image> image;
	RECT src;
};


class ImageFolder : public DataFolder<Image>
{
public:
	ImageFolder(const std::shared_ptr<Draw> &d) : draw(d) {  }
	~ImageFolder()
	{
		UnloadImages();
	}

	std::shared_ptr<Image> LoadImage(const wchar_t *strPath);
	void UnloadImages() { Deinitialize(); }

protected:
	shared_ptr<Draw> draw;
	list<std::shared_ptr<Image>> images;

};
}

#endif
