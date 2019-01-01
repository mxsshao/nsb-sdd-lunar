#pragma once

#include "global.h"
#include "statemanager.h"

class CSetting
{
protected:
	CSetting();

private:
	float volume;
	bool fullscreen;
	bool FPS;
	
	CSimpleIniA ini;

	float gravity;
	float air;
	float height;
	float weight;
	float fuel;
	ALLEGRO_COLOR sky;
	ALLEGRO_COLOR ground;
	ALLEGRO_COLOR rock;

public:
	static CSetting MSetting;

	bool FileExists(const std::string& filename);

	bool GetFPS() {return FPS;};

	void SetVolume(float v)
	{
		volume = v;
		al_set_mixer_gain(al_get_default_mixer(), volume);
	};
	float GetVolume() {return volume;};
	void SetFullscreen(bool f)
	{
		fullscreen = f;
	};
	bool GetFullscreen() {return fullscreen;};

	void LoadSettings();
	void SaveSettings();

	void SetSimulator(float gravity, float air, float height, float weight, float fuel, ALLEGRO_COLOR sky, ALLEGRO_COLOR ground, ALLEGRO_COLOR rock)
	{
		CSetting::gravity = gravity;
		CSetting::air = air;
		CSetting::height = height;
		CSetting::weight = weight;
		CSetting::fuel = fuel;
		CSetting::sky = sky;
		CSetting::ground = ground;
		CSetting::rock = rock;
	}

	float GetGravity() {return gravity;};
	float GetAir() {return air;};
	float GetHeight() {return height;};
	float GetWeight() {return weight;};
	float GetFuel() {return fuel;};
	ALLEGRO_COLOR GetSky() {return sky;};
	ALLEGRO_COLOR GetGround() {return ground;};
	ALLEGRO_COLOR GetRock() {return rock;};
};