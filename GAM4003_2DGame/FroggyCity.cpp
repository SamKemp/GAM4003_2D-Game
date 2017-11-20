#include "FroggyCity.h"



FroggyCity::FroggyCity(void)
{

}


FroggyCity::~FroggyCity(void)
{
    
}

void FroggyCity::setup()
{
	setBackground("images/grass.jpg");

	for (int i = 0; i < NUM_ROAD; i++)
	{
		road[i].setImage("images/roads/roadEW.tga");
		road[i].setWorldPosition((float)i*road[i].getWidth(), (float)(height / 2) - road[i].getCenterY());
	}
}

void FroggyCity::logic()
{

}

void FroggyCity::draw()
{
	for (int i = 0; i < NUM_ROAD; i++)
	{
		road[i].updateEverything();
	}
    showStatus();
}

void FroggyCity::showStatus()
{

}