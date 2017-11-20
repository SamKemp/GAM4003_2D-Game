#include "FroggyCity.h"



FroggyCity::FroggyCity(void)
{

}


FroggyCity::~FroggyCity(void)
{
    
}

void FroggyCity::setup()
{
	setBackground("images/grass.png");

	for (int i = 0; i < NUM_ROAD; i++)
	{
		road[i].setImage("images/roads/roadEW.tga");
		road[i].setWorldPosition(40, 40);
		road[i].setVelocities(3.0f, 0.0f);
	}
}

void FroggyCity::logic()
{

}

void FroggyCity::draw()
{

    showStatus();
}

void FroggyCity::showStatus()
{

}