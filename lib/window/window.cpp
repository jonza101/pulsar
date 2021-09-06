
#include "window.h"


void		PULSAR::WINDOW::create(const LPCTSTR win_name, const DWORD win_style, int x, int y, int w, int h, PULSAR::WIN_PROC_DEF win_proc, void *win_proc_data, HINSTANCE h_instance)
{
	HRESULT hr;

	RECT wr = { 0, 0, w, h };
	AdjustWindowRect(&wr, win_style, FALSE);

	HWND hwnd = CreateWindowEx(NULL, WIN_CLASS_NAME, win_name, win_style, x, y, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, h_instance, NULL);
	if (!hwnd)
		THROW_WIN_LAST_EXC();

	this->hwnd = hwnd;
	this->win_proc = win_proc;
	this->win_proc_data = win_proc_data;

	this->render_target.set(this->hwnd, TRUE);
	this->deferred_buffer.set_deferred_buffer((float)w, (float)h);
	this->skybox_ds_state.set(TRUE, D3D11_COMPARISON_LESS_EQUAL, D3D11_DEPTH_WRITE_MASK_ZERO);


	//this->rg = rg::create(&this->render_target, &this->deferred_buffer.get_ds_view());
}

void		PULSAR::WINDOW::destroy()
{
	this->win_id = -1;
	this->hwnd = NULL;
	this->win_proc = NULL;
	this->win_proc_data = NULL;

	this->win_update.clear();

	this->curr_camera = NULL;
	this->curr_scene = NULL;
}