#include "displayPFD.h"

CDisplayPFD CDisplayPFD::MDisplayPFD;

void CDisplayPFD::Load()
{
	font = al_load_font("font/LiquidCrystal.ttf", 24, 0);
	bg = al_load_bitmap("pfd/bg.png");
	back = al_load_bitmap("pfd/back.png");
	attitude = al_load_bitmap("pfd/attitude.png");
	altitude = al_load_bitmap("pfd/altitude.png");
	speed = al_load_bitmap("pfd/speed.png");
	border = al_load_bitmap("pfd/border.png");
	off = al_load_bitmap("pfd/off.png");

	overspeed = al_load_bitmap("pfd/overspeed.png");	
}

void CDisplayPFD::Init()
{
	al_convert_bitmaps();
	render = al_create_bitmap(580, 580);
}

void CDisplayPFD::Resize()
{
	width = al_get_display_width(CStateManager::MStateManager.GetDisplay())/140.0f*29.0f;
	y = al_get_display_height(CStateManager::MStateManager.GetDisplay()) - width;
}

void CDisplayPFD::Cleanup()
{
	al_destroy_font(font);
	al_destroy_bitmap(bg);
	al_destroy_bitmap(back);
	al_destroy_bitmap(attitude);
	al_destroy_bitmap(altitude);
	al_destroy_bitmap(speed);
	al_destroy_bitmap(border);
	al_destroy_bitmap(off);

	al_destroy_bitmap(overspeed);
	al_destroy_bitmap(render);
}

void CDisplayPFD::Render()
{
	al_set_target_bitmap(render);

	al_clear_to_color(al_map_rgba(0,0,0,0));

	if (CStateSimulator::MStateSimulator.GetAvionics())
	{
		al_draw_rotated_bitmap(attitude, 250, 250, 280, 330, (360 - CStateSimulator::MStateSimulator.GetAttitude()) * ALLEGRO_PI / 180, 0);
		al_draw_bitmap(back, 0, 0, 0);
		al_draw_bitmap(speed, 45, -930 + (CStateSimulator::MStateSimulator.GetSpeed() * 40), 0);
		al_draw_bitmap(altitude, 425, -1970 + (CStateSimulator::MStateSimulator.GetAltitude() * 0.1f), 0);
		al_draw_bitmap(bg, 0, 0, 0);

		if (altitude >= 0)
			al_draw_textf(font, al_map_rgb(255, 255, 255), 520, 320, ALLEGRO_ALIGN_RIGHT, "%i", int(CStateSimulator::MStateSimulator.GetAltitude()));
		else
			al_draw_textf(font, al_map_rgb(255, 255, 0), 520, 320, ALLEGRO_ALIGN_RIGHT, "%i", abs(int(CStateSimulator::MStateSimulator.GetAltitude())));

		if (CStateSimulator::MStateSimulator.GetOverspeed())
			al_draw_bitmap(overspeed, 0, 0, 0);
	}
	else
	{
		al_draw_bitmap(off, 0, 0, 0);
	}
	al_draw_bitmap(border, 0, 0, 0);
	

	al_set_target_bitmap(al_get_backbuffer(CStateManager::MStateManager.GetDisplay()));
	al_draw_scaled_bitmap(render, 0, 0, 580, 580, 0, y, width, width, 0);
}