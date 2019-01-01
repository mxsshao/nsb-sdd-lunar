#pragma once

#include "statemanager.h"
#include "statebase.h"

class CStateMenu : public CStateBase
{
protected:
	CStateMenu() {};

private:
	ALLEGRO_BITMAP *bg;
	ALLEGRO_SAMPLE *sound;
	int count;
	float width;
	float height;
	float x;
	float y;

public:
	static CStateMenu MStateMenu;
	void Load();
	void Init();
	void Resize();
	void Cleanup();

	void Pause() {};
	void Resume() {};

	void HandleEvents(ALLEGRO_EVENT& ev);
	void Render();

};