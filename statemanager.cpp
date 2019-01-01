#include "statemanager.h"
#include "statebase.h"
#include "stateintro.h"

CStateManager CStateManager::MStateManager;

void CStateManager::Init(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, Controls::Canvas* canvas, Skin::TexturedBase* defaultSkin, Skin::TexturedBase* menuSkin, ALLEGRO_MOUSE_CURSOR* cursor, ALLEGRO_MOUSE_CURSOR* hand, ALLEGRO_MOUSE_CURSOR* text)
{
	done = false;

	CStateManager::display = display;
	CStateManager::event_queue = event_queue;
	CStateManager::canvas = canvas;
	CStateManager::defaultSkin = defaultSkin;
	CStateManager::menuSkin = menuSkin;
	CStateManager::cursor = cursor;
	CStateManager::hand = hand;
	CStateManager::text = text;

	mutex = al_create_mutex();
	cond = al_create_cond();
}

void CStateManager::Resize()
{
	buttonHeight = al_get_display_height(CStateManager::MStateManager.GetDisplay())/64.0f*3.0f;
	buttonWidth = buttonHeight*4.0f;
	defaultSkin->SetDefaultFont(L"font/OpenSans.ttf", buttonHeight/2.0f);
	states.back()->Resize();
}

void CStateManager::Cleanup()
{
	al_destroy_mutex(mutex);
	al_destroy_cond(cond);

	while (!states.empty())
	{
		states.back()->Cleanup();
		states.pop_back();
	}
}

void CStateManager::ChangeState(CStateBase* state)
{
	al_stop_samples();
	al_set_target_bitmap(NULL);

	ready = false;

	if (state != &CStateIntro::MStateIntro)
	{
		thread = al_create_thread(Thread, &MStateManager);
		al_start_thread(thread);

		al_lock_mutex(mutex);
		while (!ready)
		{
			al_wait_cond(cond, mutex);
		}
		al_unlock_mutex(mutex);
		ready = false;
	}

	// cleanup the current state
	if (!states.empty()) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Load();

	if (state != &CStateIntro::MStateIntro)
	{
		al_destroy_thread(thread);
	}

	al_set_target_bitmap(al_get_backbuffer(display));
	Resize();
	states.back()->Init();

	if (state != &CStateIntro::MStateIntro)
	{
		al_flush_event_queue(event_queue);
	}
}

void CStateManager::PushState(CStateBase* state)
{
}

void CStateManager::PopState()
{
}

void CStateManager::HandleEvents(ALLEGRO_EVENT& ev)
{
	states.back()->HandleEvents(ev);
	canvas->ProcessDelayedDeletes();
}

void CStateManager::Render()
{
	states.back()->Render();
	canvas->RenderCanvas();
}

void* CStateManager::Thread(ALLEGRO_THREAD* thread, void *arg)
{
	CStateManager *manager = (CStateManager*) arg;
	int countA = 0;
	int countB = 180;

	PHYSFS_init(NULL);
	al_set_physfs_file_interface();
	PHYSFS_mount("resources.dat", NULL, 1);

	ALLEGRO_BITMAP* circleLarge = al_load_bitmap("manager/circleLarge.png");
	ALLEGRO_BITMAP* circleSmall = al_load_bitmap("manager/circleSmall.png");

	float size = al_get_display_height(manager->GetDisplay())/1000.0f;
	int x = al_get_display_width(manager->GetDisplay())-(size*100)-60;
	int y = al_get_display_height(manager->GetDisplay())-(size*100)-60;

	al_lock_mutex(manager->GetMutex());
	al_set_target_bitmap(al_get_backbuffer(manager->GetDisplay()));
	manager->SetReady(true);
	al_broadcast_cond(manager->GetCond());
	al_unlock_mutex(manager->GetMutex());

	while (!al_get_thread_should_stop(thread))
	{
			ALLEGRO_EVENT ev;
			al_wait_for_event(manager->GetEventQueue(), &ev);

			al_lock_mutex(manager->GetMutex());
			if (ev.type == ALLEGRO_EVENT_TIMER)
			{
				countA += 10;
				countB -= 5;
			}
			else if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
			{
				if (!al_resize_display(ev.display.source, ev.display.width, ev.display.height))
				{
					al_show_native_message_box(NULL, "ERROR", "A03:", "Error resizing window.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
					manager->Exit();
					return NULL;
				}
				manager->GetCanvas()->SetSize(ev.display.width, ev.display.height);
				size = al_get_display_height(manager->GetDisplay())/1000.0f;
				x = al_get_display_width(manager->GetDisplay())-(size*100)-60;
				y = al_get_display_height(manager->GetDisplay())-(size*100)-60;
			}
			else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				manager->Exit();
				return NULL;
			}

			if (al_is_event_queue_empty(manager->GetEventQueue()))
			{
				al_draw_scaled_rotated_bitmap(circleLarge, 50, 50, x, y, size, size, countA*ALLEGRO_PI/180, 0);
				al_draw_scaled_rotated_bitmap(circleSmall, 50, 50, x, y, size, size, countB*ALLEGRO_PI/180, 0);
				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));
			}
			al_unlock_mutex(manager->GetMutex());
	}

	al_destroy_bitmap(circleLarge);
	al_destroy_bitmap(circleSmall);
	return NULL;
}