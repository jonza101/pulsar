
#include "grng_win_manager.h"


void			grng_wm::add_win_update_secure(unsigned int win_id, GRNG_WIN_UPDATE_DEF win_update, void *win_update_data)
{
	GRNG_WINDOW *win_ptr = grng_wm::win.get_secure(win_id);
	if (!win_ptr)
		return;
	win_ptr->add_update(win_update, win_update_data);
}

void			grng_wm::add_win_update(unsigned int win_id, GRNG_WIN_UPDATE_DEF win_update, void *win_update_data)
{
	GRNG_WINDOW *win_ptr = grng_wm::win.get(win_id);
	win_ptr->add_update(win_update, win_update_data);
}

void			grng_wm::remove_win_update_secure(unsigned int win_id, GRNG_WIN_UPDATE_DEF win_update)
{
	GRNG_WINDOW *win_ptr = grng_wm::win.get_secure(win_id);
	if (!win_ptr)
		return;
	win_ptr->remove_update(win_update);
}

void			grng_wm::remove_win_update(unsigned int win_id, GRNG_WIN_UPDATE_DEF win_update)
{
	GRNG_WINDOW *win_ptr = grng_wm::win.get(win_id);
	win_ptr->remove_update(win_update);
}