#ifndef MXLIB_SPRITER
#define MXLIB_SPRITER

#include <windows.h>
#include <d3d9.h>
#include <D3dx9core.h>
#include <memory>
#include "MXDraw.h"
#include "MXImage.h"
#include "MXFont.h"

namespace MX
{
	using namespace std;
	class Spriter : public enable_shared_from_this<Spriter>, public AutoReset<Spriter>
	{
	public:
		Spriter(const shared_ptr<Draw> &d);
		~Spriter();

		bool Init();

		bool IsOK() { return sprite != NULL; }

		bool Begin(DWORD flags = 0);
		void End();
		void ResetTransform();

		void Draw(Image &image, float x, float y, float z = 0.0f, float cx = 0.0f, float cy = 0.0f, float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f, D3DCOLOR color = 0xFFFFFFFF, RECT *src = NULL);
		void DrawCenter(Image &image, float x, float y, float z = 0.0f, float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f, D3DCOLOR color = 0xFFFFFFFF, RECT *src = NULL);
		void Draw(Image &image, const D3DXVECTOR3 &position, const D3DXVECTOR3 &center = D3DXVECTOR3(0.0f,0.0f,0.0f), D3DCOLOR color = 0xFFFFFFFF, RECT *src = NULL);

		void Draw(Frame &frame, float x, float y, float z = 0.0f, float cx = 0.0f, float cy = 0.0f, float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f, D3DCOLOR color = 0xFFFFFFFF);
		void DrawCenter(Frame &frame, float x, float y, float z = 0.0f, float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f, D3DCOLOR color = 0xFFFFFFFF);
		void Draw(Frame &frame, const D3DXVECTOR3 &position, const D3DXVECTOR3 &center = D3DXVECTOR3(0.0f,0.0f,0.0f), D3DCOLOR color = 0xFFFFFFFF);



		void DrawText(Font &font, const wchar_t *text, LPRECT rect, D3DCOLOR color = 0xFF000000, DWORD format = DT_LEFT, int count = -1);

		void SetTransform(const D3DXMATRIX *pTransform = NULL);



		shared_ptr<MX::Draw>& GetDraw() { return draw; }

	protected:
		void LostDevice();
		void ResetDevice();
		friend AutoReset<Spriter>;


		LPD3DXSPRITE sprite;
		shared_ptr<MX::Draw> draw;
	};


};


#endif