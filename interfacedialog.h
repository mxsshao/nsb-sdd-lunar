#pragma once

#include "global.h"
#include "setting.h"
#include "statemanager.h"

enum type {YESNO, OK, EXIT, CRASH, VICTORY};

class CInterfaceDialog : public Event::Handler
{
protected:
	CInterfaceDialog() {};

private:

	Controls::WindowControl* window;
	
	Controls::Button* buttonYes;
	Controls::Button* buttonNo;
	Controls::Button* buttonOK;
	Controls::Label* labelOne;
	Controls::Label* labelTwo;

	float width;
	float height;
	float buttonWidth;
	float buttonHeight;
	int type;

	bool open;

public:
	static CInterfaceDialog MInterfaceDialog;
	void Init(string title, TextObject textOne, TextObject textTwo, int type);

	int GetType() {return type;};

	bool GetOpen() {return open;};
	void SetOpen(bool o) {open = o;};
	Controls::WindowControl* GetWindow() {return window;};
	void WindowClose();

	void ButtonOKClick();
	void ButtonYesClick();
	void ButtonNoClick();

	Controls::Button* GetYes() {return buttonYes;};
	Controls::Button* GetNo() {return buttonNo;};
};