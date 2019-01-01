#pragma once

#include "global.h"
#include "statefree.h"

class CInterfacePlanet : public Event::Handler
{
protected:
	CInterfacePlanet() {};

private:
	Controls::WindowControl* window;

	Controls::Button* buttonStart;
	Controls::Button* buttonCancel;

	Controls::GroupBox* boxEnvironment;
	Controls::Label* labelGravity;
	Controls::TextBoxNumeric* numericGravity;
	Controls::Label* labelAir;
	Controls::TextBoxNumeric* numericAir;

	Controls::GroupBox* boxLander;
	Controls::Label* labelShip;
	Controls::TextBoxNumeric* numericShip;
	Controls::Label* labelMass;
	Controls::TextBoxNumeric* numericMass;
	Controls::Label* labelFuel;
	Controls::TextBoxNumeric* numericFuel;
	Controls::Label* labelTotal;
	Controls::TextBoxNumeric* numericTotal;

	Controls::GroupBox* boxColor;

	Controls::Label* labelSky;
	Controls::Label* lrColor;
	Controls::Label* lgColor;
	Controls::Label* lbColor;
	Controls::TextBoxNumeric* rColor;
	Controls::TextBoxNumeric* gColor;
	Controls::TextBoxNumeric* bColor;
	Controls::ImagePanel* colorSky;

	Controls::Label* labelGround;
	Controls::Label* lrGround;
	Controls::Label* lgGround;
	Controls::Label* lbGround;
	Controls::TextBoxNumeric* rGround;
	Controls::TextBoxNumeric* gGround;
	Controls::TextBoxNumeric* bGround;
	Controls::ImagePanel* colorGround;

	Controls::Label* labelRock;
	Controls::Label* lrRock;
	Controls::Label* lgRock;
	Controls::Label* lbRock;
	Controls::TextBoxNumeric* rRock;
	Controls::TextBoxNumeric* gRock;
	Controls::TextBoxNumeric* bRock;
	Controls::ImagePanel* colorRock;

	float width;
	float height;

	bool open;

	void ButtonStartClick();
	void ButtonCancelClick();
	void TextBoxHoverEnter();
	void TextBoxHoverLeave();
	void NumericMassChanged();
	void NumericGravityChanged();
	void ColorSkyChanged();
	void ColorGroundChanged();
	void ColorRockChanged();
	void WindowClose();

public:
	static CInterfacePlanet MInterfacePlanet;
	void Init(string planet, string gravity, Color sky, Color ground, Color rock, bool force);

	bool GetOpen() {return open;};
	void SetOpen(bool o) {open = o;};
	Controls::WindowControl* GetWindow() {return window;};
};