#pragma once

#include "grng_window_def.h"
#include "grng_gfx.h"
#include "grng_deferred_buffer.h"
#include "grng_camera.h"
#include "grng_scene.h"

#include <chrono>

using namespace std::chrono;


typedef void (*WIN_PROC_DEF)(UINT, WPARAM, void*);
typedef void (*WIN_UPDATE_DEF)(void*);


struct								grng_window : public IGRNG_D3D
{
	int								win_id = -1;
	HWND							hwnd = NULL;
	WIN_PROC_DEF					win_proc = NULL;
	void							*win_proc_data = NULL;

	WIN_UPDATE_DEF					win_update = NULL;
	void							*win_update_data = NULL;

	IDXGISwapChain					*swap_chain = NULL;
	ID3D11Texture2D					*back_buffer_texture = NULL;
	ID3D11RenderTargetView			*back_buffer = NULL;
	GRNG_DEFERRED_BUFFER			deferred_buffer;

	GRNG_CAMERA						*curr_camera = NULL;
	GRNG_SCENE						*curr_scene = NULL;

	milliseconds					begin_frame;


	grng_window() : IGRNG_D3D(){ }
	grng_window(const grng_window &window) : IGRNG_D3D()
	{
		this->hwnd = window.hwnd;
		this->win_proc = window.win_proc;
		this->win_proc_data = window.win_proc_data;

		this->swap_chain = window.swap_chain;
		this->back_buffer_texture = window.back_buffer_texture;
		this->back_buffer = window.back_buffer;
		this->deferred_buffer = window.deferred_buffer;
	}

	~grng_window()
	{
		this->destroy();
	}


	void		update()
	{
		this->begin_frame = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		GRNG_GFX::set_win(this->win_id);

		if (this->curr_camera)
			GRNG_GFX::set_curr_camera(this->curr_camera);

		if (this->win_update)
			this->win_update(this->win_update_data);
	}

	void		draw_g_pass()
	{
		if (!this->curr_camera || !this->curr_scene)
			return;

		this->deferred_buffer.bind_as_rtv();

		D3D11_VIEWPORT &viewport = this->deferred_buffer.get_viewport();
		this->curr_camera->update(viewport.Width, viewport.Height);

		this->curr_scene->draw();
	}

	void		draw_deferred()
	{
		if (!this->curr_camera || !this->curr_scene)
			return;

		this->device_context->OMSetRenderTargets(1, &this->back_buffer, this->deferred_buffer.get_depth_stencil().get_ds_view());
		this->device_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		this->deferred_buffer.bind_as_srv();
		this->curr_scene->bind();
		this->device_context->Draw(4u, 0u);
	}

	void		present()
	{
		this->swap_chain->Present(1, 0);
		GRNG_GFX::set_fps((duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - this->begin_frame).count() / 1000.0f);
	}


	void		create(const LPCWSTR win_name, const DWORD win_style, int x, int y, int w, int h, WIN_PROC_DEF win_proc, void *win_proc_data, WIN_UPDATE_DEF win_update, void *win_update_data, HINSTANCE h_instance)
	{
		HRESULT hr;

		RECT wr = { 0, 0, w, h };
		AdjustWindowRect(&wr, win_style, FALSE);

		HWND hwnd = CreateWindowEx(NULL, GRNG_WIN_CLASS_NAME, win_name, win_style, x, y, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, h_instance, NULL);
		this->hwnd = hwnd;
		this->win_proc = win_proc;
		this->win_proc_data = win_proc_data;

		this->win_update = win_update;
		this->win_update_data = win_update_data;

		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
		scd.BufferCount = 1u;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.SampleDesc.Count = 1u;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.OutputWindow = hwnd;
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		hr = this->idxgi_factory->CreateSwapChain(this->device, &scd, &this->swap_chain);
		hr = this->swap_chain->GetBuffer(0u, __uuidof(ID3D11Texture2D), (LPVOID*)&this->back_buffer_texture);
		hr = this->device->CreateRenderTargetView(this->back_buffer_texture, NULL, &this->back_buffer);

		this->deferred_buffer.set_deferred_buffer((float)w, (float)h);

		this->begin_frame = milliseconds::zero();
	}

	void		destroy()
	{
		this->win_id = -1;
		this->hwnd = NULL;
		this->win_proc = NULL;
		this->win_proc_data = NULL;

		this->win_update = NULL;
		this->win_update_data = NULL;

		if (this->swap_chain)
		{
			this->swap_chain->Release();
			this->swap_chain = NULL;
		}
		if (this->back_buffer_texture)
		{
			this->back_buffer_texture->Release();
			this->back_buffer_texture = NULL;
		}
		if (this->back_buffer)
		{
			this->back_buffer->Release();
			this->back_buffer = NULL;
		}

		this->deferred_buffer.destroy();

		this->curr_camera = NULL;
		this->curr_scene = NULL;
	}


	void		set_camera(GRNG_CAMERA &cam)
	{
		this->curr_camera = &cam;
	}

	void		set_scene(GRNG_SCENE &scene)
	{
		this->curr_scene = &scene;
	}
};

using GRNG_WINDOW = grng_window;
