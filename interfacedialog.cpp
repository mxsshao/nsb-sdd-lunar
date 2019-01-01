#include "interfacedialog.h"

CInterfaceDialog CInterfaceDialog::MInterfaceDialog;

void CInterfaceDialog::Init(string title, TextObject textOne, TextObject textTwo, int type)
{
	CInterfaceDialog::type = type;
	//Size
	open = true;
	width = al_get_display_height(CStateManager::MStateManager.GetDisplay())/2.0f*1.2f;
	buttonHeight = CStateManager::MStateManager.GetButtonHeight();
	buttonWidth = buttonHeight*4.0f;

	//Window
	window = new Controls::WindowControl(CStateManager::MStateManager.GetCanvas());
	window->SetTitle(title);
	window->MakeModal(true);
	window->SetDeleteOnClose(true);
	window->SetClosable(false);
	window->SetSkin(CStateManager::MStateManager.GetDefaultSkin());
	window->onWindowClosed.Add(this, &CInterfaceDialog::WindowClose);

	//Paragraph One
	labelOne = new Controls::Label(window);
	labelOne->SetText(textOne);
	labelOne->SetWrap(true);
	labelOne->SetWidth(width-buttonHeight);
	labelOne->SizeToContents();
	labelOne->SetPos(buttonHeight/2, buttonHeight/2);
	//Paragraph Two
	labelTwo = new Controls::Label(window);
	labelTwo->SetText(textTwo);
	labelTwo->SetWrap(true);
	labelTwo->SetWidth(width-buttonHeight);
	labelTwo->SizeToContents();
	labelTwo->SetPos(buttonHeight/2, buttonHeight + labelOne->GetBounds().h);

	//Dialog Height
	height = buttonHeight*5 + labelOne->GetBounds().h + labelTwo->GetBounds().h;

	//Dialog Window
	window->SetSize(width, height);
	window->SetPos(al_get_display_width(CStateManager::MStateManager.GetDisplay())/2 - width/2, al_get_display_height(CStateManager::MStateManager.GetDisplay())/2 - height/2);

	//Dialog Type
	if (type == OK)
	{
		buttonOK = new Controls::Button(window);
		buttonOK->SetText(L"OK");
		buttonOK->SetBounds(width-buttonWidth-buttonHeight, height-buttonHeight*2.5, buttonWidth, buttonHeight);
		buttonOK->onPress.Add(this, &CInterfaceDialog::ButtonOKClick);
	}
	else if (type == YESNO)
	{
		buttonYes = new Controls::Button(window);
		buttonYes->SetText(L"Yes");
		buttonYes->SetBounds(width-buttonWidth*2-buttonHeight*1.5, height-buttonHeight*2.5, buttonWidth, buttonHeight);
		buttonYes->onPress.Add(this, &CInterfaceDialog::ButtonYesClick);

		buttonNo = new Controls::Button(window);
		buttonNo->SetText(L"No");
		buttonNo->SetBounds(width-buttonWidth-buttonHeight, height-buttonHeight*2.5, buttonWidth, buttonHeight);
		buttonNo->onPress.Add(this, &CInterfaceDialog::ButtonNoClick);
	}
	else if (type == EXIT)
	{
		buttonYes = new Controls::Button(window);
		buttonYes->SetText(L"Resume");
		buttonYes->SetBounds(width-buttonWidth*2-buttonHeight*1.5, height-buttonHeight*2.5, buttonWidth, buttonHeight);
		buttonYes->onPress.Add(this, &CInterfaceDialog::ButtonYesClick);

		buttonNo = new Controls::Button(window);
		buttonNo->SetText(L"Exit");
		buttonNo->SetBounds(width-buttonWidth-buttonHeight, height-buttonHeight*2.5, buttonWidth, buttonHeight);
		buttonNo->onPress.Add(this, &CInterfaceDialog::ButtonNoClick);
	}
	else if (type == CRASH)
	{
		buttonYes = new Controls::Button(window);
		buttonYes->SetText(L"Retry");
		buttonYes->SetBounds(width-buttonWidth*2-buttonHeight*1.5, height-buttonHeight*2.5, buttonWidth, buttonHeight);
		buttonYes->onPress.Add(this, &CInterfaceDialog::ButtonYesClick);

		buttonNo = new Controls::Button(window);
		buttonNo->SetText(L"Exit");
		buttonNo->SetBounds(width-buttonWidth-buttonHeight, height-buttonHeight*2.5, buttonWidth, buttonHeight);
		buttonNo->onPress.Add(this, &CInterfaceDialog::ButtonNoClick);
	}
	else if (type == VICTORY)
	{
		buttonYes = new Controls::Button(window);
		buttonYes->SetText(L"Fly Again");
		buttonYes->SetBounds(width-buttonWidth*2-buttonHeight*1.5, height-buttonHeight*2.5, buttonWidth, buttonHeight);
		buttonYes->onPress.Add(this, &CInterfaceDialog::ButtonYesClick);

		buttonNo = new Controls::Button(window);
		buttonNo->SetText(L"Exit");
		buttonNo->SetBounds(width-buttonWidth-buttonHeight, height-buttonHeight*2.5, buttonWidth, buttonHeight);
		buttonNo->onPress.Add(this, &CInterfaceDialog::ButtonNoClick);
	}
}

void CInterfaceDialog::WindowClose()
{
	CInterfaceDialog::MInterfaceDialog.SetOpen(false);
}

void CInterfaceDialog::ButtonOKClick()
{
	CInterfaceDialog::MInterfaceDialog.window->CloseButtonPressed();
}

void CInterfaceDialog::ButtonYesClick()
{
	CInterfaceDialog::MInterfaceDialog.window->CloseButtonPressed();
}

void CInterfaceDialog::ButtonNoClick()
{
	CInterfaceDialog::MInterfaceDialog.window->CloseButtonPressed();
}