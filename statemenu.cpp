#include "statemenu.h"
#include "interfacemenu.h"

CStateMenu CStateMenu::MStateMenu;

void CStateMenu::Load()
{
	CInterfaceMenu::MInterfaceMenu.Load();

	bg = al_load_bitmap("menu/bg.png");
	sound = al_load_sample("menu/theme.ogg");
}

void CStateMenu::Init()
{
	CInterfaceMenu::MInterfaceMenu.Init();

	al_convert_bitmaps();

	count = 254;

	al_play_sample(sound, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
}

void CStateMenu::Resize()
{
	CInterfaceMenu::MInterfaceMenu.Resize();

	if (al_get_display_width(CStateManager::MStateManager.GetDisplay())/float(al_get_display_height(CStateManager::MStateManager.GetDisplay())) > 16.0f/9.0f)
		{
			width = al_get_display_width(CStateManager::MStateManager.GetDisplay());
			height = al_get_display_width(CStateManager::MStateManager.GetDisplay())/16.0f*9.0f;
			x = 0;
			y = al_get_display_height(CStateManager::MStateManager.GetDisplay())/2.0f-height/2.0f;
		}
	else
		{
			width = al_get_display_height(CStateManager::MStateManager.GetDisplay())/9.0f*16.0f;
			height = al_get_display_height(CStateManager::MStateManager.GetDisplay());
			x = al_get_display_width(CStateManager::MStateManager.GetDisplay())/2.0f-width/2.0f;
			y = 0;
		}
}

void CStateMenu::Cleanup()
{
	CInterfaceMenu::MInterfaceMenu.Cleanup();

	al_destroy_bitmap(bg);
	al_destroy_sample(sound);
}

void CStateMenu::HandleEvents(ALLEGRO_EVENT& ev)
{
	CInterfaceMenu::MInterfaceMenu.HandleEvents(ev);

	if (ev.type == ALLEGRO_EVENT_TIMER)
	{
		if (count != 0)
		{
			count -= 2;
		}
	}
	else if (ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY)
	{
		al_show_mouse_cursor(CStateManager::MStateManager.GetDisplay());
	}
}

void CStateMenu::Render()
{
	al_draw_scaled_bitmap(bg, 0, 0, 1920, 1080, x, y, width, height, 0);

	CInterfaceMenu::MInterfaceMenu.Render();

	if (count != 0)
		al_draw_filled_rectangle(0, 0, al_get_display_width(CStateManager::MStateManager.GetDisplay()),
			al_get_display_height(CStateManager::MStateManager.GetDisplay()), al_map_rgba(0, 0, 0, count));
}