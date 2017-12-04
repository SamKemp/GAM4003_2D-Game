#pragma once

#include "GameBase.h"


const int NUM_GRID_X = 60;
const int NUM_GRID_Y = 34;


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
	void onKeyPressed();
	//void onKeyReleased();
	//void onMousePressed();
	//void onMouseReleased();
	void ChangeType(int type, int x, int y);
	void Map1();
private:

	float Bar;
	float Money;
	int Wave, Population, slainDinos;

	UBSprite ** grid;
	
	float roadLength;
	float PREVroadLength;
	float barLength;
	float PREVbarLength;
};
