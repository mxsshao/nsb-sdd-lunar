#pragma once

#include "global.h"
#include "setting.h"
#include "statemanager.h"

class CInterfaceOption : public Event::Handler
{
protected:
	CInterfaceOption() {};

private:
	Controls::WindowControl* window;
	
	Controls::Button* buttonSave;
	Controls::Button* buttonCancel;

	Controls::Label* labelVolume;	
	Controls::HorizontalSlider* sliderVolume;
	bool sliderVolumeChanged;
	Controls::Label* labelVolumePercent;

	Controls::Label* labelFullscreen;
	Controls::Button* buttonFullscreen;

	float width;
	float height;

	bool open;

	void WindowClose();

	void ButtonSaveClick();
	void ButtonCancelClick();

	void SliderVolumeChanged();

	void ButtonFullscreenToggleOn();
	void ButtonFullscreenToggleOff();

public:
	static CInterfaceOption MInterfaceOption;
	void Init();

	bool GetOpen() {return open;};
	void SetOpen(bool o) {open = o;};
	Controls::WindowControl* GetWindow() {return window;};
};