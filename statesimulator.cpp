#include "statesimulator.h"
#include "interfacesimulator.h"
#include "interfacedialog.h"
#include "setting.h"

CStateSimulator CStateSimulator::MStateSimulator;

void CStateSimulator::Load()
{
	CInterfaceSimulator::MInterfaceSimulator.Load();

	bg = al_load_bitmap("simulator/bg.png");
	ground = al_load_bitmap("simulator/ground.png");
	star = al_load_bitmap("simulator/star.png");
	starFG = al_load_bitmap("simulator/starfg.png");

	voyagerImage = al_load_bitmap("simulator/voyager.png");
	shipImage = al_load_bitmap("simulator/ship.png");
	shipBrakeOn = al_load_bitmap("simulator/shipbrakeon.png");
	shipBrakeOff = al_load_bitmap("simulator/shipbrakeoff.png");
	shipEngine1 = al_load_bitmap("simulator/shipengine1.png");
	shipEngine2 = al_load_bitmap("simulator/shipengine2.png");
	shipEngine3 = al_load_bitmap("simulator/shipengine3.png");
	shipLeft = al_load_bitmap("simulator/shipleft.png");
	shipRight = al_load_bitmap("simulator/shipright.png");

	rock1 = al_load_bitmap("simulator/rock1.png");
	rock2 = al_load_bitmap("simulator/rock2.png");
	rock3 = al_load_bitmap("simulator/rock3.png");

	engineLoop = al_load_sample("simulator/engineloop.ogg");
	engineLoopInstance = al_create_sample_instance(engineLoop);
	al_attach_sample_instance_to_mixer(engineLoopInstance, al_get_default_mixer());
	al_set_sample_instance_playmode(engineLoopInstance, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(engineLoopInstance, 0);

	default_random_engine generator;
	generator.seed(time(NULL));
	uniform_int_distribution<int> distribution(0, 100000);
	int count = 0;
	while (count < 1000)
	{
		CEntityRock* rock = new CEntityRock(0, distribution(generator));
		rocks.push_back(rock);
		count ++;
	}
	count = 0;
	while (count < 3000)
	{
		CEntityRock* rock = new CEntityRock(count/12.0f, distribution(generator));
		rocks.push_back(rock);
		count ++;
	}

	explosion = al_load_bitmap("simulator/explosion.png");
	explosionSound = al_load_sample("simulator/explosion.ogg");
}

void CStateSimulator::Init()
{
	al_convert_bitmaps();

	CInterfaceSimulator::MInterfaceSimulator.Init();

	start = false;

	keys[UP] = false;
	keys[DOWN] = false;
	keys[LEFT] = false;
	keys[RIGHT] = false;

	shipRender = al_create_bitmap(80,80);

	colorSky = CSetting::MSetting.GetSky();
	colorGround = CSetting::MSetting.GetGround();
	rockColor = CSetting::MSetting.GetRock();

	gravity.magnitude = CSetting::MSetting.GetGravity()/600.0f;
	gravity.direction = 180;
	thrust.magnitude = 0;
	thrust.direction = 0;
	resistance = CSetting::MSetting.GetAir()/600.0f;
	total.magnitude = 0;
	total.direction = 0;

	shipAttitude = 0;
	throttle = 0;

	ship.x = 50000;
	ship.y = 100000-CSetting::MSetting.GetHeight()*10.0f;
	voyager.x = 49472;
	voyager.y = 100000-CSetting::MSetting.GetHeight()*10.0f-230;

	weight = CSetting::MSetting.GetWeight();

	maxFuel = CSetting::MSetting.GetFuel();
	fuel = maxFuel;
	fuelWeight = fuel*2.236f;

	avionics = false;
	landed = false;
	crash = false;

	spoilers = false;

	crashCount = 0;
	frame = 0;
	frameCount = 0;

	al_play_sample_instance(engineLoopInstance);
}

void CStateSimulator::Resize()
{
	CInterfaceSimulator::MInterfaceSimulator.Resize();

	render = al_create_bitmap(sqrt(al_get_display_width(CStateManager::MStateManager.GetDisplay())*al_get_display_width(CStateManager::MStateManager.GetDisplay())
		+ al_get_display_height(CStateManager::MStateManager.GetDisplay())*al_get_display_height(CStateManager::MStateManager.GetDisplay())),
		sqrt(al_get_display_width(CStateManager::MStateManager.GetDisplay())*al_get_display_width(CStateManager::MStateManager.GetDisplay())
		+ al_get_display_height(CStateManager::MStateManager.GetDisplay())*al_get_display_height(CStateManager::MStateManager.GetDisplay())));
}

void CStateSimulator::Cleanup()
{
	al_destroy_sample_instance(engineLoopInstance);
	al_destroy_sample(engineLoop);
	
	al_destroy_bitmap(bg);

	al_destroy_bitmap(ground);
	al_destroy_bitmap(star);
	al_destroy_bitmap(starFG);

	al_destroy_bitmap(render);

	al_destroy_bitmap(voyagerImage);

	al_destroy_bitmap(shipRender);
	al_destroy_bitmap(shipImage);
	al_destroy_bitmap(shipBrakeOn);
	al_destroy_bitmap(shipBrakeOff);
	al_destroy_bitmap(shipEngine1);
	al_destroy_bitmap(shipEngine2);
	al_destroy_bitmap(shipEngine3);
	al_destroy_bitmap(shipLeft);
	al_destroy_bitmap(shipRight);

	rocks.clear();

	al_destroy_bitmap(rock1);
	al_destroy_bitmap(rock2);
	al_destroy_bitmap(rock3);
	al_destroy_bitmap(explosion);

	al_destroy_sample(explosionSound);

	CInterfaceSimulator::MInterfaceSimulator.Cleanup();
}

void CStateSimulator::HandleEvents(ALLEGRO_EVENT& ev)
{
	if (!CInterfaceDialog::MInterfaceDialog.GetOpen() && start)
	{
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			if (crash)
			{
				if (frameCount = 14)
				{
					frameCount = 0;
					if (frame < 39)
						frame ++;
				}
				else
					frameCount ++;

				if (crashCount != 150)
					crashCount ++;
				else
					CInterfaceSimulator::MInterfaceSimulator.Crash();
			}
			else if (landed)
			{
				if (throttle != 0)
					throttle --;
				if (total.magnitude != 0)
				{
					total.magnitude -= 0.5f/60.0f;
					if (total.magnitude < 0)
						total.magnitude = 0;
				}
				total.direction = 0;
				if (shipAttitude != 0)
				{
					if (shipAttitude > 0 && shipAttitude < 180)
						shipAttitude -= 0.1f;
					else if (shipAttitude >= 180)
						shipAttitude += 0.1f;

					if (shipAttitude >= 360)
						shipAttitude -= 360;
					else if (shipAttitude < 0)
						shipAttitude += 360;
				}

				if (throttle <= 0 && total.magnitude <= 0)
					CInterfaceSimulator::MInterfaceSimulator.Landed();
			}
			else
				UpdatePhysics();

			if (fuel <= 0 || crash || landed)
				al_set_sample_instance_gain(engineLoopInstance, 0.0f);
			else if (throttle/100.0f+0.5f < 1.0f)
				al_set_sample_instance_gain(engineLoopInstance, throttle/100.0f+0.5f);
			else
				al_set_sample_instance_gain(engineLoopInstance, 1.0f);

			CInterfaceSimulator::MInterfaceSimulator.HandleEvents();
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				if (!crash && !landed)
				{
					CInterfaceSimulator::MInterfaceSimulator.Pause();
					al_set_sample_instance_gain(engineLoopInstance, 0.0f);
				}
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_SLASH:
				if (spoilers)
					spoilers = false;
				else
					spoilers = true;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			}
		}
	}
	else
	{
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE && CInterfaceDialog::MInterfaceDialog.GetOpen() && CInterfaceDialog::MInterfaceDialog.GetType() == EXIT)
				CInterfaceDialog::MInterfaceDialog.GetWindow()->CloseButtonPressed();
			else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE && !CInterfaceDialog::MInterfaceDialog.GetOpen())
				CInterfaceSimulator::MInterfaceSimulator.Pause();
			else if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE && !CInterfaceDialog::MInterfaceDialog.GetOpen())
				start = true;
		}

	}// if training, then pass to other class
}

void CStateSimulator::UpdatePhysics()
{
	//TURNING
	if ((keys[RIGHT] && keys[LEFT]) || (!keys[RIGHT] && !keys[LEFT]))
	{
	}
	else if (keys[RIGHT])
	{
		shipAttitude += 0.5f;
	}
	else if (keys[LEFT])
	{
		shipAttitude -= 0.5f;
	}
	if (shipAttitude >= 360)
		shipAttitude -= 360;
	else if (shipAttitude < 0)
		shipAttitude += 360;
			///THROTTLE
	if ((keys[UP] && keys[DOWN]) || (!keys[UP] && !keys[DOWN]))
	{
	}
	else if (keys[UP])
	{
		throttle ++;
	}
	else if (keys[DOWN])
	{
		throttle --;
	}
	if (throttle > 100)
		throttle = 100;
	else if (throttle < 0)
		throttle = 0;


	if (fuel > 0)
	{
		thrust.magnitude = throttle*800.0f/weight/600.0f;
		fuel -= throttle/200.0f;
		if (fuel < 0)
			fuel = 0;
	}
	else
		thrust.magnitude = 0;

	fuelWeight = fuel*2.236f;

	gravity.magnitude = 1.622f/600.0f; 
	resistance = 0.2f/600.0f;
	
	if (spoilers)
	{
		resistance = resistance*4;
		gravity.magnitude += 1.2f/600.0f;
	}
	thrust.direction = shipAttitude;
	deltaShip.x = gravity.magnitude*sin(gravity.direction*ALLEGRO_PI/180)+
	thrust.magnitude*sin(thrust.direction*ALLEGRO_PI/180)+
	total.magnitude*sin(total.direction*ALLEGRO_PI/180);
	deltaShip.y = gravity.magnitude*cos(gravity.direction*ALLEGRO_PI/180)+
	thrust.magnitude*cos(thrust.direction*ALLEGRO_PI/180)+
	total.magnitude*cos(total.direction*ALLEGRO_PI/180);

	total.magnitude = sqrt(deltaShip.x*deltaShip.x+deltaShip.y*deltaShip.y) - resistance;
	total.direction = ArcTan(deltaShip.x, deltaShip.y)*180/ALLEGRO_PI;

	ship.x += deltaShip.x;
	ship.y -= deltaShip.y;

	if (ship.y > 99000)
	{
		for (iter = rocks.begin(); iter != rocks.end(); ++iter)
		{
			if ((*iter)->GetX() >= ship.x - 300 && (*iter)->GetX() <= ship.x + 300)
			{
				if ((*iter)->GetY() <= 160)
				{
					if ((*iter)->GetCrash(ship.x, ship.y))
					{
						Crash();
					}
				}
			}
		}
	}

	if (ship.x > 99000)
	{
		ship.x = 99000;
		Crash();
	}
	else if (ship.x < 1000)
	{
		ship.x = 1000;
		Crash();
	}
	else if (ship.y < 1000)
	{
		ship.y = 1000;
		Crash();
	}
	else if (ship.y >= 99960)
	{
		if (shipAttitude >= 15 && shipAttitude <= 345)
			Crash();
		else if (total.magnitude*6.0 >= 4.0f)
			Crash();
		else
		landed = true;
	}
	else if (total.magnitude >= 28.0f/6.0f)
		Crash();
	
	if (total.magnitude >= 20.0f/6.0f)
		overspeed = true;
	else
		overspeed = false;
}

void CStateSimulator::Render()
{
	al_set_target_bitmap(render);

	al_clear_to_color(al_map_rgb(0,0,0));

	screen.x = ship.x - al_get_bitmap_width(render)/2.0f;
	screen.y = ship.y - al_get_bitmap_height(render)/2.0f;

	al_draw_tinted_scaled_bitmap(bg, colorSky, 0, 0, 1, 1000, 0-screen.x, 0-screen.y, 100000, 100000, 0);

	int numberX = floor(screen.x/800.0f);
	int numberY = floor(screen.y/600.0f);
	int countX = 0;
	int countY = 0;
	while (countX < 4)
	{
		countY = 0;
		while (countY < 4)
		{
			al_draw_bitmap(star, 800*(numberX+countX)-screen.x, 600*(numberY+countY)-screen.y, 0);
			countY ++;
		}
		countX ++;
	}
	countX = 0;
	while (countX < 4)
	{
		countY = 0;
		while (countY < 4)
		{
			al_draw_bitmap(starFG, 800*(numberX+countX)-screen.x-deltaShip.x*100, 600*(numberY+countY)-screen.y-deltaShip.y*100, 0);
			countY ++;
		}
		countX ++;
	}

	al_draw_tinted_scaled_bitmap(ground, colorGround, 0, 0, 2000, 600, 0-screen.x, 100000-screen.y, 100000, 1200, 0);

	al_draw_bitmap(voyagerImage, voyager.x-screen.x, voyager.y-screen.y, 0);

	if (ship.y > 99000)
	{
		for (iter = rocks.begin(); iter != rocks.end(); ++iter)
		{
			if ((*iter)->GetX() >= screen.x && (*iter)->GetX() <= screen.x + al_get_bitmap_width(render))
				(*iter)->Render(screen.x, screen.y);
		}
	}

	al_set_target_bitmap(shipRender);
	al_clear_to_color(al_map_rgba(0,0,0,0));

	if (spoilers)
		al_draw_bitmap(shipBrakeOn, 0, 0, 0);
	else
		al_draw_bitmap(shipBrakeOff, 0, 0, 0);

	if ((keys[RIGHT] && keys[LEFT]) || (!keys[RIGHT] && !keys[LEFT]) || landed)
	{
	}
	else if (keys[RIGHT])
		al_draw_bitmap(shipRight, 0, 0, 0);
	else if (keys[LEFT])
		al_draw_bitmap(shipLeft, 0, 0, 0);

	if (throttle <= 0 || fuel <= 0)
	{
	}
	else if (throttle <= 20)
		al_draw_bitmap(shipEngine1, 0, 0, 0);
	else if (throttle <= 50)
		al_draw_bitmap(shipEngine2, 0, 0, 0);
	else if (throttle <= 100)
		al_draw_bitmap(shipEngine3, 0, 0, 0);

	al_draw_bitmap(shipImage, 0, 0, 0);

	al_set_target_bitmap(al_get_backbuffer(CStateManager::MStateManager.GetDisplay()));

	al_draw_rotated_bitmap(render, al_get_bitmap_width(render)/2.0f, al_get_bitmap_height(render)/2.0f, al_get_display_width(CStateManager::MStateManager.GetDisplay())/2.0f,
		al_get_display_height(CStateManager::MStateManager.GetDisplay())/2.0f, (360-shipAttitude)*ALLEGRO_PI/180, 0);

	if (frame <= 28)
		al_draw_bitmap(shipRender, al_get_display_width(CStateManager::MStateManager.GetDisplay())/2.0f-40, al_get_display_height(CStateManager::MStateManager.GetDisplay())/2.0f-40, 0);

	if (crash)
	{
		al_draw_bitmap_region(explosion, (frame % 8) * 288, (frame / 8) * 288, 288, 288, al_get_display_width(CStateManager::MStateManager.GetDisplay())/2.0f-144,
			al_get_display_height(CStateManager::MStateManager.GetDisplay())/2.0f-144, 0);
	}

	CInterfaceSimulator::MInterfaceSimulator.Render();
}

void CStateSimulator::Crash()
{
	crash = true;
	al_play_sample(explosionSound, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
}