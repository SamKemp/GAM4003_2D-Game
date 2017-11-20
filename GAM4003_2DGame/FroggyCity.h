#pragma once

#include "GameBase.h"

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
};