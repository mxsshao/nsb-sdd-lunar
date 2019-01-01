#pragma once

#include "global.h"
#include "statefree.h"

class CInterfaceFree: public Event::Handler
{
protected:
	CInterfaceFree() {};

private:
	ALLEGRO_BITMAP* ship;

	ALLEGRO_BITMAP* buttonLeft;
	float buttonWidth;
	float buttonHeight;
	float buttonY;

	ALLEGRO_SAMPLE* mouseOver;

	int buttonVenusCount;
	int buttonEarthCount;
	int buttonMoonCount;
	int buttonMarsCount;
	int buttonCustomCount;
	Controls::Button* buttonVenus;
	Controls::Button* buttonEarth;
	Controls::Button* buttonMoon;
	Controls::Button* buttonMars;
	Controls::Button* buttonCustom;
	int buttonBackCount;
	Controls::Button* buttonBack;
	int buttonSelect;
	enum buttonSelect{NONE, VENUS, EARTH, MOON, MARS, CUSTOM, BACK};

	void buttonHoverEnter();
	void buttonVenusClick();
	void buttonVenusHoverEnter();
	void buttonVenusHoverLeave();
	void buttonEarthClick();
	void buttonEarthHoverEnter();
	void buttonEarthHoverLeave();
	void buttonMoonClick();
	void buttonMoonHoverEnter();
	void buttonMoonHoverLeave();
	void buttonMarsClick();
	void buttonMarsHoverEnter();
	void buttonMarsHoverLeave();
	void buttonCustomClick();
	void buttonCustomHoverEnter();
	void buttonCustomHoverLeave();
	void buttonBackClick();
	void buttonBackHoverEnter();
	void buttonBackHoverLeave();

public:
	static CInterfaceFree MInterfaceFree;
	void Load();
	void Init();
	void Resize();
	void Cleanup();

	void HandleEvents(ALLEGRO_EVENT& ev);
	void Render();

	int GetButtonSelect() {return buttonSelect;};
	void SetButtonSelect(int s) {buttonSelect = s;};
};