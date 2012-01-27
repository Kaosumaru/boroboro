#include "stdafx.h"
#include "MXDraw.h"
#include "MXUtils.h"
#include <D3dx9math.h>

using namespace MX;

_DirectXData::_DirectXData()
{
	Device = NULL;
	D3D = NULL;
}

_DirectXData::~_DirectXData()
{
	safe_release(D3D);
	safe_release(Device);
}

bool _DirectXData::Initialize()
{
	if (!D3D)
		D3D = Direct3DCreate9( D3D_SDK_VERSION );
	if (!D3D)
		return false;
	return true;
}

Draw::Draw()
{
	render_target = NULL;
	width = 0;
	height = 0;
	init_counter = 0;
	is_device_reset = false;
}

Draw::~Draw()
{
	init_counter = 0;
	Deinitialize();
}

bool Draw::Initialize()
{
	if (init_counter != 0)
		return false;
	if (!helperdata.Initialize())
		return false;
	init_counter ++;
	return true;
}

void Draw::Deinitialize()
{
	if (init_counter == 0)
		return;
	init_counter --;
}

void Draw::ResetRenderTarget()
{
	//AfterReset();
	//Device()->SetRenderTarget(0, render_target);
}

void Draw::Clear(D3DCOLOR color, D3DRECT *rect)
{
	Device()->Clear(0L, rect, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
}

bool Draw::Begin()
{
	return SUCCEEDED(Device()->BeginScene());
}

void Draw::End()
{
	 Device()->EndScene();
}

void Draw::Present()
{
	HRESULT hr = Device()->Present(NULL, NULL, NULL, NULL);
	if (hr == D3DERR_DEVICELOST)
	{
		is_device_reset = false;
		Reset();
	}
}

bool Draw::IsDeviceReset()
{
	return is_device_reset;
}

void Draw::Reset(int tries)
{
	if (!Device())
		return;

	HRESULT hr = Device()->TestCooperativeLevel();
	if (hr == D3D_OK)
	{
		AfterReset();
		return;
	}
	is_device_reset = false;
	OnLost();
	
	while(tries > 0)
	{
		if (tries != 0xFFFFFFFF)
			tries --;
		Sleep(10);
		switch(hr)
		{
		case D3D_OK:
			AfterReset();
			OnReset();
			return;
		case D3DERR_DEVICELOST:
			is_device_reset = false;
			break;
		case D3DERR_DEVICENOTRESET:
			Device()->Reset(&params);		
			break;
		}

		hr = Device()->TestCooperativeLevel();
	}

}

void Draw::AfterReset()
{
	is_device_reset = true;
	D3DXMATRIX Identity;

	D3DXMatrixLookAtRH(&ViewMatrix, &D3DXVECTOR3((float)width/2.0f, (float)height/2.0f, 1.0f), &D3DXVECTOR3((float)width/2.0f, (float)height/2.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));


	
	D3DXMatrixOrthoRH(&Ortho2D, (float)width, (float)height, 0.0f, 1.0f);
	D3DXMatrixIdentity(&Identity);

	Device()->SetTransform(D3DTS_PROJECTION, &Ortho2D);
	Device()->SetTransform(D3DTS_WORLD, &Identity);

	Device()->SetTransform(D3DTS_VIEW, &Identity);

	Device()->SetRenderState( D3DRS_ZENABLE, TRUE );
	Device()->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);
	Device()->SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );
	



    Device()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    Device()->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    Device()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
    Device()->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    Device()->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	Device()->ShowCursor(false);
}

bool Draw::ChangeView(int w, int h, bool fullscreen)
{
	if (!IsInitialized())
		return false;
	window.reset(new Window(L"Dummy", fullscreen ? 400 : w, fullscreen ? 400 : h));
	if (!window)
		return false;



	    // Create the D3DDevice
	params = DeviceParameters(w, h, fullscreen);
	if( FAILED( helperdata.D3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window->GetHWND(),
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &params, &Device() ) ) )
        return false;

	if (fullscreen)
	{
		ShowCursor(false);
	}
	width = w;
	height = h;
	//Device()->GetRenderTarget(0, &render_target);
	AfterReset();
	return true;
}

D3DPRESENT_PARAMETERS Draw::DeviceParameters(int w, int h, bool fullscreen)
{
	D3DDISPLAYMODE d3ddm;
	helperdata.D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = fullscreen ? FALSE : TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferWidth = w;
	d3dpp.BackBufferHeight = h;
	d3dpp.BackBufferFormat =  d3ddm.Format;

	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;


	return d3dpp;
}
