#include "interfacesimulator.h"
#include "interfacedialog.h"
#include "displayPFD.h"
#include "displayECAM.h"
#include "statemenu.h"

CInterfaceSimulator CInterfaceSimulator::MInterfaceSimulator;

void CInterfaceSimulator::Load()
{
	CDisplayPFD::MDisplayPFD.Load();
	CDisplayECAM::MDisplayECAM.Load();

	font = al_load_font("font/LiquidCrystal.ttf", 24, 0);

	panel = al_load_bitmap("simulator/panel.png");
	panelBG = al_load_bitmap("simulator/panelbg.png");
	overhead = al_load_bitmap("simulator/overhead.png");

	nosmoking = al_load_bitmap("simulator/nosmoking.png");
	seatbelt = al_load_bitmap("simulator/seatbelt.png");

	warningEngine = al_load_bitmap("simulator/warningengine.png");
	warning200 = al_load_bitmap("simulator/warning200.png");
	warning100 = al_load_bitmap("simulator/warning100.png");
	warning50 = al_load_bitmap("simulator/warning50.png");
	warning40 = al_load_bitmap("simulator/warning40.png");
	warning30 = al_load_bitmap("simulator/warning30.png");
	warning20 = al_load_bitmap("simulator/warning20.png");
	warning10 = al_load_bitmap("simulator/warning10.png");

	sound200 = al_load_sample("simulator/sound200.ogg");
	sound100 = al_load_sample("simulator/sound100.ogg");
	sound50 = al_load_sample("simulator/sound50.ogg");
	sound40 = al_load_sample("simulator/sound40.ogg");
	sound30 = al_load_sample("simulator/sound30.ogg");
	sound20 = al_load_sample("simulator/sound20.ogg");
	sound10 = al_load_sample("simulator/sound10.ogg");
	sound5 = al_load_sample("simulator/sound5.ogg");

	throttle = al_load_bitmap("simulator/throttle.png");

	//BUTTONS
	CStateManager::MStateManager.GetCanvas()->SetSkin(CStateManager::MStateManager.GetMenuSkin());

	switchSound = al_load_sample("simulator/switch.ogg");
	switchUp = al_load_bitmap("simulator/switchup.png");
	switchDown = al_load_bitmap("simulator/switchdown.png");

	buttonNoSmoking = new Controls::Button(CStateManager::MStateManager.GetCanvas());
	buttonNoSmoking->SetIsToggle(true);
	buttonNoSmoking->onToggleOn.Add(this, &CInterfaceSimulator::ButtonSwitchChanged);
	buttonNoSmoking->onHoverEnter.Add(this, &CInterfaceSimulator::ButtonHoverEnter);
	buttonNoSmoking->onHoverLeave.Add(this, &CInterfaceSimulator::ButtonHoverLeave);
	buttonSeatbelt = new Controls::Button(CStateManager::MStateManager.GetCanvas());
	buttonSeatbelt->SetIsToggle(true);
	buttonSeatbelt->onToggleOn.Add(this, &CInterfaceSimulator::ButtonSwitchChanged);
	buttonSeatbelt->onHoverEnter.Add(this, &CInterfaceSimulator::ButtonHoverEnter);
	buttonSeatbelt->onHoverLeave.Add(this, &CInterfaceSimulator::ButtonHoverLeave);

	imageWarning = al_load_bitmap("simulator/buttonwarning.png");
	imageWarningOn = al_load_bitmap("simulator/buttonwarningon.png");
	buttonWarning = new Controls::Button(CStateManager::MStateManager.GetCanvas());
	buttonWarning->SetIsToggle(true);
	buttonWarning->SetMouseInputEnabled(false);
	//buttonWarning->onToggle.Add(this, &CInterfaceSimulator::ButtonWarningChanged);
	buttonWarning->onHoverEnter.Add(this, &CInterfaceSimulator::ButtonHoverEnter);
	buttonWarning->onHoverLeave.Add(this, &CInterfaceSimulator::ButtonHoverLeave);
	imageCaution = al_load_bitmap("simulator/buttoncaution.png");
	imageCautionOn = al_load_bitmap("simulator/buttoncautionon.png");
	buttonCaution = new Controls::Button(CStateManager::MStateManager.GetCanvas());
	buttonCaution->SetIsToggle(true);
	buttonCaution->SetMouseInputEnabled(false);
	//buttonCaution->onToggle.Add(this, &CInterfaceSimulator::ButtonCautionChanged);
	buttonCaution->onHoverEnter.Add(this, &CInterfaceSimulator::ButtonHoverEnter);
	buttonCaution->onHoverLeave.Add(this, &CInterfaceSimulator::ButtonHoverLeave);
	imageAvion = al_load_bitmap("simulator/buttonavion.png");
	imageAvionOn = al_load_bitmap("simulator/buttonavionon.png");
	buttonAvion = new Controls::Button(CStateManager::MStateManager.GetCanvas());
	buttonAvion->SetIsToggle(true);
	buttonAvion->onToggle.Add(this, &CInterfaceSimulator::ButtonAvionChanged);
	buttonAvion->onHoverEnter.Add(this, &CInterfaceSimulator::ButtonHoverEnter);
	buttonAvion->onHoverLeave.Add(this, &CInterfaceSimulator::ButtonHoverLeave);

	victory = al_load_sample("simulator/victory.ogg");
}

void CInterfaceSimulator::Init()
{
	al_convert_bitmaps();

	CDisplayPFD::MDisplayPFD.Init();
	CDisplayECAM::MDisplayECAM.Init();

	w200 = false;
	w100 = false;
	w50 = false;
	w40 = false;
	w30 = false;
	w20 = false;
	w10 = false;
	w5 = false;

	fuelWarning = false;
	fuelEmpty = false;
	engineFailure = false;
	bankAngle = false;
	overSpeed = false;
	sinkRate = false;
}

void CInterfaceSimulator::Resize()
{
	width = al_get_display_width(CStateManager::MStateManager.GetDisplay());
	height = width/35.0f*8.0f;
	y = al_get_display_height(CStateManager::MStateManager.GetDisplay()) - height;

	indicatorWidth = width/200.0f*21.0f;
	indicatorHeight = indicatorWidth/147.0f*40.0f;
	warningWidth = indicatorHeight/10.0f*57.0f;
	indicatorX = width/20.0f*13.0f;
	indicatorY = height/40;

	CDisplayPFD::MDisplayPFD.Resize();
	CDisplayECAM::MDisplayECAM.Resize();

	switchWidth = width/140.0f*3.0f;
	switchHeight = switchWidth/5.0f*9.0f;

	buttonNoSmoking->SetBounds(indicatorX+indicatorWidth+switchWidth/2, indicatorY, switchWidth, switchHeight);
	buttonSeatbelt->SetBounds(indicatorX+indicatorWidth+switchWidth*1.5f, indicatorY, switchWidth, switchHeight);

	buttonWarning->SetBounds(indicatorX+indicatorWidth-indicatorHeight*2.0f-switchWidth, indicatorY+indicatorWidth/3.0f, indicatorHeight, indicatorHeight);
	buttonCaution->SetBounds(indicatorX+indicatorWidth-indicatorHeight-switchWidth/2.0f, indicatorY+indicatorWidth/3.0f, indicatorHeight, indicatorHeight);
	buttonAvion->SetBounds(indicatorX+indicatorWidth+warningWidth+switchWidth/2.0f, indicatorY+indicatorWidth/3.0f, indicatorHeight, indicatorHeight);
}

void CInterfaceSimulator::Cleanup()
{
	al_destroy_font(font);

	al_destroy_bitmap(panel);
	al_destroy_bitmap(panelBG);
	al_destroy_bitmap(overhead);
	al_destroy_bitmap(nosmoking);
	al_destroy_bitmap(seatbelt);
	al_destroy_bitmap(throttle);

	al_destroy_bitmap(switchUp);
	al_destroy_bitmap(switchDown);
	al_destroy_sample(switchSound);

	buttonNoSmoking->DelayedDelete();
	buttonSeatbelt->DelayedDelete();

	al_destroy_bitmap(warningEngine);
	al_destroy_bitmap(warning200);
	al_destroy_bitmap(warning100);
	al_destroy_bitmap(warning50);
	al_destroy_bitmap(warning40);
	al_destroy_bitmap(warning30);
	al_destroy_bitmap(warning20);
	al_destroy_bitmap(warning10);
	al_destroy_sample(sound200);
	al_destroy_sample(sound100);
	al_destroy_sample(sound50);
	al_destroy_sample(sound40);
	al_destroy_sample(sound30);
	al_destroy_sample(sound20);
	al_destroy_sample(sound10);
	al_destroy_sample(sound5);

	al_destroy_bitmap(imageWarning);
	al_destroy_bitmap(imageWarningOn);
	al_destroy_bitmap(imageCaution);
	al_destroy_bitmap(imageCautionOn);
	al_destroy_bitmap(imageAvion);
	al_destroy_bitmap(imageAvionOn);

	al_destroy_sample(victory);

	buttonWarning->DelayedDelete();
	buttonCaution->DelayedDelete();
	buttonAvion->DelayedDelete();

	CDisplayPFD::MDisplayPFD.Cleanup();
	CDisplayECAM::MDisplayECAM.Cleanup();
}

void CInterfaceSimulator::ButtonHoverEnter()
{
	al_set_mouse_cursor(CStateManager::MStateManager.GetDisplay(), CStateManager::MStateManager.GetHand());
}
void CInterfaceSimulator::ButtonHoverLeave()
{
	al_set_mouse_cursor(CStateManager::MStateManager.GetDisplay(), CStateManager::MStateManager.GetCursor());
}

void CInterfaceSimulator::ButtonSwitchChanged()
{
	al_play_sample(CInterfaceSimulator::MInterfaceSimulator.switchSound, 0.6f, 1, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void CInterfaceSimulator::ButtonAvionChanged()
{
	CStateSimulator::MStateSimulator.SetAvionics(CInterfaceSimulator::MInterfaceSimulator.buttonAvion->GetToggleState());
}

void CInterfaceSimulator::HandleEvents()
{
	//WARNINGS
	if (CStateSimulator::MStateSimulator.GetAltitude() <= 200)
	{
		if (!w200)
		{
			w200 = true;
			al_play_sample(sound200, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
	}
	else
		w200 = false;
	if (CStateSimulator::MStateSimulator.GetAltitude() <= 100)
	{
		if (!w100)
		{
			w100 = true;
			al_play_sample(sound100, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
	}
	else
		w100 = false;
	if (CStateSimulator::MStateSimulator.GetAltitude() <= 50)
	{
		if (!w50)
		{
			w50 = true;
			al_play_sample(sound50, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
	}
	else
		w50 = false;
	if (CStateSimulator::MStateSimulator.GetAltitude() <= 40)
	{
		if (!w40)
		{
			w40 = true;
			al_play_sample(sound40, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
	}
	else
		w40 = false;
	if (CStateSimulator::MStateSimulator.GetAltitude() <= 30)
	{
		if (!w30)
		{
			w30 = true;
			al_play_sample(sound30, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
	}
	else
		w30 = false;
	if (CStateSimulator::MStateSimulator.GetAltitude() <= 20)
	{
		if (!w20)
		{
			w20 = true;
			al_play_sample(sound20, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
	}
	else
		w20 = false;
	if (CStateSimulator::MStateSimulator.GetAltitude() <= 10)
	{
		if (!w10)
		{
			w10 = true;
			al_play_sample(sound10, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
	}
	else
		w10 = false;
	if (CStateSimulator::MStateSimulator.GetAltitude() <= 5)
	{
		if (!w5)
		{
			w5 = true;
			al_play_sample(sound5, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
	}
	else
		w5 = false;

	if (CStateSimulator::MStateSimulator.GetFuel()/CStateSimulator::MStateSimulator.GetMaxFuel() <= 0.2f)
		if (!fuelWarning)
		{
			fuelWarning = true;
			buttonCaution->SetToggleState(true);
		}
	if (CStateSimulator::MStateSimulator.GetFuel() <= 0)
		if (!fuelEmpty)
		{
			fuelEmpty = true;
			buttonWarning->SetToggleState(true);
		}

}

void CInterfaceSimulator::Pause()
{
	CInterfaceDialog::MInterfaceDialog.Init("Simulation Paused", "Do you want to exit the simulation?", "", EXIT);
	CInterfaceDialog::MInterfaceDialog.GetNo()->onPress.Add(this, &CInterfaceSimulator::Exit);
}

void CInterfaceSimulator::Crash()
{
	CInterfaceDialog::MInterfaceDialog.Init("You Failed", "Select to retry or return to menu", "", CRASH);
	CInterfaceDialog::MInterfaceDialog.GetYes()->onPress.Add(this, &CInterfaceSimulator::Retry);
	CInterfaceDialog::MInterfaceDialog.GetNo()->onPress.Add(this, &CInterfaceSimulator::Exit);
}

void CInterfaceSimulator::Landed()
{
	al_play_sample(victory, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

	CInterfaceDialog::MInterfaceDialog.Init("You Landed", "Select to fly again or return to menu", "", VICTORY);
	CInterfaceDialog::MInterfaceDialog.GetYes()->onPress.Add(this, &CInterfaceSimulator::Retry);
	CInterfaceDialog::MInterfaceDialog.GetNo()->onPress.Add(this, &CInterfaceSimulator::Exit);
}

void CInterfaceSimulator::Retry()
{
	CStateManager::MStateManager.ChangeState(&CStateSimulator::MStateSimulator);
}

void CInterfaceSimulator::Exit()
{
	CStateManager::MStateManager.ChangeState(&CStateMenu::MStateMenu);
}

void CInterfaceSimulator::Render()
{
	//al_clear_to_color(al_map_rgb(176, 226, 225));

	al_draw_scaled_bitmap(overhead, 0, 0, 2800, 640, 0, 0, width, height, 0);
	if (buttonNoSmoking->GetToggleState())
	{
		al_draw_scaled_bitmap(nosmoking, 0, 0, 294, 80, indicatorX, indicatorY, indicatorWidth, indicatorHeight, 0);
		al_draw_scaled_bitmap(switchDown, 0, 0, 80, 144, buttonNoSmoking->GetBounds().x, buttonNoSmoking->GetBounds().y, switchWidth, switchHeight, 0);
	}
	else
		al_draw_scaled_bitmap(switchUp, 0, 0, 80, 144, buttonNoSmoking->GetBounds().x, buttonNoSmoking->GetBounds().y, switchWidth, switchHeight, 0);

	if (buttonSeatbelt->GetToggleState())
	{
		al_draw_scaled_bitmap(seatbelt, 0, 0, 294, 80, indicatorX, indicatorY, indicatorWidth, indicatorHeight, 0);
		al_draw_scaled_bitmap(switchDown, 0, 0, 80, 144, buttonSeatbelt->GetBounds().x, buttonSeatbelt->GetBounds().y, switchWidth, switchHeight, 0);
	}
	else
		al_draw_scaled_bitmap(switchUp, 0, 0, 80, 144, buttonSeatbelt->GetBounds().x, buttonSeatbelt->GetBounds().y, switchWidth, switchHeight, 0);

	if (w200)
		al_draw_scaled_bitmap(warning200, 0, 0, 456, 80, indicatorX+indicatorWidth, indicatorY+indicatorWidth/3.0f, warningWidth, indicatorHeight, 0);
	if (w100)
		al_draw_scaled_bitmap(warning100, 0, 0, 456, 80, indicatorX+indicatorWidth, indicatorY+indicatorWidth/3.0f, warningWidth, indicatorHeight, 0);
	if (w50)
		al_draw_scaled_bitmap(warning50, 0, 0, 456, 80, indicatorX+indicatorWidth, indicatorY+indicatorWidth/3.0f, warningWidth, indicatorHeight, 0);
	if (w40)
		al_draw_scaled_bitmap(warning40, 0, 0, 456, 80, indicatorX+indicatorWidth, indicatorY+indicatorWidth/3.0f, warningWidth, indicatorHeight, 0);
	if (w30)
		al_draw_scaled_bitmap(warning30, 0, 0, 456, 80, indicatorX+indicatorWidth, indicatorY+indicatorWidth/3.0f, warningWidth, indicatorHeight, 0);
	if (w20)
		al_draw_scaled_bitmap(warning20, 0, 0, 456, 80, indicatorX+indicatorWidth, indicatorY+indicatorWidth/3.0f, warningWidth, indicatorHeight, 0);
	if (w10)
		al_draw_scaled_bitmap(warning10, 0, 0, 456, 80, indicatorX+indicatorWidth, indicatorY+indicatorWidth/3.0f, warningWidth, indicatorHeight, 0);

	if (buttonWarning->GetToggleState())
		al_draw_scaled_bitmap(imageWarningOn, 0, 0, 80, 80, buttonWarning->GetBounds().x, buttonWarning->GetBounds().y, buttonWarning->GetBounds().w, buttonWarning->GetBounds().h, 0);
	else
		al_draw_scaled_bitmap(imageWarning, 0, 0, 80, 80, buttonWarning->GetBounds().x, buttonWarning->GetBounds().y, buttonWarning->GetBounds().w, buttonWarning->GetBounds().h, 0);

	if (buttonCaution->GetToggleState())
		al_draw_scaled_bitmap(imageCautionOn, 0, 0, 80, 80, buttonCaution->GetBounds().x, buttonCaution->GetBounds().y, buttonCaution->GetBounds().w, buttonCaution->GetBounds().h, 0);
	else
		al_draw_scaled_bitmap(imageCaution, 0, 0, 80, 80, buttonCaution->GetBounds().x, buttonCaution->GetBounds().y, buttonCaution->GetBounds().w, buttonCaution->GetBounds().h, 0);

	if (buttonAvion->GetToggleState())
		al_draw_scaled_bitmap(imageAvionOn, 0, 0, 80, 80, buttonAvion->GetBounds().x, buttonAvion->GetBounds().y, buttonAvion->GetBounds().w, buttonAvion->GetBounds().h, 0);
	else
		al_draw_scaled_bitmap(imageAvion, 0, 0, 80, 80, buttonAvion->GetBounds().x, buttonAvion->GetBounds().y, buttonAvion->GetBounds().w, buttonAvion->GetBounds().h, 0);

	if (fuelEmpty)
		al_draw_scaled_bitmap(warningEngine, 0, 0, 456, 80, indicatorX+indicatorWidth, indicatorY+indicatorWidth/3.0f, warningWidth, indicatorHeight, 0);

	al_draw_filled_rounded_rectangle(width/40.0f*37.0f, height/2.0f, width/40.0f*38.0f, al_get_display_height(CStateManager::MStateManager.GetDisplay())-height/3.0f, width/80.0f, width/80.0f, al_map_rgba(24, 24, 24, 0.5f));
	
	al_draw_scaled_bitmap(throttle, 0, 0, 200, 50, width/40.0f*36.0f,
		(al_get_display_height(CStateManager::MStateManager.GetDisplay()) - height/12.0f*13.0f)/100.0f*(100.0f-CStateSimulator::MStateSimulator.GetThrottle()) + height/8*5 - width/80.0f, width/40.0f*3.0f, width/40.0f, 0);

	if (!CStateSimulator::MStateSimulator.GetStart())
		al_draw_text(font, al_map_rgb(255,0,0), al_get_display_width(CStateManager::MStateManager.GetDisplay())/2.0f, y, ALLEGRO_ALIGN_CENTRE, "PRESS SPACE TO START DESCENT");

	al_draw_scaled_bitmap(panelBG, 0, 0, 2800, 640, 0, y, width, height, 0);

	CDisplayPFD::MDisplayPFD.Render();
	CDisplayECAM::MDisplayECAM.Render();

	al_draw_scaled_bitmap(panel, 0, 0, 2800, 640, 0, y, width, height, 0);
}