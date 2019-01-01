#pragma once

#include "statemanager.h"
#include "statebase.h"
#include "entityrock.h"

struct Vector
{
	float magnitude;
	float direction;
};

struct Coordinate
{
	float x;
	float y;
};

class CStateSimulator : public CStateBase
{
protected:
	CStateSimulator() {};

private:
	bool keys [4];
	enum keys {UP, DOWN, LEFT, RIGHT};

	bool start;

	ALLEGRO_SAMPLE* engineLoop;
	ALLEGRO_SAMPLE_INSTANCE* engineLoopInstance;

	ALLEGRO_BITMAP* bg;
	ALLEGRO_BITMAP* ground;
	ALLEGRO_BITMAP* star;
	ALLEGRO_BITMAP* starFG;
	ALLEGRO_BITMAP* render;

	ALLEGRO_BITMAP* voyagerImage;
	ALLEGRO_BITMAP* shipRender;
	ALLEGRO_BITMAP* shipImage;
	ALLEGRO_BITMAP* shipBrakeOn;
	ALLEGRO_BITMAP* shipBrakeOff;
	ALLEGRO_BITMAP* shipEngine1;
	ALLEGRO_BITMAP* shipEngine2;
	ALLEGRO_BITMAP* shipEngine3;
	ALLEGRO_BITMAP* shipLeft;
	ALLEGRO_BITMAP* shipRight;

	list<CEntityRock *> rocks;
	list<CEntityRock *>::iterator iter;

	ALLEGRO_BITMAP* rock1;
	ALLEGRO_BITMAP* rock2;
	ALLEGRO_BITMAP* rock3;

	ALLEGRO_COLOR colorSky;
	ALLEGRO_COLOR colorGround;
	ALLEGRO_COLOR rockColor;

	Vector gravity;
	Vector thrust;
	float resistance;
	Vector total;

	Coordinate voyager;
	Coordinate deltaShip;
	Coordinate ship;
	float shipAttitude;
	Coordinate screen;

	float fuel;
	float maxFuel;
	float fuelWeight;

	float weight;
	float throttle;
	bool spoilers;

	bool avionics;
	bool landed;
	bool overspeed;

	ALLEGRO_BITMAP* explosion;
	ALLEGRO_SAMPLE* explosionSound;
	bool crash;
	int crashCount;
	int frame;
	int frameCount;

	double ArcTan(float x, float y)
	{
		if (x >= 0 && y >= 0)
			return abs(atan(x/y));
		else if (x >= 0 && y <= 0)
			return ALLEGRO_PI - abs(atan(x/y));
		else if (x <= 0 && y <= 0)
			return ALLEGRO_PI + abs(atan(x/y));
		else if (x <= 0 && y >= 0)
			return ALLEGRO_PI*2 - abs(atan(x/y));
		else
			return 0;
	};

	void UpdatePhysics();

public:
	static CStateSimulator MStateSimulator;
	void Load();
	void Init();
	void Resize();
	void Cleanup();

	void Pause() {};
	void Resume() {};

	void HandleEvents(ALLEGRO_EVENT& ev);
	void Render();

	void Crash();

	bool GetStart() {return start;};

	float GetAttitude() {return shipAttitude;};
	float GetAltitude() {return (100000-ship.y-40)/10.0f;};
	float GetSpeed() {return total.magnitude*6.0f;};
	float GetFuel() {return fuel;};
	float GetMaxFuel() {return maxFuel;};
	float GetFuelWeight() {return fuelWeight;};
	float GetThrottle() {return throttle;};

	bool GetAvionics() {return avionics;};
	void SetAvionics(bool a) {avionics = a;};

	bool GetOverspeed() {return overspeed;};

	ALLEGRO_BITMAP* GetRock1() {return rock1;};
	ALLEGRO_BITMAP* GetRock2() {return rock2;};
	ALLEGRO_BITMAP* GetRock3() {return rock3;};
	ALLEGRO_COLOR GetRockColor() {return rockColor;};
};