#include "interfacefree.h"
#include "interfaceplanet.h"
#include "statesimulator.h"
#include "statemenu.h"

CInterfaceFree CInterfaceFree::MInterfaceFree;

void CInterfaceFree::Load()
{
	buttonLeft = al_load_bitmap("menu/buttonleft.png");
	ship = al_load_bitmap("free/ship.png");
	mouseOver = al_load_sample("menu/mouseover.ogg");

	CStateManager::MStateManager.GetCanvas()->SetSkin(CStateManager::MStateManager.GetMenuSkin());

	//---------
	//BUTTONS
	//---------

	//Venus
	buttonVenus = new Controls::Button(CStateManager::MStateManager.GetCanvas());
	buttonVenus->onPress.Add(this, &CInterfaceFree::buttonVenusClick);
	buttonVenus->onHoverEnter.Add(this, &CInterfaceFree::buttonHoverEnter);
	buttonVenus->onHoverEnter.Add(this, &CInterfaceFree::buttonVenusHoverEnter);
	buttonVenus->onHoverLeave.Add(this, &CInterfaceFree::buttonVenusHoverLeave);

	//Earth
	buttonEarth = new Controls::Button(CStateManager::MStateManager.GetCanvas());
	buttonEarth->onPress.Add(this, &CInterfaceFree::buttonEarthClick);
	buttonEarth->onHoverEnter.Add(this, &CInterfaceFree::buttonHoverEnter);
	buttonEarth->onHoverEnter.Add(this, &CInterfaceFree::buttonEarthHoverEnter);
	buttonEarth->onHoverLeave.Add(this, &CInterfaceFree::buttonEarthHoverLeave);

	//Moon
	buttonMoon = new Controls::Button(CStateManager::MStateManager.GetCanvas());
	buttonMoon->onPress.Add(this, &CInterfaceFree::buttonMoonClick);
	buttonMoon->onHoverEnter.Add(this, &CInterfaceFree::buttonHoverEnter);
	buttonMoon->onHoverEnter.Add(this, &CInterfaceFree::buttonMoonHoverEnter);
	buttonMoon->onHoverLeave.Add(this, &CInterfaceFree::buttonMoonHoverLeave);

	//Mars
	buttonMars = new Controls::Button(CStateManager::MStateManager.GetCanvas());
	buttonMars->onPress.Add(this, &CInterfaceFree::buttonMarsClick);
	buttonMars->onHoverEnter.Add(this, &CInterfaceFree::buttonHoverEnter);
	buttonMars->onHoverEnter.Add(this, &CInterfaceFree::buttonMarsHoverEnter);
	buttonMars->onHoverLeave.Add(this, &CInterfaceFree::buttonMarsHoverLeave);

	//Custom
	buttonCustom = new Controls::Button(CStateManager::MStateManager.GetCanvas());
	buttonCustom->onPress.Add(this, &CInterfaceFree::buttonCustomClick);
	buttonCustom->onHoverEnter.Add(this, &CInterfaceFree::buttonHoverEnter);
	buttonCustom->onHoverEnter.Add(this, &CInterfaceFree::buttonCustomHoverEnter);
	buttonCustom->onHoverLeave.Add(this, &CInterfaceFree::buttonCustomHoverLeave);

	//Back
	buttonBack = new Controls::Button(CStateManager::MStateManager.GetCanvas());
	buttonBack->SetText(L"Back");
	buttonBack->onPress.Add(this, &CInterfaceFree::buttonBackClick);
	buttonBack->onHoverEnter.Add(this, &CInterfaceFree::buttonHoverEnter);
	buttonBack->onHoverEnter.Add(this, &CInterfaceFree::buttonBackHoverEnter);
	buttonBack->onHoverLeave.Add(this, &CInterfaceFree::buttonBackHoverLeave);
}

void CInterfaceFree::Init()
{
	buttonLeft = al_clone_bitmap(buttonLeft);
	ship = al_clone_bitmap(ship);

	//VARIABLES
	buttonVenusCount = rand() % 180;
	buttonEarthCount = rand() % 180;
	buttonMoonCount = rand() % 180;
	buttonMarsCount = rand() % 180;
	buttonCustomCount = rand() % 180;
	buttonBackCount = 20;
	buttonSelect = NONE;
}

void CInterfaceFree::Resize()
{
	//Resize
	buttonHeight = al_get_display_height(CStateManager::MStateManager.GetDisplay())/14;
	buttonWidth = buttonHeight * 8;
	buttonY = al_get_display_height(CStateManager::MStateManager.GetDisplay())-buttonHeight*2;

	//Resize Buttons
	buttonVenus->SetBounds(CStateFree::MStateFree.GetWidth()/384.0f*79.0f + CStateFree::MStateFree.GetX(), CStateFree::MStateFree.GetHeight()/25.0f*14.0f + CStateFree::MStateFree.GetY(),
		CStateFree::MStateFree.GetWidth()/384.0f*49.0f, CStateFree::MStateFree.GetWidth()/384.0f*49.0f);
	buttonEarth->SetBounds(CStateFree::MStateFree.GetWidth()/48.0f*23.0f + CStateFree::MStateFree.GetX(), CStateFree::MStateFree.GetHeight()/300.0f*113.0f + CStateFree::MStateFree.GetY(),
		CStateFree::MStateFree.GetWidth()/48.0f*7.0f, CStateFree::MStateFree.GetWidth()/48.0f*7.0f);
	buttonMoon->SetBounds(CStateFree::MStateFree.GetWidth()/120.0f*73.0f + CStateFree::MStateFree.GetX(), CStateFree::MStateFree.GetHeight()/40.0f*11.0f + CStateFree::MStateFree.GetY(),
		CStateFree::MStateFree.GetWidth()/480.0f*17.0f, CStateFree::MStateFree.GetWidth()/480.0f*17.0f);
	buttonMars->SetBounds(CStateFree::MStateFree.GetWidth()/24.0f*19.0f + CStateFree::MStateFree.GetX(), CStateFree::MStateFree.GetHeight()/200.0f*39.0f + CStateFree::MStateFree.GetY(),
		CStateFree::MStateFree.GetWidth()/10.0f, CStateFree::MStateFree.GetWidth()/10.0f);
	buttonCustom->SetBounds(CStateFree::MStateFree.GetWidth()/320.0f*253.0f + CStateFree::MStateFree.GetX(), CStateFree::MStateFree.GetHeight()/300.0f*217.0f + CStateFree::MStateFree.GetY(),
		CStateFree::MStateFree.GetWidth()/9.6f, CStateFree::MStateFree.GetWidth()/9.6f);

	buttonBack->SetBounds(0, buttonY, buttonWidth, buttonHeight);
	buttonBack->SetFont(L"font/RyskyLines.ttf", buttonHeight/3.0f, true);
	buttonBack->SetAlignment( Pos::Left | Pos::CenterV );
	buttonBack->SetTextPadding(Padding(buttonHeight/2.0f, 0, 0, 0));

	//Close Open Window
	if (CInterfacePlanet::MInterfacePlanet.GetOpen())
		CInterfacePlanet::MInterfacePlanet.GetWindow()->CloseButtonPressed();
}

void CInterfaceFree::Cleanup()
{
	buttonVenus->DelayedDelete();
	buttonEarth->DelayedDelete();
	buttonMoon->DelayedDelete();
	buttonMars->DelayedDelete();
	buttonCustom->DelayedDelete();
	buttonBack->DelayedDelete();

	al_destroy_sample(mouseOver);
	al_destroy_bitmap(buttonLeft);
	al_destroy_bitmap(ship);
}

void CInterfaceFree::buttonHoverEnter()
{
	al_play_sample(CInterfaceFree::MInterfaceFree.mouseOver, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
}
void CInterfaceFree::buttonVenusClick()
{
	CInterfacePlanet::MInterfacePlanet.Init("VENUS", "8.87", Color(91,37,3), Color(248,110,0), Color(168,111,0), true);
}
void CInterfaceFree::buttonVenusHoverEnter()
{
	CInterfaceFree::MInterfaceFree.SetButtonSelect(VENUS);
}
void CInterfaceFree::buttonVenusHoverLeave()
{
	CInterfaceFree::MInterfaceFree.SetButtonSelect(NONE);
}
void CInterfaceFree::buttonEarthClick()
{
	CInterfacePlanet::MInterfacePlanet.Init("EARTH", "9.8", Color(0,64,113), Color(0,142,33), Color(180,180,180), true);
}
void CInterfaceFree::buttonEarthHoverEnter()
{
	CInterfaceFree::MInterfaceFree.SetButtonSelect(EARTH);
}
void CInterfaceFree::buttonEarthHoverLeave()
{
	CInterfaceFree::MInterfaceFree.SetButtonSelect(NONE);
}
void CInterfaceFree::buttonMoonClick()
{
	CInterfacePlanet::MInterfacePlanet.Init("MOON", "1.622", Color(0,0,0), Color(150,150,150), Color(150,150,150), true);
}
void CInterfaceFree::buttonMoonHoverEnter()
{
	CInterfaceFree::MInterfaceFree.SetButtonSelect(MOON);
}
void CInterfaceFree::buttonMoonHoverLeave()
{
	CInterfaceFree::MInterfaceFree.SetButtonSelect(NONE);
}
void CInterfaceFree::buttonMarsClick()
{
	CInterfacePlanet::MInterfacePlanet.Init("MARS", "3.711", Color(131,85,69), Color(230,129,94), Color(206,149,120), true);
}
void CInterfaceFree::buttonMarsHoverEnter()
{
	CInterfaceFree::MInterfaceFree.SetButtonSelect(MARS);
}
void CInterfaceFree::buttonMarsHoverLeave()
{
	CInterfaceFree::MInterfaceFree.SetButtonSelect(NONE);
}
void CInterfaceFree::buttonCustomClick()
{
	CInterfacePlanet::MInterfacePlanet.Init("CUSTOM", "1", Color(0,0,0), Color(154,132,95), Color(200,200,200), false);
}
void CInterfaceFree::buttonCustomHoverEnter()
{
	CInterfaceFree::MInterfaceFree.SetButtonSelect(CUSTOM);
}
void CInterfaceFree::buttonCustomHoverLeave()
{
	CInterfaceFree::MInterfaceFree.SetButtonSelect(NONE);
}
void CInterfaceFree::buttonBackClick()
{
	CStateManager::MStateManager.ChangeState(&CStateMenu::MStateMenu);
}
void CInterfaceFree::buttonBackHoverEnter()
{
	CInterfaceFree::MInterfaceFree.SetButtonSelect(BACK);
}
void CInterfaceFree::buttonBackHoverLeave()
{
	CInterfaceFree::MInterfaceFree.SetButtonSelect(NONE);
}

void CInterfaceFree::HandleEvents(ALLEGRO_EVENT& ev)
{
	if (ev.type == ALLEGRO_EVENT_TIMER)
	{
		//Animation Counter
		if (buttonSelect == VENUS)
		{
			buttonVenusCount ++;
			if (buttonVenusCount >= 180)
				buttonVenusCount = 0;
		}
		if (buttonSelect == EARTH)
		{
			buttonEarthCount ++;
			if (buttonEarthCount >= 180)
				buttonEarthCount = 0;
		}
		if (buttonSelect == MOON)
		{
			buttonMoonCount ++;
			if (buttonMoonCount >= 180)
				buttonMoonCount = 0;
		}
		if (buttonSelect == MARS)
		{
			buttonMarsCount ++;
			if (buttonMarsCount >= 180)
				buttonMarsCount = 0;
		}
		if (buttonSelect == CUSTOM)
		{
			buttonCustomCount ++;
			if (buttonCustomCount >= 180)
				buttonCustomCount = 0;
		}
		if (buttonSelect == BACK)
		{
			if (buttonBackCount != 0)
				buttonBackCount --;
		}
		else
		{
			if (buttonBackCount != 20)
				buttonBackCount ++;
		}
	}
}

void CInterfaceFree::Render()
{
	//Button Back
	al_draw_scaled_bitmap(buttonLeft, 0, 0, 600, 100, 0-buttonBackCount*buttonWidth/20, buttonY, buttonWidth, buttonHeight, 0);

	//Animated Ship
	if (buttonSelect == VENUS)
		al_draw_scaled_rotated_bitmap(ship, 200, 200, buttonVenus->GetBounds().x+buttonVenus->GetBounds().w/2.0f, buttonVenus->GetBounds().y+buttonVenus->GetBounds().h/2.0f,
			buttonVenus->GetBounds().w/300.0f, buttonVenus->GetBounds().w/300.0f, buttonVenusCount*ALLEGRO_PI/90, 0);
	else if (buttonSelect == EARTH)
		al_draw_scaled_rotated_bitmap(ship, 200, 200, buttonEarth->GetBounds().x+buttonEarth->GetBounds().w/2.0f, buttonEarth->GetBounds().y+buttonEarth->GetBounds().h/2.0f,
			buttonEarth->GetBounds().w/300.0f, buttonEarth->GetBounds().w/300.0f, buttonEarthCount*ALLEGRO_PI/90, 0);
	else if (buttonSelect == MOON)
		al_draw_scaled_rotated_bitmap(ship, 200, 200, buttonMoon->GetBounds().x+buttonMoon->GetBounds().w/2.0f, buttonMoon->GetBounds().y+buttonMoon->GetBounds().h/2.0f,
			buttonMoon->GetBounds().w/300.0f, buttonMoon->GetBounds().w/300.0f, buttonMoonCount*ALLEGRO_PI/90, 0);
	else if (buttonSelect == MARS)
		al_draw_scaled_rotated_bitmap(ship, 200, 200, buttonMars->GetBounds().x+buttonMars->GetBounds().w/2.0f, buttonMars->GetBounds().y+buttonMars->GetBounds().h/2.0f,
			buttonMars->GetBounds().w/300.0f, buttonMars->GetBounds().w/300.0f, buttonMarsCount*ALLEGRO_PI/90, 0);
	else if (buttonSelect == CUSTOM)
		al_draw_scaled_rotated_bitmap(ship, 200, 200, buttonCustom->GetBounds().x+buttonCustom->GetBounds().w/2.0f, buttonCustom->GetBounds().y+buttonCustom->GetBounds().h/2.0f,
			buttonCustom->GetBounds().w/300.0f, buttonCustom->GetBounds().w/300.0f, buttonCustomCount*ALLEGRO_PI/90, 0);
}