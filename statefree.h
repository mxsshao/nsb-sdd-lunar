#pragma once

#include "statemanager.h"
#include "statebase.h"

class CStateFree: public CStateBase
{
protected:
	CStateFree() {};

private:
	ALLEGRO_BITMAP* bg;
	ALLEGRO_BITMAP* light;

	ALLEGRO_SAMPLE* sound;
	int count;
	float lightWidth;
	float lightHeight;
	float width;
	float height;
	float x;
	float y;

public:
	static CStateFree MStateFree;
	void Load();
	void Init();
	void Resize();
	void Cleanup();

	void Pause() {};
	void Resume() {};

	void HandleEvents(ALLEGRO_EVENT& ev);
	void Render();

	float GetHeight() {return height;};
	float GetWidth() {return width;};
	float GetX() {return x;};
	float GetY() {return y;};
};