#include "global.h"
#include "setting.h"
#include "statemanager.h"
#include "stateintro.h"

int main(int argc, char **argv)
{
	float gameTime = 0;
	int frames = 0;
	int gameFPS = 0;

	//ALLEGRO INIT
	if (!al_init())
	{
		al_show_native_message_box(NULL, "ERROR", "A01:", "Error loading Allegro.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	cout << "Allegro Init Successful" << endl;

	//ALLEGRO ADDONS
	al_install_keyboard();
	al_install_mouse();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_audio();
	al_init_acodec_addon();

	//IMAGE SETTINGS
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR | ALLEGRO_MIPMAP | ALLEGRO_CONVERT_BITMAP);
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

	//ALLEGRO VAR
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer;
	ALLEGRO_DISPLAY* display;
	ALLEGRO_FONT* font20;
	ALLEGRO_BITMAP* icon;

	//SETTINGS INIT
	CSetting::MSetting.LoadSettings();

	/*if (CSetting::MSetting.FileExists("resources.dat"))
	{
		//PHYSFS INIT
		PHYSFS_init(argv[0]);
		al_set_physfs_file_interface();
		PHYSFS_mount("resources.dat", NULL, 1);
		cout << "PHYSFS Init Successful" << endl;
	}
	else
	{
		al_show_native_message_box(NULL, "ERROR", "R01:", "Missing resource file \"resources.dat\"", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}*/

	//Display
	al_set_new_display_flags(ALLEGRO_RESIZABLE);

	display = al_create_display(1000, 680);
	if (!display)
	{
		al_show_native_message_box(NULL, "ERROR", "A02:", "Error creating display.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	icon = al_load_bitmap("allegro_icon.png");
	al_set_display_icon(display, icon);
	al_set_window_title(display, "Tycho Simulator");

	al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, CSetting::MSetting.GetFullscreen());

	cout << "Display created " << al_get_display_width(display) << "*" << al_get_display_height(display) << endl;

	//FIRST LOADING SCREEN
	font20 = al_load_font("font/OpenSans.ttf", 20, 0);
	al_clear_to_color(al_map_rgb(0,0,0));
	al_draw_text(font20, al_map_rgb(255,255,255), al_get_display_width(display)-120, al_get_display_height(display)-40, 0, "LOADING...");
	al_flip_display();
	al_clear_to_color(al_map_rgb(0,0,0));

	al_reserve_samples(15);

	//------------------------
	//GWEN INIT
	//------------------------
	Gwen::Renderer::Allegro* renderer = new Gwen::Renderer::Allegro();
	Gwen::Skin::TexturedBase defaultskin(renderer);
	defaultskin.SetRender(renderer);
	defaultskin.Init("skin/defaultskin.png");
	defaultskin.SetDefaultFont(L"font/OpenSans.ttf", 14);
	Gwen::Skin::TexturedBase menuskin(renderer);
	menuskin.SetRender(renderer);
	menuskin.Init("skin/menuskin.png");
	menuskin.SetDefaultFont(L"font/OpenSans.ttf", 20);

	Gwen::Controls::Canvas* canvas = new Gwen::Controls::Canvas(&defaultskin);
	canvas->SetSize(al_get_display_width(display), al_get_display_height(display));
	canvas->SetDrawBackground(false);
	canvas->SetKeyboardInputEnabled(true);

	Gwen::Input::Allegro GwenInput;
	GwenInput.Initialize(canvas);

	cout << "GWEN Init successful" << endl;

	//MOUSE
	ALLEGRO_BITMAP *imageCursor = al_load_bitmap("mouse/cursor.png");
	ALLEGRO_BITMAP *imageHand = al_load_bitmap("mouse/hand.png");
	ALLEGRO_BITMAP *imageText = al_load_bitmap("mouse/text.png");
	ALLEGRO_MOUSE_CURSOR *cursor = al_create_mouse_cursor(imageCursor, 0, 0);
	ALLEGRO_MOUSE_CURSOR *hand = al_create_mouse_cursor(imageHand, 0, 0);
	ALLEGRO_MOUSE_CURSOR *text = al_create_mouse_cursor(imageText, 0, 16);
	al_set_mouse_cursor(display, cursor);

	//AUDIO SETTINGS
	al_set_mixer_gain(al_get_default_mixer(), CSetting::MSetting.GetVolume());

	//TIMER
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	//------------------------
	//STATEMANAGER INIT
	//------------------------
	CStateManager::MStateManager.Init(display, event_queue, canvas, &defaultskin, &menuskin, cursor, hand, text);
	CStateManager::MStateManager.ChangeState(&CStateIntro::MStateIntro);
	CStateManager::MStateManager.Resize();

	//EVENT SOURCES
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());

	srand(time(NULL));
	al_start_timer(timer);
	gameTime = al_current_time();

	//EVENT LOOP
	while (!CStateManager::MStateManager.XRunning())
	{
			ALLEGRO_EVENT ev;
			al_wait_for_event(event_queue, &ev);

			GwenInput.ProcessMessage(ev);

			//RESIZE WINDOW
			if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
			{
				if (!al_resize_display(ev.display.source, ev.display.width, ev.display.height))
				{
					al_show_native_message_box(NULL, "ERROR", "A03:", "Error resizing window.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
					return -1;
				}
				canvas->SetSize(ev.display.width, ev.display.height);
				CStateManager::MStateManager.Resize();
				cout << "Display resized " << ev.display.width << "*" << ev.display.height << endl;
			}
			//CLOSE BUTTON
			else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				CStateManager::MStateManager.Exit();
			}
			else
			{
				CStateManager::MStateManager.HandleEvents(ev);
			}

			//al_grab_mouse(display);

			if (al_is_event_queue_empty(event_queue))
			{
				CStateManager::MStateManager.Render();
				
				frames ++;
				if(al_current_time() - gameTime >= 1)
				{
					gameTime = al_current_time();
					gameFPS = frames;
					frames = 0;
				}

				if (CSetting::MSetting.GetFPS())
					al_draw_textf(font20, al_map_rgb(255, 255, 0), 5, 5, 0, "FPS: %i", gameFPS);

				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));
			}
	}

	//EXIT
	CStateManager::MStateManager.Cleanup();

	CSetting::MSetting.SaveSettings();

	canvas->DelayedDelete();

	al_destroy_font(font20);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	return 0;
}