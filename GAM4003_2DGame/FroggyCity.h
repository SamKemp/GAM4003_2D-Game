#pragma once

#include "GameBase.h"


const int NUM_GRID = 100;
const int NUM_ROAD = 50;
const int NUM_FROG = 100;
const int NUM_BAR = 1000;


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
private:

	float Bar;

	UBSprite grid[NUM_GRID][NUM_GRID];
	UBSprite road[NUM_ROAD];
	UBSprite frog[NUM_FROG];
	UBSprite bar[NUM_BAR];
	
	float roadLength;
	float PREVroadLength;
	float barLength;
	float PREVbarLength;
};
