#include "FroggyCity.h"

int SpriteHeight = 32;
int SpriteWidth = 32;

FroggyCity::FroggyCity(void)
{

}


FroggyCity::~FroggyCity(void)
{
    
}

void FroggyCity::setup()
{
	// Dynamic creaton of sprite
	grid = new UBSprite*[NUM_GRID_X];
	for(int i = 0; i < NUM_GRID_X;i++)
	{
		grid[i] = new UBSprite[NUM_GRID_Y];
	}

	// Set seed for Random based on the current time
	// and adding a random number
	srand((int)time(0) + rand());


	for (int i = 0; i < NUM_GRID_X; i++)
	{
		for (int j = 0; j < NUM_GRID_Y; j++)
		{
			ChangeType(0, i, j);
		}
	}

	Map1();

	Money = 100.0f;
	Population = 0;
	Wave = 0;
	slainDinos = 0;
}

void FroggyCity::logic()
{
	
}

void FroggyCity::draw()
{
	for (int i = 0; i < NUM_GRID_X; i++)
	{
		for (int j = 0; j < NUM_GRID_Y; j++)
		{
			grid[i][j].updateEverything();
		}
	}


    showStatus();
}

void FroggyCity::showStatus()
{
	print("STATUS", width - 275, height - 250);
	print("Money", width - 275, height - 200);
	print(Money, width - 100, height - 200);
	print("Population", width - 275, height - 170);
	print(Population, width - 100, height - 170);
	print("Wave", width - 275, height - 140);
	print(Wave, width - 100, height - 140);
	print("Dinosaurs Slain", width - 275, height - 110);
	print(slainDinos, width - 100, height - 110);
	print("Wave Timer", width - 275, height - 80);
	print("0.0", width - 100, height - 80);
}

// Shoudd't need to look at

void FroggyCity::ChangeType(int type, int i, int j)
{
	grid[i][j].setType(type);

	if(type == 1)
	{
		int tmpPosX = i, tmpPosY = j;

		grid[tmpPosX][tmpPosY].setImage("images/grid/road/roadV2.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 1, tmpPosY = j;
		grid[tmpPosX][tmpPosY].setImage("images/grid/road/roadV1.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 1, tmpPosY = j;
		grid[tmpPosX][tmpPosY].setImage("images/grid/road/roadV3.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 2, tmpPosY = j;
		grid[tmpPosX][tmpPosY].setImage("images/grid/road/pavement.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 2, tmpPosY = j;
		grid[tmpPosX][tmpPosY].setImage("images/grid/road/pavement.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

	}
	else if(type == 2)
	{
		int tmpPosX = i, tmpPosY = j;

		grid[tmpPosX][tmpPosY].setImage("images/grid/road/roadH2.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i, tmpPosY = j - 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/road/roadH1.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i, tmpPosY = j + 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/road/roadH3.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i, tmpPosY = j + 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/road/pavement.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i, tmpPosY = j - 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/road/pavement.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

	}
	else if(type == 3)
	{
		int tmpPosX, tmpPosY;

		grid[i][j].setImage("images/grid/road/RoadCross_4.png");
		grid[i][j].setWorldPositionY(0 + (SpriteHeight*j));
		grid[i][j].setWorldPositionX((float)i*SpriteWidth);

		tmpPosX = i - 1, tmpPosY = j - 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/road/RoadCross_0.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i, tmpPosY = j - 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/road/RoadCross_1.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 1, tmpPosY = j - 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/road/RoadCross_2.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 1, tmpPosY = j;
		grid[tmpPosX][tmpPosY].setImage("images/grid/road/RoadCross_3.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 1, tmpPosY = j;
		grid[tmpPosX][tmpPosY].setImage("images/grid/road/RoadCross_5.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 1, tmpPosY = j + 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/road/RoadCross_6.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i, tmpPosY = j + 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/road/RoadCross_7.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 1, tmpPosY = j + 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/road/RoadCross_8.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);
	}
	else if (type == 4)
	{
		grid[i][j].setImage("images/grid/frog.png");
		grid[i][j].setWorldPositionY(0 + (SpriteHeight*j));
		grid[i][j].setWorldPositionX((float)i*SpriteWidth);
	}
	else if (type == 5)
	{
		grid[i][j].setImage("images/grid/dino.png");
		grid[i][j].setWorldPositionY(0 + (SpriteHeight*j));
		grid[i][j].setWorldPositionX((float)i*SpriteWidth);
	}
	else if(type == 6)
	{
		int tmpPosX, tmpPosY;

		grid[i][j].setImage("images/grid/turret/Turret_12.png");
		grid[i][j].setWorldPositionY(0 + (SpriteHeight*j));
		grid[i][j].setWorldPositionX((float)i*SpriteWidth);

		tmpPosX = i - 2, tmpPosY = j - 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_0.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 1, tmpPosY = j - 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_1.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i, tmpPosY = j - 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_2.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 1, tmpPosY = j - 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_3.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 2, tmpPosY = j - 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_4.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 2, tmpPosY = j - 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_5.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 1, tmpPosY = j - 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_6.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i, tmpPosY = j - 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_7.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 1, tmpPosY = j - 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_8.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 2, tmpPosY = j - 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_9.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 2, tmpPosY = j;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_10.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 1, tmpPosY = j;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_11.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 1, tmpPosY = j;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_13.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 2, tmpPosY = j;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_14.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 2, tmpPosY = j + 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_15.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 1, tmpPosY = j + 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_16.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i, tmpPosY = j + 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_17.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 1, tmpPosY = j + 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_18.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 2, tmpPosY = j + 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_19.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 2, tmpPosY = j + 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_20.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 1, tmpPosY = j + 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_21.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i, tmpPosY = j + 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_22.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 1, tmpPosY = j + 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_23.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 2, tmpPosY = j + 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/turret/Turret_24.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);
	}
	else if(type == 7)
	{
		int tmpPosX, tmpPosY;

		grid[i][j].setImage("images/grid/building/Building_12.png");
		grid[i][j].setWorldPositionY(0 + (SpriteHeight*j));
		grid[i][j].setWorldPositionX((float)i*SpriteWidth);

		tmpPosX = i - 2, tmpPosY = j - 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_0.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 1, tmpPosY = j - 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_1.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i, tmpPosY = j - 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_2.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 1, tmpPosY = j - 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_3.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 2, tmpPosY = j - 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_4.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 2, tmpPosY = j - 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_5.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 1, tmpPosY = j - 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_6.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i, tmpPosY = j - 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_7.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 1, tmpPosY = j - 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_8.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 2, tmpPosY = j - 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_9.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 2, tmpPosY = j;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_10.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 1, tmpPosY = j;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_11.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 1, tmpPosY = j;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_13.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 2, tmpPosY = j;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_14.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 2, tmpPosY = j + 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_15.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 1, tmpPosY = j + 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_16.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i, tmpPosY = j + 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_17.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 1, tmpPosY = j + 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_18.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 2, tmpPosY = j + 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_19.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 2, tmpPosY = j + 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_20.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 1, tmpPosY = j + 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_21.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i, tmpPosY = j + 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_22.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 1, tmpPosY = j + 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_23.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 2, tmpPosY = j + 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/building/Building_24.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);
	}
	else if (type == 8)
	{
		grid[i][j].setImage("images/grid/bar.png");
		grid[i][j].setWorldPositionY(0 + (SpriteHeight*j));
		grid[i][j].setWorldPositionX((float)i*SpriteWidth);
	}
	else if(type == 9)
	{
		int tmpPosX, tmpPosY;

		grid[i][j].setImage("images/grid/plot/plot.png");
		grid[i][j].setWorldPositionY(0 + (SpriteHeight*j));
		grid[i][j].setWorldPositionX((float)i*SpriteWidth);

		tmpPosX = i - 2, tmpPosY = j - 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot0.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 1, tmpPosY = j - 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i, tmpPosY = j - 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 1, tmpPosY = j - 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 2, tmpPosY = j - 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot4.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 2, tmpPosY = j - 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 1, tmpPosY = j - 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i, tmpPosY = j - 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 1, tmpPosY = j - 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 2, tmpPosY = j - 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 2, tmpPosY = j;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 1, tmpPosY = j;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 1, tmpPosY = j;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 2, tmpPosY = j;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 2, tmpPosY = j + 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 1, tmpPosY = j + 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i, tmpPosY = j + 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 1, tmpPosY = j + 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 2, tmpPosY = j + 1;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 2, tmpPosY = j + 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot20.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i - 1, tmpPosY = j + 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i, tmpPosY = j + 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 1, tmpPosY = j + 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);

		tmpPosX = i + 2, tmpPosY = j + 2;
		grid[tmpPosX][tmpPosY].setImage("images/grid/plot/plot24.png");
		grid[tmpPosX][tmpPosY].setWorldPositionY(0 + (SpriteHeight*tmpPosY));
		grid[tmpPosX][tmpPosY].setWorldPositionX((float)tmpPosX*SpriteWidth);
	}
	else if (type == 0)
	{
		grid[i][j].setImage("images/grid/grass.png");
		grid[i][j].setWorldPositionY(0 + (SpriteHeight*j));
		grid[i][j].setWorldPositionX((float)i*SpriteWidth);
	}
}

void FroggyCity::onKeyPressed()
{
	ChangeType(4, 1, 10);
}

void FroggyCity::Map1()
{
	for (int i = 0; i < NUM_GRID_X; i++)
	{
		for (int j = 0; j < NUM_GRID_Y; j++)
		{
			if(i < 21 && j == 12) { ChangeType(2, i, j); }
			else if(i == 22 && j < 11) { ChangeType(1, i, j); }
			else if(i == 22 && j == 12) { ChangeType(3, i, j); }
			else if(i == 22 && (j > 13 && j < (NUM_GRID_Y-9))) { ChangeType(1, i, j); }
			else if(i > 23 && j == 12) { ChangeType(2, i, j); }
			else if(i == 17 && j == 2) { ChangeType(9, i, j); }
			else if(i == 27 && j == 2) { ChangeType(9, i, j); }
			else if(i == 3 && j == 7) { ChangeType(9, i, j); }
			else if(i == 10 && j == 7) { ChangeType(9, i, j); }
			else if(i == 17 && j == 7) { ChangeType(9, i, j); }
			else if(i == 27 && j == 7) { ChangeType(9, i, j); }
			else if(i == 33 && j == 7) { ChangeType(9, i, j); }
			else if(i == 39 && j == 7) { ChangeType(9, i, j); }
			else if(i == 45 && j == 7) { ChangeType(9, i, j); }
			else if(i == 51 && j == 7) { ChangeType(9, i, j); }
			else if(i == 57 && j == 7) { ChangeType(9, i, j); }
			else if(i == 3 && j == 17) { ChangeType(9, i, j); }
			else if(i == 10 && j == 17) { ChangeType(9, i, j); }
			else if(i == 17 && j == 17) { ChangeType(9, i, j); }
			else if(i == 27 && j == 17) { ChangeType(9, i, j); }
			else if(i == 33 && j == 17) { ChangeType(9, i, j); }
			else if(i == 39 && j == 17) { ChangeType(9, i, j); }
			else if(i == 45 && j == 17) { ChangeType(9, i, j); }
			else if(i == 51 && j == 17) { ChangeType(9, i, j); }
			else if(i == 57 && j == 17) { ChangeType(9, i, j); }
			else if(i == 17 && j == NUM_GRID_Y - 12) { ChangeType(9, i, j); }
			else if(i == 27 && j == NUM_GRID_Y - 12) { ChangeType(9, i, j); }
			else if(j > (NUM_GRID_Y - 10)) { ChangeType(8, i, j); }
		}
	}
}