#pragma once

#include "interfacesimulator.h"

class CDisplayPFD
{
protected:
	CDisplayPFD() {};

private:
	ALLEGRO_BITMAP* render;

	ALLEGRO_FONT* font;
	ALLEGRO_BITMAP* bg;
	ALLEGRO_BITMAP* back;
	ALLEGRO_BITMAP* attitude;
	ALLEGRO_BITMAP* altitude;
	ALLEGRO_BITMAP* speed;
	ALLEGRO_BITMAP* border;
	ALLEGRO_BITMAP* off;
	ALLEGRO_BITMAP* overspeed;
	float y;
	float width;

public:
	static CDisplayPFD MDisplayPFD;
	void Load();
	void Init();
	void Resize();
	void Cleanup();

	void Render();

	float GetWidth() {return width;};
};