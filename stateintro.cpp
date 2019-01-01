#include "stateintro.h"
#include "statemenu.h"
#include "statesimulator.h"

CStateIntro CStateIntro::MStateIntro;

void CStateIntro::Load()
{
	allegro = al_load_bitmap("intro/allegro.png");
	gwen = al_load_bitmap("intro/gwen.png");
	m = al_load_bitmap("intro/m.png");
	sound = al_load_sample("intro/sound.ogg");
	soundInstance = al_create_sample_instance(sound);
	al_set_sample_instance_playmode(soundInstance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(soundInstance, al_get_default_mixer());
}

void CStateIntro::Init()
{
	al_convert_bitmaps();

	fade = false; //Fade In
	count = 254;
	image = ALLEGRO;

	al_hide_mouse_cursor(CStateManager::MStateManager.GetDisplay());

	al_play_sample_instance(soundInstance);
}

void CStateIntro::Resize()
{
	if (al_get_display_width(CStateManager::MStateManager.GetDisplay())/float(al_get_display_height(CStateManager::MStateManager.GetDisplay())) > 8.0f/5.0f) //Height Limit
		{
			width = al_get_display_width(CStateManager::MStateManager.GetDisplay());
			height = al_get_display_width(CStateManager::MStateManager.GetDisplay())/8.0f*5.0f;
			x = 0;
			y = al_get_display_height(CStateManager::MStateManager.GetDisplay())/2.0f-height/2.0f;
		}
	else
		{
			width = al_get_display_height(CStateManager::MStateManager.GetDisplay())/5.0f*8.0f;
			height = al_get_display_height(CStateManager::MStateManager.GetDisplay());
			x = al_get_display_width(CStateManager::MStateManager.GetDisplay())/2.0f-width/2.0f;
			y = 0;
		}
}

void CStateIntro::Cleanup()
{
	al_show_mouse_cursor(CStateManager::MStateManager.GetDisplay());

	al_destroy_bitmap(allegro);
	al_destroy_bitmap(gwen);
	al_destroy_bitmap(m);
	al_destroy_sample_instance(soundInstance);
	al_destroy_sample(sound);
}

void CStateIntro::HandleEvents(ALLEGRO_EVENT& ev)
{
	if (ev.type == ALLEGRO_EVENT_TIMER)
	{
		if (fade)
		{
			if (count != 254)
				count += 2;
			else
			{
				fade = false;
				if (image != M)
					image++;
				else
				{
					if (al_get_sample_instance_playing(soundInstance))
						al_stop_sample_instance(soundInstance);
					CStateManager::MStateManager.ChangeState(&CStateMenu::MStateMenu);
				}
			}
		}
		else
		{
			if (count != 0)
				count -= 2;
			else
			{
				fade = true;
			}
		}
	}
	else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE || ev.keyboard.keycode == ALLEGRO_KEY_ENTER || ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			CStateManager::MStateManager.ChangeState(&CStateMenu::MStateMenu);
	}
	else if (ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY)
	{
		al_hide_mouse_cursor(CStateManager::MStateManager.GetDisplay());
	}
}

void CStateIntro::Render()
{
	if (image == ALLEGRO)
		al_draw_scaled_bitmap(allegro, 0, 0, 1920, 1200, x, y, width, height, 0);
	else if (image == GWEN)
		al_draw_scaled_bitmap(gwen, 0, 0, 1920, 1200, x, y, width, height, 0);
	else
		al_draw_scaled_bitmap(m, 0, 0, 1920, 1200, x, y, width, height, 0);
		
	al_draw_filled_rectangle(0, 0, al_get_display_width(CStateManager::MStateManager.GetDisplay()),
			al_get_display_height(CStateManager::MStateManager.GetDisplay()), al_map_rgba(0, 0, 0, count));
}