#pragma once

#include "GameBase.h"

const int NUM_ROAD = 50;


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
	float BAR;

	UBSprite road[NUM_ROAD];
	float roadLength;
	float PREVroadLength;
};