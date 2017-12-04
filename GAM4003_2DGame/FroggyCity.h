#pragma once

#include "GameBase.h"


const int NUM_GRID_X = 60;
const int NUM_GRID_Y = 34;
const int MAX_BUILD_TIME = 5;


class FroggyCity :
	public GameBase
{
public:
	FroggyCity();
	~FroggyCity();
	void setup();
	void logic();
	void draw();
	void showStatus();

	// Meh touchy
	void onKeyReleased();
	void onMousePressed();

	// Sorta touchy
	void onWave();
	void onBuild();
	void onPause();

	// No touchy
	void ChangeType(int type, int x, int y);
	void Map1();

// We're not sure touch?
private:

	float Bar;
	float Money;
	int Wave, Population, slainDinos, buildTimer;

	UBSprite ** grid;
	UBSprite UI_Building;
	UBSprite UI_Turret;
	
	float roadLength;
	float PREVroadLength;
	float barLength;
	float PREVbarLength;
};
