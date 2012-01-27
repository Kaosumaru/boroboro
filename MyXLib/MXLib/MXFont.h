#ifndef MXLIB_FONT
#define MXLIB_FONT

#include <list>
#include "MXUtils.h"
#include "MXDraw.h"
#include <D3dx9core.h>

namespace MX
{
class Spriter;




class Font : public std::enable_shared_from_this<Font>, public AutoReset<Font>
{
protected:
	Font(const std::shared_ptr<MX::Draw> &d);
public:
	~Font();
	static shared_ptr<Font> CreateFont(const std::shared_ptr<MX::Draw> &d, const wchar_t* font_name, int height, bool italic = false, int weight = FW_MEDIUM, int width = 0, int mipmaps = 1, DWORD precision = OUT_TT_ONLY_PRECIS, DWORD quality = DEFAULT_QUALITY);
	
protected:
	void LostDevice();
	void ResetDevice();
	friend AutoReset<Font>;

	shared_ptr<MX::Draw> draw;

	LPD3DXFONT font;
	friend Spriter;
};

}

#endif
