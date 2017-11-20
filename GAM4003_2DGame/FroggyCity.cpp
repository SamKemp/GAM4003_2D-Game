#include "FroggyCity.h"



FroggyCity::FroggyCity(void)
{

}


FroggyCity::~FroggyCity(void)
{
    
}

void FroggyCity::setup()
{
	Bar = height / 10;
	float rndX, rndY;


	setBackground("images/grass.jpg");

	for (int i = 0; i < NUM_ROAD; i++)
	{
		road[i].setImage("images/roads/roadEW.tga");
		roadLength = 0;
		roadLength = width / road[i].getWidth();
		if (i <= roadLength)
		{
			road[i].setWorldPosition((float)i*road[i].getWidth(), (float)((height - Bar) / 2) - road[i].getCenterY());
			PREVroadLength = roadLength;
		}
		else
		{
			road[i].setImage("images/roads/roadNS.tga");
			roadLength = 0;
			roadLength = (height-Bar) / road[i].getHeight() + PREVroadLength +1;
			if (i <= roadLength)
			{
				road[i].setWorldPosition((float)(width / 2) - road[i].getCenterX(), (float)(i- PREVroadLength-1)*road[i].getHeight());
			}
			else if(i <= roadLength + 1)
			{
				road[i].setImage("images/roads/roadNEWS.tga");
				road[i].setWorldPosition((float)(width / 2) - road[i].getCenterX(), (float)((height - Bar) / 2) - road[i].getCenterY());
			}
			else
			{
				road[i].setWorldPosition(-5000, -5000);

			}
		}
	}

	for (int i = 0; i < NUM_FROG; i++) 
	{
		rndX = rnd.number(0,1820);
		rndY = rnd.number(0,1000);
		frog[i].setImage("images/FrogFace.png", 0.5);
		frog[i].setWorldPosition(rndX, rndY);
		frog[i].setTransparentColour(0,0,0);

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
		frog[i].updateEverything();
		print(i, road[i].getCenterX(), road[i].getCenterY());
		print(i, frog[i].getCenterX(), frog[i].getCenterY());
	}
    showStatus();
}

void FroggyCity::showStatus()
{

}