#pragma once

#include "statemanager.h"
#include "statebase.h"

class CStateIntro : public CStateBase
{
protected:
	CStateIntro() {};

private:
	ALLEGRO_BITMAP *allegro;
	ALLEGRO_BITMAP *gwen;
	ALLEGRO_BITMAP *m;
	ALLEGRO_SAMPLE *sound;
	ALLEGRO_SAMPLE_INSTANCE *soundInstance;
	bool fade;
	int count;
	int image;
	enum image{ALLEGRO, GWEN, M};
	float width;
	float height;
	float x;
	float y;

public:
	static CStateIntro MStateIntro;
	void Load();
	void Init();
	void Resize();
	void Cleanup();

	void Pause() {};
	void Resume() {};

	void HandleEvents(ALLEGRO_EVENT& ev);
	void Render();

};