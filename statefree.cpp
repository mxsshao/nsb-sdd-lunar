#include "statefree.h"
#include "interfacefree.h"

CStateFree CStateFree::MStateFree;

void CStateFree::Load()
{
	CInterfaceFree::MInterfaceFree.Load();

	bg = al_load_bitmap("free/bg.png");
	light = al_load_bitmap("free/light.png");
	sound = al_load_sample("free/sound.ogg");
}

void CStateFree::Init()
{
	CInterfaceFree::MInterfaceFree.Init();

	al_convert_bitmaps();

	count = 254;

	al_play_sample(sound, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
}

void CStateFree::Resize()
{
	if (al_get_display_width(CStateManager::MStateManager.GetDisplay())/float(al_get_display_height(CStateManager::MStateManager.GetDisplay())) > 8.0f/5.0f)
		{
			width = al_get_display_height(CStateManager::MStateManager.GetDisplay())/5.0f*8.0f;
			height = al_get_display_height(CStateManager::MStateManager.GetDisplay());
			x = al_get_display_width(CStateManager::MStateManager.GetDisplay())/2.0f-width/2.0f;
			y = 0;
		}
	else
		{
			width = al_get_display_width(CStateManager::MStateManager.GetDisplay());
			height = al_get_display_width(CStateManager::MStateManager.GetDisplay())/8.0f*5.0f;
			x = 0;
			y = al_get_display_height(CStateManager::MStateManager.GetDisplay())/2.0f-height/2.0f;
		}

	lightHeight = al_get_display_height(CStateManager::MStateManager.GetDisplay());
	lightWidth = lightHeight/300.0f*83.0f;

	CInterfaceFree::MInterfaceFree.Resize();
}

void CStateFree::Cleanup()
{
	CInterfaceFree::MInterfaceFree.Cleanup();

	al_destroy_bitmap(bg);
	al_destroy_bitmap(light);
	al_destroy_sample(sound);
}

void CStateFree::HandleEvents(ALLEGRO_EVENT& ev)
{
	CInterfaceFree::MInterfaceFree.HandleEvents(ev);

	if (ev.type == ALLEGRO_EVENT_TIMER)
	{
		if (count != 0)
		{
			count -= 2;
		}
	}
}

void CStateFree::Render()
{
	al_draw_scaled_bitmap(bg, 0, 0, 1920, 1200, x, y, width, height, 0);

	CInterfaceFree::MInterfaceFree.Render();

	al_draw_scaled_bitmap(light, 0, 0, 332, 1200, 0, 0, lightWidth, lightHeight, 0);

	if (count != 0)
		al_draw_filled_rectangle(0, 0, al_get_display_width(CStateManager::MStateManager.GetDisplay()),
			al_get_display_height(CStateManager::MStateManager.GetDisplay()), al_map_rgba(0, 0, 0, count));
}