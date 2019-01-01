#pragma once

#include "statemanager.h"
#include "statebase.h"

class CStateTraining : public CStateBase
{
protected:
	CStateTraining() {};

private:
	ALLEGRO_BITMAP *image;
	float width;
	float height;

public:
	static CStateTraining MStateTraining;
	void Load();
	void Init();
	void Resize();
	void Cleanup();

	void Pause() {};
	void Resume() {};

	void HandleEvents(ALLEGRO_EVENT& ev);
	void Render();

};