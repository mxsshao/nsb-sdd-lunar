#include "displayECAM.h"
#include "displayPFD.h"

CDisplayECAM CDisplayECAM::MDisplayECAM;

void CDisplayECAM::Load()
{
	font = al_load_font("font/LiquidCrystal.ttf", 24, 0);
	border = al_load_bitmap("ecam/border.png");
	off = al_load_bitmap("ecam/off.png");

	fuelBG = al_load_bitmap("ecam/fuelbg.png");
	fuelBack = al_load_bitmap("ecam/fuelback.png");

	fuelLow = al_load_bitmap("ecam/fuellow.png");
	fuelEmpty = al_load_bitmap("ecam/fuelempty.png");
}

void CDisplayECAM::Init()
{
	al_convert_bitmaps();

	render = al_create_bitmap(800, 336);
}

void CDisplayECAM::Resize()
{
	width = al_get_display_width(CStateManager::MStateManager.GetDisplay())/7.0f*2.0f;
	height = width/50.0f*21.0f;
	x = CDisplayPFD::MDisplayPFD.GetWidth();
	y = al_get_display_height(CStateManager::MStateManager.GetDisplay()) - height;
}

void CDisplayECAM::Cleanup()
{
	al_destroy_font(font);
	al_destroy_bitmap(border);
	al_destroy_bitmap(off);
	al_destroy_bitmap(fuelBG);
	al_destroy_bitmap(fuelBack);
	al_destroy_bitmap(fuelLow);
	al_destroy_bitmap(fuelEmpty);

	al_destroy_bitmap(render);
}

void CDisplayECAM::Render()
{
	al_set_target_bitmap(render);
	al_clear_to_color(al_map_rgba(0,0,0,0));

	if (CStateSimulator::MStateSimulator.GetAvionics())
	{
		al_draw_bitmap(fuelBack, 0, 0, 0);

		al_draw_filled_rectangle(68, 52, 138,
			232-(CStateSimulator::MStateSimulator.GetFuel()/CStateSimulator::MStateSimulator.GetMaxFuel()*180.0f), al_map_rgb(96,96,96));
		al_draw_filled_rectangle(302, 52, 372,
			232-(CStateSimulator::MStateSimulator.GetFuel()/CStateSimulator::MStateSimulator.GetMaxFuel()*180.0f), al_map_rgb(96,96,96));

		al_draw_bitmap(fuelBG, 0, 0, 0);
		al_draw_textf(font, al_map_rgb(255, 255, 255), 228, 156, ALLEGRO_ALIGN_RIGHT, "%i", int(CStateSimulator::MStateSimulator.GetFuel()));
		al_draw_textf(font, al_map_rgb(255, 255, 255), 462, 156, ALLEGRO_ALIGN_RIGHT, "%i", int(CStateSimulator::MStateSimulator.GetFuel()/2.5f));
		al_draw_textf(font, al_map_rgb(255, 255, 255), 612, 104, ALLEGRO_ALIGN_RIGHT, "%i", int(CStateSimulator::MStateSimulator.GetFuelWeight()));

		if (CInterfaceSimulator::MInterfaceSimulator.GetFuelWarning())
		{
			al_draw_bitmap(fuelLow, 0, 0, 0);
		}
		if (CInterfaceSimulator::MInterfaceSimulator.GetFuelEmpty())
		{
			al_draw_bitmap(fuelEmpty, 0, 0, 0);
		}
			//Weight - N2O4 - 1.443 g/cc - UDMH - 0.793 g/cc
	}
	else
		al_draw_bitmap(off, 0, 0, 0);

	al_draw_bitmap(border, 0, 0, 0);

	al_set_target_bitmap(al_get_backbuffer(CStateManager::MStateManager.GetDisplay()));
	al_draw_scaled_bitmap(render, 0, 0, 800, 336, x, y, width, height, 0);
}