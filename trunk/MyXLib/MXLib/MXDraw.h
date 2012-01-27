#ifndef MXLIB_DRAW
#define MXLIB_DRAW

#include <windows.h>
#include <d3d9.h>
#include <memory>
#include "MXUtils.h"
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include "MXWindow.h"

namespace MX
{
	class _DirectXData
	{
	public:
		_DirectXData();
		~_DirectXData();

		bool Initialize();

		LPDIRECT3D9         D3D; 
		LPDIRECT3DDEVICE9   Device;
	};

	class Draw : public std::enable_shared_from_this<Draw> 
	{
	public:
		Draw();
		virtual ~Draw();

		bool Initialize();
		void Deinitialize();
		bool IsInitialized() { return init_counter > 0; }

		void ResetRenderTarget();

		void Clear(D3DCOLOR color, D3DRECT *rect = NULL);

		bool Begin();
		void End();
		void Present();

		bool ChangeView(int w, int h, bool fullscreen);
		inline LPDIRECT3DDEVICE9 &Device() { return helperdata.Device; }

		boost::signal<void ()> OnReset; //after resetting device
		boost::signal<void ()> OnLost; //if device is lost

		D3DXMATRIX& GetView() { return ViewMatrix; }

		void Reset(int tries = 1);
		bool IsDeviceReset();

		int width;
		int height;
		D3DXMATRIX Ortho2D;	
		D3DXMATRIX ViewMatrix;
	protected:
		
		virtual void AfterReset();

		virtual D3DPRESENT_PARAMETERS DeviceParameters(int w, int h, bool fullscreen);
		unsigned init_counter;
		D3DPRESENT_PARAMETERS params;
		_DirectXData helperdata;
		std::unique_ptr<Window> window;
		bool is_device_reset;

		LPDIRECT3DSURFACE9 render_target;
		
		
	};

	template <class T>
	class AutoReset
	{
	protected:
		void Connect(const std::shared_ptr<MX::Draw> &d, const std::weak_ptr<T> &ptr)
		{
			LostSignal = d->OnLost.connect([ptr](){ 
						auto p = ptr.lock(); 
						if (p) p->LostDevice(); 
					});
			ResetSignal = d->OnReset.connect([ptr](){ 
						auto p = ptr.lock(); 
						if (p) p->ResetDevice(); 
					});
		}
		boost::signals::scoped_connection LostSignal;
		boost::signals::scoped_connection ResetSignal;
	};


};

#endif