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

	grid = new UBSprite*[NUM_GRID_X];
	for(int i = 0; i < NUM_GRID_X;i++)
	{
		grid[i] = new UBSprite[NUM_GRID_Y];
	}

	//Bar = height / 10;
	//float rndX, rndY;

	//Set seed for Random based on the current time
	srand((int)time(0) + rand());

	setBackground("images/grass.jpg");

	for (int i = 0; i < NUM_GRID_X; i++)
	{
		for (int j = 0; j < NUM_GRID_Y; j++)
		{
			grid[i][j].setImage("images/grid/empty_32z32.png");
			grid[i][j].setTransparentColour(0,0,0);
			grid[i][j].setWorldPositionY(0 + (SpriteHeight*j));
			grid[i][j].setWorldPositionX((float)i*SpriteWidth);
		}
	}
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
			//string temp = to_string(j);
			//print(temp, grid[i][j].getCenterX(), grid[i][j].getCenterY());
		}
	}


    showStatus();
}

void FroggyCity::showStatus()
{

}

void FroggyCity::onKeyReleased()
{
	cout << to_string(grid[1][1].getY()) << to_string(grid[1][1].getX())<< endl;
}