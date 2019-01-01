#pragma once

#include "global.h"
#include "statemenu.h"

class CInterfaceMenu : public Event::Handler
{
protected:
	CInterfaceMenu() {};

private:
	ALLEGRO_BITMAP* button;
	ALLEGRO_BITMAP* buttonLeft;
	float buttonWidth;
	float buttonHeight;
	float buttonX;
	float buttonY;

	ALLEGRO_SAMPLE* mouseOver;

	int buttonTutorialCount;
	int buttonFreeCount;
	int buttonOptionCount;
	int buttonCloseCount;
	Controls::Button* buttonTutorial;
	Controls::Button* buttonFree;
	Controls::Button* buttonOption;
	Controls::Button* buttonClose;
	int buttonSelect;
	enum buttonSelect{NONE, TUTORIAL, FREE, OPTION, CLOSE};

	void buttonHoverEnter();
	void buttonTutorialClick();
	void buttonTutorialHoverEnter();
	void buttonTutorialHoverLeave();
	void buttonFreeClick();
	void buttonFreeHoverEnter();
	void buttonFreeHoverLeave();
	void buttonOptionClick();
	void buttonOptionHoverEnter();
	void buttonOptionHoverLeave();
	void buttonCloseClick();
	void buttonCloseHoverEnter();
	void buttonCloseHoverLeave();
	void Close();

public:
	static CInterfaceMenu MInterfaceMenu;
	void Load();
	void Init();
	void Resize();
	void Cleanup();

	void HandleEvents(ALLEGRO_EVENT& ev);
	void Render();

	int GetButtonSelect() {return buttonSelect;};
	void SetButtonSelect(int s) {buttonSelect = s;};
};