#pragma once

#include "global.h"
#include "statesimulator.h"

class CInterfaceSimulator : public Event::Handler
{
protected:
	CInterfaceSimulator() {};

private:
	ALLEGRO_FONT* font;

	ALLEGRO_BITMAP* panel;
	ALLEGRO_BITMAP* panelBG;
	ALLEGRO_BITMAP* overhead;
	float y;
	float width;
	float height;

	ALLEGRO_BITMAP* throttle;

	ALLEGRO_SAMPLE* switchSound;
	ALLEGRO_BITMAP* switchUp;
	ALLEGRO_BITMAP* switchDown;
	float switchWidth;
	float switchHeight;

	ALLEGRO_BITMAP* nosmoking;
	ALLEGRO_BITMAP* seatbelt;
	float indicatorX;
	float indicatorY;
	float indicatorWidth;
	float indicatorHeight;
	Controls::Button* buttonNoSmoking;
	Controls::Button* buttonSeatbelt;

	ALLEGRO_BITMAP* warningEngine;
	ALLEGRO_BITMAP* warning200;
	ALLEGRO_BITMAP* warning100;
	ALLEGRO_BITMAP* warning50;
	ALLEGRO_BITMAP* warning40;
	ALLEGRO_BITMAP* warning30;
	ALLEGRO_BITMAP* warning20;
	ALLEGRO_BITMAP* warning10;
	ALLEGRO_SAMPLE* sound200;
	ALLEGRO_SAMPLE* sound100;
	ALLEGRO_SAMPLE* sound50;
	ALLEGRO_SAMPLE* sound40;
	ALLEGRO_SAMPLE* sound30;
	ALLEGRO_SAMPLE* sound20;
	ALLEGRO_SAMPLE* sound10;
	ALLEGRO_SAMPLE* sound5;

	float warningWidth;
	bool w200;
	bool w100;
	bool w50;
	bool w40;
	bool w30;
	bool w20;
	bool w10;
	bool w5;

	bool fuelWarning;
	bool fuelEmpty;
	bool engineFailure;
	bool bankAngle;
	bool overSpeed;
	bool sinkRate;

	ALLEGRO_BITMAP* imageWarning;
	ALLEGRO_BITMAP* imageWarningOn;
	ALLEGRO_BITMAP* imageCaution;
	ALLEGRO_BITMAP* imageCautionOn;
	ALLEGRO_BITMAP* imageAvion;
	ALLEGRO_BITMAP* imageAvionOn;
	Controls::Button* buttonWarning;
	Controls::Button* buttonCaution;
	Controls::Button* buttonAvion;

	void ButtonHoverEnter();
	void ButtonHoverLeave();
	void ButtonSwitchChanged();
	void ButtonAvionChanged();
	
	ALLEGRO_SAMPLE* victory;

	void Retry();
	void Exit();

public:
	static CInterfaceSimulator MInterfaceSimulator;
	void Load();
	void Init();
	void Resize();
	void Cleanup();

	void HandleEvents();
	void Render();

	bool GetFuelWarning() {return fuelWarning;};
	bool GetFuelEmpty() {return fuelEmpty;};

	void Pause();
	void Crash();
	void Landed();
};