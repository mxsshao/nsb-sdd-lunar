#pragma once

#include "global.h"

class CEntityRock
{
private:
	int x;
	int y;
	float scale;

	float rotation;
	int rock;

public:
	CEntityRock(int y, int x)
	{
		CEntityRock::y = y;
		CEntityRock::x = x;
		scale = (float)rand()/((float)RAND_MAX);

		rock = rand() % 3;
		rotation = rand() % 360;
	};

	bool GetCrash(float shipX, float shipY);
	void Render(float screenX, float screenY);
	int GetX() {return x;};
	int GetY() {return y;};
};