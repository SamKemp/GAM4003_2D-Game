#pragma once

#include "GameBase.h"


const int NUM_GRID_X = 60;
const int NUM_GRID_Y = 34;
//const int NUM_ROAD = 50;
//const int NUM_FROG = 100;
//const int NUM_BAR = 100;


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
	void Map1();
	void CheckPlayer();
private:

	float Bar;

	UBSprite ** grid;
	//UBSprite road[NUM_ROAD];
	//UBSprite frog[NUM_FROG];
	//UBSprite bar[NUM_BAR];
	
	float roadLength;
	float PREVroadLength;
	float barLength;
	float PREVbarLength;
};
