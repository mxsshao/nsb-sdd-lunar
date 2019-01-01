#pragma once

#include "interfacesimulator.h"

class CDisplayECAM
{
protected:
	CDisplayECAM() {};

private:
	ALLEGRO_BITMAP* render;

	ALLEGRO_FONT* font;
	ALLEGRO_BITMAP* border;
	ALLEGRO_BITMAP* off;

	ALLEGRO_BITMAP* fuelBG;
	ALLEGRO_BITMAP* fuelBack;

	ALLEGRO_BITMAP* fuelLow;
	ALLEGRO_BITMAP* fuelEmpty;

	float x;
	float y;
	float height;
	float width;

public:
	static CDisplayECAM MDisplayECAM;
	void Load();
	void Init();
	void Resize();
	void Cleanup();

	void Render();
};