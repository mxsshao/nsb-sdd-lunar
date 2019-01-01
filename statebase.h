#pragma once

#include "global.h"
#include "setting.h"

class CStateBase
{
protected:
	CStateBase() {};

public:
	virtual void Load() = 0;
	virtual void Init() = 0;
	virtual void Resize() = 0;
	virtual void Cleanup() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents(ALLEGRO_EVENT& ev) = 0;
	virtual void Render() = 0;
};