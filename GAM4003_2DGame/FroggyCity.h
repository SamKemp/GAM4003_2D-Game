#pragma once

#include "GameBase.h"

const int NUM_ROAD = 20;
const int NUM_FROG = 20;


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

	UBSprite road[NUM_ROAD];
	UBSprite frog[NUM_FROG];
	float roadLength;
	float PREVroadLength;
};