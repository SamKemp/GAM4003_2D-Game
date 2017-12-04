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

}

void FroggyCity::onKeyPressed()
{
	
}

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
	else if (type == 8)
	{
		grid[i][j].setImage("images/grid/bar.png");
		grid[i][j].setWorldPositionY(0 + (SpriteHeight*j));
		grid[i][j].setWorldPositionX((float)i*SpriteWidth);
	}
	else if (type == 0)
	{
		grid[i][j].setImage("images/grid/empty-g.png");
		grid[i][j].setWorldPositionY(0 + (SpriteHeight*j));
		grid[i][j].setWorldPositionX((float)i*SpriteWidth);
	}
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
			else if(j > (NUM_GRID_Y - 10)) { ChangeType(8, i, j); }
		}
	}
}