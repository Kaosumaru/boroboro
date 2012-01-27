#ifndef MXLIB_LINE
#define MXLIB_LINE

#include <list>
#include <D3dx9core.h>
#include "MXUtils.h"
#include "MXDraw.h"

namespace MX
{
	class Liner : public AutoReset<Liner>
	{
	protected:
		Liner(const shared_ptr<Draw> &d) : draw(d) {  }
	public:
		~Liner()
		{
		}

		static shared_ptr<Liner> Create(const shared_ptr<MX::Draw> &d);
		bool Begin();
		void End();

		void Draw(float x1, float y1, float x2, float y2, D3DCOLOR color);
		void Draw(float x1, float y1, float z1, float x2, float y2, float z2, D3DCOLOR color);
		void Draw(const D3DXVECTOR2 *points, DWORD count, D3DCOLOR color);
		void DrawTransform(const D3DXVECTOR3 *points, DWORD count, const D3DXMATRIX *transform, D3DCOLOR color);
	
		void SetAntialias(bool anti);
		bool GetAntialias();
	
		void SetWidth(float width);
		float GetWidth();

		void SetPattern(DWORD pattern);
		DWORD GetPattern();

		void SetPatternScale(float width);
		float GetPatternScale();

		void SetGLLines(bool bGL);

	protected:
		void LostDevice();
		void ResetDevice();
		friend AutoReset<Liner>;

		LPD3DXLINE line;
		shared_ptr<MX::Draw> draw;

	};

}

#endif
