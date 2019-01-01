#include "interfacemenu.h"
#include "interfaceoption.h"
#include "interfacedialog.h"
#include "statesimulator.h"
#include "statetraining.h"
#include "statefree.h"

CInterfaceMenu CInterfaceMenu::MInterfaceMenu;

void CInterfaceMenu::Load()
{
	button = al_load_bitmap("menu/button.png");
	buttonLeft = al_load_bitmap("menu/buttonleft.png");

	mouseOver = al_load_sample("menu/mouseover.ogg");

	CStateManager::MStateManager.GetCanvas()->SetSkin(CStateManager::MStateManager.GetMenuSkin());

	//---------
	//BUTTONS
	//---------

	//Training
	buttonTutorial = new Controls::Button(CStateManager::MStateManager.GetCanvas());
	buttonTutorial->SetText(L"Training");
	buttonTutorial->onPress.Add(this, &CInterfaceMenu::buttonTutorialClick);
	buttonTutorial->onHoverEnter.Add(this, &CInterfaceMenu::buttonHoverEnter);
	buttonTutorial->onHoverEnter.Add(this, &CInterfaceMenu::buttonTutorialHoverEnter);
	buttonTutorial->onHoverLeave.Add(this, &CInterfaceMenu::buttonTutorialHoverLeave);

	//Free Flight
	buttonFree = new Controls::Button(CStateManager::MStateManager.GetCanvas());
	buttonFree->SetText(L"Free Flight");
	buttonFree->onPress.Add(this, &CInterfaceMenu::buttonFreeClick);
	buttonFree->onHoverEnter.Add(this, &CInterfaceMenu::buttonHoverEnter);
	buttonFree->onHoverEnter.Add(this, &CInterfaceMenu::buttonFreeHoverEnter);
	buttonFree->onHoverLeave.Add(this, &CInterfaceMenu::buttonFreeHoverLeave);

	//Options
	buttonOption = new Controls::Button(CStateManager::MStateManager.GetCanvas());
	buttonOption->SetText(L"Options");
	buttonOption->onPress.Add(this, &CInterfaceMenu::buttonOptionClick);
	buttonOption->onHoverEnter.Add(this, &CInterfaceMenu::buttonHoverEnter);
	buttonOption->onHoverEnter.Add(this, &CInterfaceMenu::buttonOptionHoverEnter);
	buttonOption->onHoverLeave.Add(this, &CInterfaceMenu::buttonOptionHoverLeave);

	//Close
	buttonClose = new Controls::Button(CStateManager::MStateManager.GetCanvas());
	buttonClose->SetText(L"Exit");
	buttonClose->onPress.Add(this, &CInterfaceMenu::buttonCloseClick);
	buttonClose->onHoverEnter.Add(this, &CInterfaceMenu::buttonHoverEnter);
	buttonClose->onHoverEnter.Add(this, &CInterfaceMenu::buttonCloseHoverEnter);
	buttonClose->onHoverLeave.Add(this, &CInterfaceMenu::buttonCloseHoverLeave);
}

void CInterfaceMenu::Init()
{
	al_convert_bitmaps();

	//VARIABLES
	buttonTutorialCount = 20;
	buttonFreeCount = 20;
	buttonOptionCount = 20;
	buttonSelect = NONE;
}

void CInterfaceMenu::Resize()
{
	buttonHeight = al_get_display_height(CStateManager::MStateManager.GetDisplay())/14;
	buttonWidth = buttonHeight * 8.0f;
	buttonX = al_get_display_width(CStateManager::MStateManager.GetDisplay())-buttonWidth;
	buttonY = al_get_display_height(CStateManager::MStateManager.GetDisplay())-buttonHeight*2;

	Padding padding(0, 0, buttonHeight/2, 0);

	//Resize Buttons
	buttonTutorial->SetBounds(buttonX, buttonY-buttonHeight*3, buttonWidth, buttonHeight);
	buttonTutorial->SetFont(L"font/RyskyLines.ttf", buttonHeight/3, true);
	buttonTutorial->SetTextPadding(padding);
	buttonTutorial->SetAlignment( Pos::Right | Pos::CenterV );
	buttonFree->SetBounds(buttonX, buttonY-buttonHeight*1.5, buttonWidth, buttonHeight);
	buttonFree->SetFont(buttonTutorial->GetFont());
	buttonFree->SetTextPadding(padding);
	buttonFree->SetAlignment( Pos::Right | Pos::CenterV );
	buttonOption->SetBounds(buttonX, buttonY, buttonWidth, buttonHeight);
	buttonOption->SetFont(buttonTutorial->GetFont());
	buttonOption->SetTextPadding(padding);
	buttonOption->SetAlignment( Pos::Right | Pos::CenterV );
	buttonClose->SetBounds(0, buttonY, buttonWidth, buttonHeight);
	buttonClose->SetFont(buttonTutorial->GetFont());
	buttonClose->SetTextPadding(Padding(buttonHeight/2, 0, 0, 0));
	buttonClose->SetAlignment( Pos::Left | Pos::CenterV );

	//Close Open Windows
	if (CInterfaceOption::MInterfaceOption.GetOpen())
		CInterfaceOption::MInterfaceOption.GetWindow()->CloseButtonPressed();

	if (CInterfaceDialog::MInterfaceDialog.GetOpen())
		CInterfaceDialog::MInterfaceDialog.GetWindow()->CloseButtonPressed();
}

void CInterfaceMenu::Cleanup()
{
	buttonTutorial->DelayedDelete();
	buttonFree->DelayedDelete();
	buttonOption->DelayedDelete();
	buttonClose->DelayedDelete();

	al_destroy_bitmap(button);
	al_destroy_bitmap(buttonLeft);
	al_destroy_sample(mouseOver);
}

void CInterfaceMenu::buttonHoverEnter()
{
	al_play_sample(CInterfaceMenu::MInterfaceMenu.mouseOver, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void CInterfaceMenu::buttonTutorialClick()
{
	CStateManager::MStateManager.ChangeState(&CStateTraining::MStateTraining);
}
void CInterfaceMenu::buttonTutorialHoverEnter()
{
	CInterfaceMenu::MInterfaceMenu.SetButtonSelect(TUTORIAL);
}
void CInterfaceMenu::buttonTutorialHoverLeave()
{
	CInterfaceMenu::MInterfaceMenu.SetButtonSelect(NONE);
}
void CInterfaceMenu::buttonFreeClick()
{
	CStateManager::MStateManager.ChangeState(&CStateFree::MStateFree);
}
void CInterfaceMenu::buttonFreeHoverEnter()
{
	CInterfaceMenu::MInterfaceMenu.SetButtonSelect(FREE);
}
void CInterfaceMenu::buttonFreeHoverLeave()
{
	CInterfaceMenu::MInterfaceMenu.SetButtonSelect(NONE);
}
void CInterfaceMenu::buttonOptionClick()
{
	CInterfaceOption::MInterfaceOption.Init();
}
void CInterfaceMenu::buttonOptionHoverEnter()
{
	CInterfaceMenu::MInterfaceMenu.SetButtonSelect(OPTION);
}
void CInterfaceMenu::buttonOptionHoverLeave()
{
	CInterfaceMenu::MInterfaceMenu.SetButtonSelect(NONE);
}
void CInterfaceMenu::buttonCloseClick()
{
	//Show Dialog
	CInterfaceDialog::MInterfaceDialog.Init("Exit", "Are you sure you want to exit this program?", "", YESNO);
	CInterfaceDialog::MInterfaceDialog.GetYes()->onPress.Add(this, &CInterfaceMenu::Close);
}
void CInterfaceMenu::buttonCloseHoverEnter()
{
	CInterfaceMenu::MInterfaceMenu.SetButtonSelect(CLOSE);
}
void CInterfaceMenu::buttonCloseHoverLeave()
{
	CInterfaceMenu::MInterfaceMenu.SetButtonSelect(NONE);
}

void CInterfaceMenu::Close()
{
	CStateManager::MStateManager.Exit();
}

void CInterfaceMenu::HandleEvents(ALLEGRO_EVENT& ev)
{
	if (ev.type == ALLEGRO_EVENT_TIMER)
	{
		//Animation Counter
		if (buttonSelect == TUTORIAL)
		{
			if (buttonTutorialCount != 0)
				buttonTutorialCount -= 1;
		}
		else
		{
			if (buttonTutorialCount != 20)
				buttonTutorialCount += 1;
		}
		if (buttonSelect == FREE)
		{
			if (buttonFreeCount != 0)
				buttonFreeCount -= 1;
		}
		else
		{
			if (buttonFreeCount != 20)
				buttonFreeCount += 1;
		}
		if (buttonSelect == OPTION)
		{
			if (buttonOptionCount != 0)
				buttonOptionCount -= 1;
		}
		else
		{
			if (buttonOptionCount != 20)
				buttonOptionCount += 1;
		}
		if (buttonSelect == CLOSE)
		{
			if (buttonCloseCount != 0)
				buttonCloseCount -= 1;
		}
		else
		{
			if (buttonCloseCount != 20)
				buttonCloseCount += 1;
		}
	}
	else if (ev.type == ALLEGRO_EVENT_KEY_DOWN && !CInterfaceOption::MInterfaceOption.GetOpen() && !CInterfaceDialog::MInterfaceDialog.GetOpen())
	{
		//Keyboard Selection
		switch (ev.keyboard.keycode)
		{
		case ALLEGRO_KEY_UP:
			if (buttonSelect == TUTORIAL)
				buttonSelect = OPTION;
			else if (buttonSelect == NONE)
				buttonSelect = TUTORIAL;
			else if (buttonSelect == CLOSE)
				buttonSelect = TUTORIAL;
			else
				buttonSelect -= 1;

			buttonHoverEnter();

			break;
		case ALLEGRO_KEY_DOWN:
			if (buttonSelect == OPTION)
				buttonSelect = TUTORIAL;
			else if (buttonSelect == NONE)
				buttonSelect = TUTORIAL;
			else if (buttonSelect == CLOSE)
				buttonSelect = OPTION;
			else
				buttonSelect += 1;

			buttonHoverEnter();

			break;
		case ALLEGRO_KEY_RIGHT:
			if (buttonSelect == CLOSE)
			{
				buttonSelect = TUTORIAL;
				buttonHoverEnter();
			}
			break;
		case ALLEGRO_KEY_LEFT:
			if (buttonSelect != CLOSE)
			{
				buttonSelect = CLOSE;
				buttonHoverEnter();
			}
			break;
		case ALLEGRO_KEY_ENTER:
			switch (buttonSelect)
			{
			case TUTORIAL:
				buttonTutorialClick();
				break;
			case FREE:
				buttonFreeClick();
				break;
			case OPTION:
				buttonOptionClick();
				break;
			case CLOSE:
				buttonCloseClick();
				break;
			}
			break;
		}
	}
}

void CInterfaceMenu::Render()
{
	//Button Images
	al_draw_scaled_bitmap(button, 0, 0, 600, 100, buttonX+buttonTutorialCount*buttonWidth/20, buttonY-buttonHeight*3, buttonWidth, buttonHeight, 0);
	al_draw_scaled_bitmap(button, 0, 0, 600, 100, buttonX+buttonFreeCount*buttonWidth/20, buttonY-buttonHeight*1.5, buttonWidth, buttonHeight, 0);
	al_draw_scaled_bitmap(button, 0, 0, 600, 100, buttonX+buttonOptionCount*buttonWidth/20, buttonY, buttonWidth, buttonHeight, 0);
	al_draw_scaled_bitmap(buttonLeft, 0, 0, 600, 100, 0-buttonCloseCount*buttonWidth/20, buttonY, buttonWidth, buttonHeight, 0);
}