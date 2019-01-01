#include "entityrock.h"
#include "statesimulator.h"

void CEntityRock::Render(float screenX, float screenY)
{
	switch (rock)
	{
	case 0:
		al_draw_tinted_scaled_rotated_bitmap(CStateSimulator::MStateSimulator.GetRock1(), CStateSimulator::MStateSimulator.GetRockColor(), 62, 48, x-screenX, 100000 + y - screenY, scale, scale, rotation*ALLEGRO_PI/180.0f, 0);
		break;
	case 1:
		al_draw_tinted_scaled_rotated_bitmap(CStateSimulator::MStateSimulator.GetRock2(), CStateSimulator::MStateSimulator.GetRockColor(), 42, 38, x-screenX, 100000 + y - screenY, scale, scale, rotation*ALLEGRO_PI/180.0f, 0);
		break;
	case 2:
		al_draw_tinted_scaled_rotated_bitmap(CStateSimulator::MStateSimulator.GetRock3(), CStateSimulator::MStateSimulator.GetRockColor(), 42, 42, x-screenX, 100000 + y - screenY, scale, scale, rotation*ALLEGRO_PI/180.0f, 0);
		break;
	}
}

bool CEntityRock::GetCrash(float shipX, float shipY)
{
	if (sqrt((shipX-x)*(shipX-x) + (shipY-100000-y)*(shipY-100000-y)) <= 42.0f*(1+scale))
	{
		return true;
	}
	else
		return false;
}