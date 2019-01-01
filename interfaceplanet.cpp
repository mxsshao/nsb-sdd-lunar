#include "interfaceplanet.h"
#include "statesimulator.h"
#include "setting.h"

CInterfacePlanet CInterfacePlanet::MInterfacePlanet;

void CInterfacePlanet::Init(string planet, string gravity, Color sky, Color ground, Color rock, bool force)
{
	open = true;
	height = al_get_display_height(CStateManager::MStateManager.GetDisplay())/4.0f*3.0f;
	width = height*1.75f;

	window = new Controls::WindowControl(CStateManager::MStateManager.GetCanvas());
	window->SetTitle(planet);
	window->MakeModal(true);
	window->SetDeleteOnClose(true);
	window->SetClosable(false);
	window->SetSkin(CStateManager::MStateManager.GetDefaultSkin());
	window->onWindowClosed.Add(this, &CInterfacePlanet::WindowClose);
	window->SetSize(width, height);
	window->SetPos(al_get_display_width(CStateManager::MStateManager.GetDisplay())/2 - width/2, al_get_display_height(CStateManager::MStateManager.GetDisplay())/2 - height/2);

	buttonStart = new Controls::Button(window);
	buttonStart->SetText(L"Start Simulation");
	buttonStart->SetBounds(width-CStateManager::MStateManager.GetButtonWidth()*2-CStateManager::MStateManager.GetButtonHeight()*1.5, height-CStateManager::MStateManager.GetButtonHeight()*2.5, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	buttonStart->onPress.Add(this, &CInterfacePlanet::ButtonStartClick);

	buttonCancel = new Controls::Button(window);
	buttonCancel->SetText(L"Cancel");
	buttonCancel->SetBounds(width-CStateManager::MStateManager.GetButtonWidth()-CStateManager::MStateManager.GetButtonHeight(), height-CStateManager::MStateManager.GetButtonHeight()*2.5, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	buttonCancel->onPress.Add(this, &CInterfacePlanet::ButtonCancelClick);

	boxEnvironment = new Controls::GroupBox(window);
	boxEnvironment->SetText(L"Environment");
	boxEnvironment->SetBounds(CStateManager::MStateManager.GetButtonHeight()/2, CStateManager::MStateManager.GetButtonHeight()/2, CStateManager::MStateManager.GetButtonWidth()*2.5f, CStateManager::MStateManager.GetButtonHeight()*3.5f);

	labelAir = new Controls::Label(boxEnvironment);
	labelAir->SetText(L"Air Resistance");
	labelAir->SetAlignment( Pos::Right | Pos::CenterV );
	labelAir->SetBounds(0, CStateManager::MStateManager.GetButtonHeight()*1.5f, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	numericAir = new Controls::TextBoxNumeric(boxEnvironment);
	numericAir->SetAlignment( Pos::Right | Pos::CenterV );
	numericAir->SetBounds(CStateManager::MStateManager.GetButtonHeight()/2.0f + CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight()*1.5f, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());

	labelGravity = new Controls::Label(boxEnvironment);
	labelGravity->SetText(L"Gravity (m/s\u00B2)");
	labelGravity->SetAlignment( Pos::Right | Pos::CenterV );
	labelGravity->SetBounds(0, 0, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	numericGravity = new Controls::TextBoxNumeric(boxEnvironment);
	numericGravity->SetAlignment( Pos::Right | Pos::CenterV );
	numericGravity->SetBounds(CStateManager::MStateManager.GetButtonHeight()/2.0f + CStateManager::MStateManager.GetButtonWidth(), 0, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	numericGravity->onTextChanged.Add(this, &CInterfacePlanet::NumericGravityChanged);
	numericGravity->SetText(gravity);

	if (force)
	{
		numericGravity->SetDisabled(true);
		numericAir->SetDisabled(true);
	}
	else
	{
		numericAir->onHoverEnter.Add(this, &CInterfacePlanet::TextBoxHoverEnter);
		numericAir->onHoverLeave.Add(this, &CInterfacePlanet::TextBoxHoverLeave);
		numericGravity->onHoverEnter.Add(this, &CInterfacePlanet::TextBoxHoverEnter);
		numericGravity->onHoverLeave.Add(this, &CInterfacePlanet::TextBoxHoverLeave);
	}

	boxLander = new Controls::GroupBox(window);
	boxLander->SetText(L"Lander");
	boxLander->SetBounds(CStateManager::MStateManager.GetButtonHeight()/2, CStateManager::MStateManager.GetButtonHeight()*4.5f, CStateManager::MStateManager.GetButtonWidth()*2.5f, CStateManager::MStateManager.GetButtonHeight()*6.5f);
	
	labelShip = new Controls::Label(boxLander);
	labelShip->SetText(L"Start Height (m)");
	labelShip->SetAlignment( Pos::Right | Pos::CenterV );
	labelShip->SetBounds(0, 0, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	numericShip = new Controls::TextBoxNumeric(boxLander);
	numericShip->SetAlignment( Pos::Right | Pos::CenterV );
	numericShip->SetText("800");
	numericShip->SetBounds(CStateManager::MStateManager.GetButtonHeight()/2.0f + CStateManager::MStateManager.GetButtonWidth(), 0, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	numericShip->onTextChanged.Add(this, &CInterfacePlanet::NumericMassChanged);
	numericShip->onHoverEnter.Add(this, &CInterfacePlanet::TextBoxHoverEnter);
	numericShip->onHoverLeave.Add(this, &CInterfacePlanet::TextBoxHoverLeave);
	labelTotal = new Controls::Label(boxLander);
	labelTotal->SetText(L"Total Mass (kg)");
	labelTotal->SetAlignment( Pos::Right | Pos::CenterV );
	labelTotal->SetBounds(0, CStateManager::MStateManager.GetButtonHeight()*4.5f, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	numericTotal = new Controls::TextBoxNumeric(boxLander);
	numericTotal->SetAlignment( Pos::Right | Pos::CenterV );
	numericTotal->SetKeyboardInputEnabled(false);
	numericTotal->SetMouseInputEnabled(false);
	numericTotal->SetBounds(CStateManager::MStateManager.GetButtonHeight()/2.0f + CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight()*4.5f, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	labelMass = new Controls::Label(boxLander);
	labelMass->SetText(L"Ship Mass (kg)");
	labelMass->SetAlignment( Pos::Right | Pos::CenterV );
	labelMass->SetBounds(0, CStateManager::MStateManager.GetButtonHeight()*1.5f, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	numericMass = new Controls::TextBoxNumeric(boxLander);
	numericMass->SetAlignment( Pos::Right | Pos::CenterV );
	numericMass->SetBounds(CStateManager::MStateManager.GetButtonHeight()/2.0f + CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight()*1.5f, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	numericMass->SetText("15000");
	numericMass->onTextChanged.Add(this, &CInterfacePlanet::NumericMassChanged);
	numericMass->onHoverEnter.Add(this, &CInterfacePlanet::TextBoxHoverEnter);
	numericMass->onHoverLeave.Add(this, &CInterfacePlanet::TextBoxHoverLeave);
	labelFuel = new Controls::Label(boxLander);
	labelFuel->SetText(L"Ship Fuel (L)");
	labelFuel->SetAlignment( Pos::Right | Pos::CenterV );
	labelFuel->SetBounds(0, CStateManager::MStateManager.GetButtonHeight()*3.0f, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	numericFuel = new Controls::TextBoxNumeric(boxLander);
	numericFuel->SetAlignment( Pos::Right | Pos::CenterV );
	numericFuel->SetBounds(CStateManager::MStateManager.GetButtonHeight()/2.0f + CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight()*3.0f, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	numericFuel->onTextChanged.Add(this, &CInterfacePlanet::NumericMassChanged);
	numericFuel->onHoverEnter.Add(this, &CInterfacePlanet::TextBoxHoverEnter);
	numericFuel->onHoverLeave.Add(this, &CInterfacePlanet::TextBoxHoverLeave);
	numericFuel->SetText("1600");


	//COLORS
	boxColor = new Controls::GroupBox(window);
	boxColor->SetText("Color");
	boxColor->SetBounds(CStateManager::MStateManager.GetButtonWidth()*3.0f, CStateManager::MStateManager.GetButtonHeight()/2.0f, CStateManager::MStateManager.GetButtonWidth()*3.5f, CStateManager::MStateManager.GetButtonHeight()*10.5f);
	
	labelSky = new Controls::Label(boxColor);
	labelSky->SetText(L"Sky Color");
	labelSky->SetAlignment( Pos::Right | Pos::CenterV );
	labelSky->SetBounds(0, 0, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	colorSky = new Controls::ImagePanel(boxColor);
	colorSky->SetBounds(CStateManager::MStateManager.GetButtonHeight()/2.0f + CStateManager::MStateManager.GetButtonWidth(), 0, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	colorSky->SetDrawColor(sky);

	lrColor = new Controls::Label(boxColor);
	lrColor->SetText("R: ");
	lrColor->SetAlignment( Pos::Right | Pos::CenterV );
	lrColor->SetBounds(0, CStateManager::MStateManager.GetButtonHeight()*1.5f, CStateManager::MStateManager.GetButtonWidth()/4.0f, CStateManager::MStateManager.GetButtonHeight());
	rColor = new Controls::TextBoxNumeric(boxColor);
	rColor->SetAlignment( Pos::Right | Pos::CenterV );
	rColor->SetBounds(CStateManager::MStateManager.GetButtonWidth()/4.0f, CStateManager::MStateManager.GetButtonHeight()*1.5f, CStateManager::MStateManager.GetButtonWidth()/2.0f, CStateManager::MStateManager.GetButtonHeight());
	rColor->SetText(to_string(sky.r));
	rColor->onTextChanged.Add(this, &CInterfacePlanet::ColorSkyChanged);

	lgColor = new Controls::Label(boxColor);
	lgColor->SetText("G: ");
	lgColor->SetAlignment( Pos::Right | Pos::CenterV );
	lgColor->SetBounds(CStateManager::MStateManager.GetButtonWidth()*0.75f, CStateManager::MStateManager.GetButtonHeight()*1.5f, CStateManager::MStateManager.GetButtonWidth()/4.0f, CStateManager::MStateManager.GetButtonHeight());
	gColor = new Controls::TextBoxNumeric(boxColor);
	gColor->SetAlignment( Pos::Right | Pos::CenterV );
	gColor->SetBounds(CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight()*1.5f, CStateManager::MStateManager.GetButtonWidth()/2.0f, CStateManager::MStateManager.GetButtonHeight());
	gColor->SetText(to_string(sky.g));
	gColor->onTextChanged.Add(this, &CInterfacePlanet::ColorSkyChanged);

	lbColor = new Controls::Label(boxColor);
	lbColor->SetText("B: ");
	lbColor->SetAlignment( Pos::Right | Pos::CenterV );
	lbColor->SetBounds(CStateManager::MStateManager.GetButtonWidth()*1.5f, CStateManager::MStateManager.GetButtonHeight()*1.5f, CStateManager::MStateManager.GetButtonWidth()/4.0f, CStateManager::MStateManager.GetButtonHeight());
	bColor = new Controls::TextBoxNumeric(boxColor);
	bColor->SetAlignment( Pos::Right | Pos::CenterV );
	bColor->SetBounds(CStateManager::MStateManager.GetButtonWidth()*1.75f, CStateManager::MStateManager.GetButtonHeight()*1.5f, CStateManager::MStateManager.GetButtonWidth()/2.0f, CStateManager::MStateManager.GetButtonHeight());
	bColor->SetText(to_string(sky.b));
	bColor->onTextChanged.Add(this, &CInterfacePlanet::ColorSkyChanged);

	//
	labelGround = new Controls::Label(boxColor);
	labelGround->SetText(L"Ground Color");
	labelGround->SetAlignment( Pos::Right | Pos::CenterV );
	labelGround->SetBounds(0, CStateManager::MStateManager.GetButtonHeight()*3.0f, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	colorGround = new Controls::ImagePanel(boxColor);
	colorGround->SetBounds(CStateManager::MStateManager.GetButtonHeight()/2.0f + CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight()*3.0f, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	colorGround->SetDrawColor(ground);

	lrGround = new Controls::Label(boxColor);
	lrGround->SetText("R: ");
	lrGround->SetAlignment( Pos::Right | Pos::CenterV );
	lrGround->SetBounds(0, CStateManager::MStateManager.GetButtonHeight()*4.5f, CStateManager::MStateManager.GetButtonWidth()/4.0f, CStateManager::MStateManager.GetButtonHeight());
	rGround = new Controls::TextBoxNumeric(boxColor);
	rGround->SetAlignment( Pos::Right | Pos::CenterV );
	rGround->SetBounds(CStateManager::MStateManager.GetButtonWidth()/4.0f, CStateManager::MStateManager.GetButtonHeight()*4.5f, CStateManager::MStateManager.GetButtonWidth()/2.0f, CStateManager::MStateManager.GetButtonHeight());
	rGround->SetText(to_string(ground.r));
	rGround->onTextChanged.Add(this, &CInterfacePlanet::ColorGroundChanged);

	lgGround = new Controls::Label(boxColor);
	lgGround->SetText("G: ");
	lgGround->SetAlignment( Pos::Right | Pos::CenterV );
	lgGround->SetBounds(CStateManager::MStateManager.GetButtonWidth()*0.75f, CStateManager::MStateManager.GetButtonHeight()*4.5f, CStateManager::MStateManager.GetButtonWidth()/4.0f, CStateManager::MStateManager.GetButtonHeight());
	gGround = new Controls::TextBoxNumeric(boxColor);
	gGround->SetAlignment( Pos::Right | Pos::CenterV );
	gGround->SetBounds(CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight()*4.5f, CStateManager::MStateManager.GetButtonWidth()/2.0f, CStateManager::MStateManager.GetButtonHeight());
	gGround->SetText(to_string(ground.g));
	gGround->onTextChanged.Add(this, &CInterfacePlanet::ColorGroundChanged);

	lbGround = new Controls::Label(boxColor);
	lbGround->SetText("B: ");
	lbGround->SetAlignment( Pos::Right | Pos::CenterV );
	lbGround->SetBounds(CStateManager::MStateManager.GetButtonWidth()*1.5f, CStateManager::MStateManager.GetButtonHeight()*4.5f, CStateManager::MStateManager.GetButtonWidth()/4.0f, CStateManager::MStateManager.GetButtonHeight());
	bGround = new Controls::TextBoxNumeric(boxColor);
	bGround->SetAlignment( Pos::Right | Pos::CenterV );
	bGround->SetBounds(CStateManager::MStateManager.GetButtonWidth()*1.75f, CStateManager::MStateManager.GetButtonHeight()*4.5f, CStateManager::MStateManager.GetButtonWidth()/2.0f, CStateManager::MStateManager.GetButtonHeight());
	bGround->SetText(to_string(ground.b));
	bGround->onTextChanged.Add(this, &CInterfacePlanet::ColorGroundChanged);
	

	//
	labelRock = new Controls::Label(boxColor);
	labelRock->SetText(L"Rock Color");
	labelRock->SetAlignment( Pos::Right | Pos::CenterV );
	labelRock->SetBounds(0, CStateManager::MStateManager.GetButtonHeight()*6.0f, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	colorRock = new Controls::ImagePanel(boxColor);
	colorRock->SetBounds(CStateManager::MStateManager.GetButtonHeight()/2.0f + CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight()*6.0f, CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight());
	colorRock->SetDrawColor(rock);

	lrRock = new Controls::Label(boxColor);
	lrRock->SetText("R: ");
	lrRock->SetAlignment( Pos::Right | Pos::CenterV );
	lrRock->SetBounds(0, CStateManager::MStateManager.GetButtonHeight()*7.5f, CStateManager::MStateManager.GetButtonWidth()/4.0f, CStateManager::MStateManager.GetButtonHeight());
	rRock = new Controls::TextBoxNumeric(boxColor);
	rRock->SetAlignment( Pos::Right | Pos::CenterV );
	rRock->SetBounds(CStateManager::MStateManager.GetButtonWidth()/4.0f, CStateManager::MStateManager.GetButtonHeight()*7.5f, CStateManager::MStateManager.GetButtonWidth()/2.0f, CStateManager::MStateManager.GetButtonHeight());
	rRock->SetText(to_string(rock.r));
	rRock->onTextChanged.Add(this, &CInterfacePlanet::ColorRockChanged);

	lgRock = new Controls::Label(boxColor);
	lgRock->SetText("G: ");
	lgRock->SetAlignment( Pos::Right | Pos::CenterV );
	lgRock->SetBounds(CStateManager::MStateManager.GetButtonWidth()*0.75f, CStateManager::MStateManager.GetButtonHeight()*7.5f, CStateManager::MStateManager.GetButtonWidth()/4.0f, CStateManager::MStateManager.GetButtonHeight());
	gRock = new Controls::TextBoxNumeric(boxColor);
	gRock->SetAlignment( Pos::Right | Pos::CenterV );
	gRock->SetBounds(CStateManager::MStateManager.GetButtonWidth(), CStateManager::MStateManager.GetButtonHeight()*7.5f, CStateManager::MStateManager.GetButtonWidth()/2.0f, CStateManager::MStateManager.GetButtonHeight());
	gRock->SetText(to_string(rock.g));
	gRock->onTextChanged.Add(this, &CInterfacePlanet::ColorRockChanged);

	lbRock = new Controls::Label(boxColor);
	lbRock->SetText("B: ");
	lbRock->SetAlignment( Pos::Right | Pos::CenterV );
	lbRock->SetBounds(CStateManager::MStateManager.GetButtonWidth()*1.5f, CStateManager::MStateManager.GetButtonHeight()*7.5f, CStateManager::MStateManager.GetButtonWidth()/4.0f, CStateManager::MStateManager.GetButtonHeight());
	bRock = new Controls::TextBoxNumeric(boxColor);
	bRock->SetAlignment( Pos::Right | Pos::CenterV );
	bRock->SetBounds(CStateManager::MStateManager.GetButtonWidth()*1.75f, CStateManager::MStateManager.GetButtonHeight()*7.5f, CStateManager::MStateManager.GetButtonWidth()/2.0f, CStateManager::MStateManager.GetButtonHeight());
	bRock->SetText(to_string(rock.b));
	bRock->onTextChanged.Add(this, &CInterfacePlanet::ColorRockChanged);

	if (force)
	{
		rColor->SetDisabled(true);
		gColor->SetDisabled(true);
		bColor->SetDisabled(true);
		rGround->SetDisabled(true);
		gGround->SetDisabled(true);
		bGround->SetDisabled(true);
		rRock->SetDisabled(true);
		gRock->SetDisabled(true);
		bRock->SetDisabled(true);
	}
	else
	{
		rColor->onHoverEnter.Add(this, &CInterfacePlanet::TextBoxHoverEnter);
		rColor->onHoverLeave.Add(this, &CInterfacePlanet::TextBoxHoverLeave);
		gColor->onHoverEnter.Add(this, &CInterfacePlanet::TextBoxHoverEnter);
		gColor->onHoverLeave.Add(this, &CInterfacePlanet::TextBoxHoverLeave);
		bColor->onHoverEnter.Add(this, &CInterfacePlanet::TextBoxHoverEnter);
		bColor->onHoverLeave.Add(this, &CInterfacePlanet::TextBoxHoverLeave);
		rGround->onHoverEnter.Add(this, &CInterfacePlanet::TextBoxHoverEnter);
		rGround->onHoverLeave.Add(this, &CInterfacePlanet::TextBoxHoverLeave);
		gGround->onHoverEnter.Add(this, &CInterfacePlanet::TextBoxHoverEnter);
		gGround->onHoverLeave.Add(this, &CInterfacePlanet::TextBoxHoverLeave);
		bGround->onHoverEnter.Add(this, &CInterfacePlanet::TextBoxHoverEnter);
		bGround->onHoverLeave.Add(this, &CInterfacePlanet::TextBoxHoverLeave);
		rRock->onHoverEnter.Add(this, &CInterfacePlanet::TextBoxHoverEnter);
		rRock->onHoverLeave.Add(this, &CInterfacePlanet::TextBoxHoverLeave);
		gRock->onHoverEnter.Add(this, &CInterfacePlanet::TextBoxHoverEnter);
		gRock->onHoverLeave.Add(this, &CInterfacePlanet::TextBoxHoverLeave);
		bRock->onHoverEnter.Add(this, &CInterfacePlanet::TextBoxHoverEnter);
		bRock->onHoverLeave.Add(this, &CInterfacePlanet::TextBoxHoverLeave);
	}
}

void CInterfacePlanet::ButtonStartClick()
{
	CSetting::MSetting.SetSimulator(numericGravity->GetFloatFromText(),
		numericAir->GetFloatFromText(),
		numericShip->GetFloatFromText(),
		numericMass->GetFloatFromText(),
		numericFuel->GetFloatFromText(),
		al_map_rgb(rColor->GetFloatFromText(), gColor->GetFloatFromText(), bColor->GetFloatFromText()),
		al_map_rgb(rGround->GetFloatFromText(), gGround->GetFloatFromText(), bGround->GetFloatFromText()),
		al_map_rgb(rRock->GetFloatFromText(), gRock->GetFloatFromText(), bRock->GetFloatFromText()));
	
	CInterfacePlanet::MInterfacePlanet.window->CloseButtonPressed();
	CStateManager::MStateManager.ChangeState(&CStateSimulator::MStateSimulator);
}

void CInterfacePlanet::ButtonCancelClick()
{
	CInterfacePlanet::MInterfacePlanet.window->CloseButtonPressed();
}

void CInterfacePlanet::TextBoxHoverEnter()
{
	al_set_mouse_cursor(CStateManager::MStateManager.GetDisplay(), CStateManager::MStateManager.GetText());
}
void CInterfacePlanet::TextBoxHoverLeave()
{
	al_set_mouse_cursor(CStateManager::MStateManager.GetDisplay(), CStateManager::MStateManager.GetCursor());
}

void CInterfacePlanet::NumericMassChanged()
{
	if (CInterfacePlanet::MInterfacePlanet.numericShip->GetFloatFromText() >= 98000.0f)
		CInterfacePlanet::MInterfacePlanet.numericShip->SetText(L"98000");
	else if (CInterfacePlanet::MInterfacePlanet.numericShip->GetFloatFromText() <= 0.0f)
		CInterfacePlanet::MInterfacePlanet.numericShip->SetText("0");

	if (CInterfacePlanet::MInterfacePlanet.numericMass->GetFloatFromText() >= 50000.0f)
		CInterfacePlanet::MInterfacePlanet.numericMass->SetText(L"50000");
	else if (CInterfacePlanet::MInterfacePlanet.numericMass->GetFloatFromText() <= 0.0f)
		CInterfacePlanet::MInterfacePlanet.numericMass->SetText("0");

	if (CInterfacePlanet::MInterfacePlanet.numericFuel->GetFloatFromText() >= 6000.0f)
		CInterfacePlanet::MInterfacePlanet.numericFuel->SetText(L"6000");
	else if (CInterfacePlanet::MInterfacePlanet.numericFuel->GetFloatFromText() <= 0.0f)
		CInterfacePlanet::MInterfacePlanet.numericFuel->SetText("0");

	CInterfacePlanet::MInterfacePlanet.numericTotal->SetText(to_string(CInterfacePlanet::MInterfacePlanet.numericMass->GetFloatFromText() + CInterfacePlanet::MInterfacePlanet.numericFuel->GetFloatFromText()*2.236f));
}

void CInterfacePlanet::NumericGravityChanged()
{
	if (CInterfacePlanet::MInterfacePlanet.numericGravity->GetFloatFromText() >= 10.0f)
		CInterfacePlanet::MInterfacePlanet.numericGravity->SetText("10");
	else if (CInterfacePlanet::MInterfacePlanet.numericGravity->GetFloatFromText() <= 0.0f)
		CInterfacePlanet::MInterfacePlanet.numericGravity->SetText("0");

	CInterfacePlanet::MInterfacePlanet.numericAir->SetText(to_string(CInterfacePlanet::MInterfacePlanet.numericGravity->GetFloatFromText()*0.08f));
}

void CInterfacePlanet::ColorSkyChanged()
{
	if (CInterfacePlanet::MInterfacePlanet.rColor->GetFloatFromText() < 0)
		CInterfacePlanet::MInterfacePlanet.rColor->SetText("0");
	else if (CInterfacePlanet::MInterfacePlanet.rColor->GetFloatFromText() > 255)
		CInterfacePlanet::MInterfacePlanet.rColor->SetText("255");
	if (CInterfacePlanet::MInterfacePlanet.gColor->GetFloatFromText() < 0)
		CInterfacePlanet::MInterfacePlanet.gColor->SetText("0");
	else if (CInterfacePlanet::MInterfacePlanet.gColor->GetFloatFromText() > 255)
		CInterfacePlanet::MInterfacePlanet.gColor->SetText("255");
	if (CInterfacePlanet::MInterfacePlanet.bColor->GetFloatFromText() < 0)
		CInterfacePlanet::MInterfacePlanet.bColor->SetText("0");
	else if (CInterfacePlanet::MInterfacePlanet.bColor->GetFloatFromText() > 255)
		CInterfacePlanet::MInterfacePlanet.bColor->SetText("255");

	CInterfacePlanet::MInterfacePlanet.colorSky->SetDrawColor(Color(
		CInterfacePlanet::MInterfacePlanet.rColor->GetFloatFromText(),
		CInterfacePlanet::MInterfacePlanet.gColor->GetFloatFromText(),
		CInterfacePlanet::MInterfacePlanet.bColor->GetFloatFromText(),
		255));
}

void CInterfacePlanet::ColorGroundChanged()
{
	if (CInterfacePlanet::MInterfacePlanet.rGround->GetFloatFromText() < 0)
		CInterfacePlanet::MInterfacePlanet.rGround->SetText("0");
	else if (CInterfacePlanet::MInterfacePlanet.rGround->GetFloatFromText() > 255)
		CInterfacePlanet::MInterfacePlanet.rGround->SetText("255");
	if (CInterfacePlanet::MInterfacePlanet.gGround->GetFloatFromText() < 0)
		CInterfacePlanet::MInterfacePlanet.gGround->SetText("0");
	else if (CInterfacePlanet::MInterfacePlanet.gGround->GetFloatFromText() > 255)
		CInterfacePlanet::MInterfacePlanet.gGround->SetText("255");
	if (CInterfacePlanet::MInterfacePlanet.bGround->GetFloatFromText() < 0)
		CInterfacePlanet::MInterfacePlanet.bGround->SetText("0");
	else if (CInterfacePlanet::MInterfacePlanet.bGround->GetFloatFromText() > 255)
		CInterfacePlanet::MInterfacePlanet.bGround->SetText("255");

	CInterfacePlanet::MInterfacePlanet.colorGround->SetDrawColor(Color(
		CInterfacePlanet::MInterfacePlanet.rGround->GetFloatFromText(),
		CInterfacePlanet::MInterfacePlanet.gGround->GetFloatFromText(),
		CInterfacePlanet::MInterfacePlanet.bGround->GetFloatFromText(),
		255));
}

void CInterfacePlanet::ColorRockChanged()
{
	if (CInterfacePlanet::MInterfacePlanet.rRock->GetFloatFromText() < 0)
		CInterfacePlanet::MInterfacePlanet.rRock->SetText("0");
	else if (CInterfacePlanet::MInterfacePlanet.rRock->GetFloatFromText() > 255)
		CInterfacePlanet::MInterfacePlanet.rRock->SetText("255");
	if (CInterfacePlanet::MInterfacePlanet.gRock->GetFloatFromText() < 0)
		CInterfacePlanet::MInterfacePlanet.gRock->SetText("0");
	else if (CInterfacePlanet::MInterfacePlanet.gRock->GetFloatFromText() > 255)
		CInterfacePlanet::MInterfacePlanet.gRock->SetText("255");
	if (CInterfacePlanet::MInterfacePlanet.bRock->GetFloatFromText() < 0)
		CInterfacePlanet::MInterfacePlanet.bRock->SetText("0");
	else if (CInterfacePlanet::MInterfacePlanet.bRock->GetFloatFromText() > 255)
		CInterfacePlanet::MInterfacePlanet.bRock->SetText("255");

	CInterfacePlanet::MInterfacePlanet.colorRock->SetDrawColor(Color(
		CInterfacePlanet::MInterfacePlanet.rRock->GetFloatFromText(),
		CInterfacePlanet::MInterfacePlanet.gRock->GetFloatFromText(),
		CInterfacePlanet::MInterfacePlanet.bRock->GetFloatFromText(),
		255));
}

void CInterfacePlanet::WindowClose()
{
	CInterfacePlanet::MInterfacePlanet.SetOpen(false);
}