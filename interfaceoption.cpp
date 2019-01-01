#include "interfaceoption.h"
#include "interfacedialog.h"

CInterfaceOption CInterfaceOption::MInterfaceOption;

void CInterfaceOption::Init()
{
	open = true;
	height = al_get_display_height(CStateManager::MStateManager.GetDisplay())/4.0f*3.0f;
	width = height*1.2f;

	//Window
	window = new Controls::WindowControl(CStateManager::MStateManager.GetCanvas());
	window->SetTitle(L"Options");
	window->MakeModal(true);
	window->SetDeleteOnClose(true);
	window->SetClosable(false);
	window->SetSkin(CStateManager::MStateManager.GetDefaultSkin());
	window->onWindowClosed.Add(this, &CInterfaceOption::WindowClose);
	window->SetSize(width, height);
	window->SetPos(al_get_display_width(CStateManager::MStateManager.GetDisplay())/2 - width/2, al_get_display_height(CStateManager::MStateManager.GetDisplay())/2 - height/2);

	//Bottom Buttons
	buttonSave = new Controls::Button(window);
	buttonSave->SetText(L"OK");
	buttonSave->SetBounds(width-CStateManager::MStateManager.GetButtonWidth()*2-CStateManager::MStateManager.GetButtonHeight()*1.5, height-CStateManager::MStateManager.GetButtonHeight()*2.5, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	buttonSave->onPress.Add(this, &CInterfaceOption::ButtonSaveClick);
	buttonSave->SetImageAlpha(0);

	buttonCancel = new Controls::Button(window);
	buttonCancel->SetText(L"Cancel");
	buttonCancel->SetBounds(width-CStateManager::MStateManager.GetButtonWidth()-CStateManager::MStateManager.GetButtonHeight(), height-CStateManager::MStateManager.GetButtonHeight()*2.5, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	buttonCancel->onPress.Add(this, &CInterfaceOption::ButtonCancelClick);

	//Options
	labelVolume = new Controls::Label(window);
	labelVolume->SetText(L"Volume:");
	labelVolume->SetAlignment( Pos::Right | Pos::CenterV );
	labelVolume->SetBounds(CStateManager::MStateManager.GetButtonHeight()/2, CStateManager::MStateManager.GetButtonHeight()/2, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	sliderVolume = new Controls::HorizontalSlider(window);
	sliderVolume->SetRange(0, 100);
	sliderVolume->SetNotchCount(50);
	sliderVolume->SetClampToNotches(true);
	sliderVolume->SetBounds(CStateManager::MStateManager.GetButtonHeight() + CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight()/2, CStateManager::MStateManager.GetButtonWidth()*2, CStateManager::MStateManager.GetButtonHeight());
	sliderVolumeChanged = false;
	if (CSetting::MSetting.GetVolume() >= 0.98)
		sliderVolume->SetFloatValue(98);
	else
		sliderVolume->SetFloatValue(CSetting::MSetting.GetVolume() * 100);
	sliderVolume->onValueChanged.Add(this, &CInterfaceOption::SliderVolumeChanged);
	labelVolumePercent = new Controls::Label(window);
	if (CSetting::MSetting.GetVolume() >= 0.98)
		labelVolumePercent->SetText(to_string(int(CSetting::MSetting.GetVolume()*100)) + "%");
	else
		labelVolumePercent->SetText(to_string(int(sliderVolume->GetFloatValue())) + "%");
	labelVolumePercent->SetAlignment( Pos::Left | Pos::CenterV );
	labelVolumePercent->SetBounds(CStateManager::MStateManager.GetButtonHeight() + CStateManager::MStateManager.GetButtonWidth()*3, CStateManager::MStateManager.GetButtonHeight()/2, CStateManager::MStateManager.GetButtonWidth()*2, CStateManager::MStateManager.GetButtonHeight());

	labelFullscreen = new Controls::Label(window);
	labelFullscreen->SetText(L"Fullscreen Mode:");
	labelFullscreen->SetAlignment( Pos::Right | Pos::CenterV );
	labelFullscreen->SetBounds(CStateManager::MStateManager.GetButtonHeight()/2, CStateManager::MStateManager.GetButtonHeight()*2, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	buttonFullscreen = new Controls::Button(window);
	buttonFullscreen->SetBounds(CStateManager::MStateManager.GetButtonHeight() + CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight()*2, CStateManager::MStateManager.GetButtonWidth()/2, CStateManager::MStateManager.GetButtonHeight());
	buttonFullscreen->SetIsToggle(true);
	if (CSetting::MSetting.GetFullscreen())
	{
		buttonFullscreen->SetToggleState(true);
		buttonFullscreen->SetText("On");
	}
	else
	{
		buttonFullscreen->SetToggleState(false);
		buttonFullscreen->SetText("Off");
	}
	buttonFullscreen->onToggleOn.Add(this, &CInterfaceOption::ButtonFullscreenToggleOn);
	buttonFullscreen->onToggleOff.Add(this, &CInterfaceOption::ButtonFullscreenToggleOff);

}

void CInterfaceOption::WindowClose()
{
	CInterfaceOption::MInterfaceOption.SetOpen(false);
}

void CInterfaceOption::ButtonSaveClick()
{
	//Save Settings
	if (CInterfaceOption::MInterfaceOption.sliderVolumeChanged)
	{
		CSetting::MSetting.SetVolume(CInterfaceOption::MInterfaceOption.sliderVolume->GetFloatValue()/100);	
	}
	CSetting::MSetting.SetFullscreen(CInterfaceOption::MInterfaceOption.buttonFullscreen->GetToggleState());
	CSetting::MSetting.SaveSettings();
	CInterfaceOption::MInterfaceOption.window->CloseButtonPressed();
}

void CInterfaceOption::ButtonCancelClick()
{
	CInterfaceOption::MInterfaceOption.window->CloseButtonPressed();
}

void CInterfaceOption::SliderVolumeChanged()
{
	CInterfaceOption::MInterfaceOption.sliderVolumeChanged = true;
	CInterfaceOption::MInterfaceOption.labelVolumePercent->SetText(to_string(int(CInterfaceOption::MInterfaceOption.sliderVolume->GetFloatValue())) + "%");
}

void CInterfaceOption::ButtonFullscreenToggleOn()
{
	//Show Dialog
	CInterfaceOption::MInterfaceOption.buttonFullscreen->SetText("On");
	CInterfaceDialog::MInterfaceDialog.Init("Options", "Fullscreen Mode On", "Please restart program to apply Fullscreen Changes", OK);
}

void CInterfaceOption::ButtonFullscreenToggleOff()
{
	//Show Dialog
	CInterfaceOption::MInterfaceOption.buttonFullscreen->SetText("Off");
	CInterfaceDialog::MInterfaceDialog.Init("Options", "Fullscreen Mode Off", "Please restart program to apply Fullscreen Changes", OK);
}