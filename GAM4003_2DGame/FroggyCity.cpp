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
		rndX = rnd.number(0,width - (width / 30));
		rndY = rnd.number((float)0,(height - Bar));
		frog[i].setImage("images/FrogFace.png");
		frog[i].setWorldPosition(rndX, rndY);
		frog[i].setTransparentColour(0,0,0);

	}
	
	for (int i = 0; i < NUM_BAR; i++)
	{
		bar[i].setImage("images/Bar2.png");
		barLength = 0;
		barLength = width /bar[i].getWidth();
		if (i <= barLength)
		{
			bar[i].setWorldPosition((float)i*bar[i].getWidth(), (float)(height - BAR) - bar[i].getCenterY());
			PREVbarLength = barLength;
		}
		else
		{
			barLength = width /bar[i].getWidth() + PREVbarLength + 1;
			if (i <= barLength)
			{
				bar[i].setWorldPosition((float)(i-PREVbarLength -1)*bar[i].getWidth(), (float)(height - BAR + bar[i].getHeight()) - bar[i].getCenterY());
			}
		}
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
		print(i, road[i].getCenterX(), road[i].getCenterY());
	}

	for (int i = 0; i < NUM_FROG; i++) {
		frog[i].updateEverything();
		print(i, frog[i].getCenterX(), frog[i].getCenterY());
	}

	for (int i = 0; i < NUM_BAR; i++)
	{
		bar[i].updateEverything();
		print(i, bar[i].getCenterX(), bar[i].getCenterY());
	}
	
    showStatus();
}

void FroggyCity::showStatus()
{

}
