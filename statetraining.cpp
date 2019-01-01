#include "setting.h"
#include "statetraining.h"
#include "statesimulator.h"
#include "interfacedialog.h"

CStateTraining CStateTraining::MStateTraining;

void CStateTraining::Load()
{
	CSetting::MSetting.SetSimulator(1.622f, 0.129760f, 800, 15000, 1600, al_map_rgb(0,0,0), al_map_rgb(150,150,150), al_map_rgb(150,150,150));
	image = al_load_bitmap("training/training.png");
	CStateSimulator::MStateSimulator.Load();
}

void CStateTraining::Init()
{
	al_convert_bitmaps();
	CStateSimulator::MStateSimulator.Init();
	CInterfaceDialog::MInterfaceDialog.Init("Training", "Welcome",
		"To get started, click the switch labeled \"Avion\" in the upper right to turn on the electronic display. Then use the arrow keys and the forward slash key to control the ship.", OK);
}

void CStateTraining::Resize()
{
	width = al_get_display_width(CStateManager::MStateManager.GetDisplay());
	height = width/35.0f*8.0f;
	CStateSimulator::MStateSimulator.Resize();
}

void CStateTraining::Cleanup()
{
	al_destroy_bitmap(image);
	CStateSimulator::MStateSimulator.Cleanup();
}

void CStateTraining::HandleEvents(ALLEGRO_EVENT& ev)
{
	if (!CInterfaceDialog::MInterfaceDialog.GetOpen())
		CStateSimulator::MStateSimulator.HandleEvents(ev);
}

void CStateTraining::Render()
{
	CStateSimulator::MStateSimulator.Render();

	al_draw_scaled_bitmap(image, 0, 0, 2800, 640, 0, 0, width, height, 0);
}