#include "FroggyCity.h"

int SpriteHeight = 16;
int SpriteWidth = 16;

FroggyCity::FroggyCity(void)
{

}


FroggyCity::~FroggyCity(void)
{
    
}

void FroggyCity::setup()
{

	grid = new UBSprite*[NUM_GRID];
	for(int i = 0; i < NUM_GRID;i++)
	{
		grid[i] = new UBSprite[NUM_GRID];
	}

	//Bar = height / 10;
	//float rndX, rndY;

	//Set seed for Random based on the current time
	srand((int)time(0) + rand());

	setBackground("images/grass.jpg");

	for (int i = 0; i < NUM_GRID; i++)
	{
		for (int j = 0; j < NUM_GRID; j++)
		{
			grid[i][j].setImage(SpriteHeight,SpriteWidth,0,0,0);
			grid[i][j].setWorldPositionY(0 + (SpriteHeight*j));
			grid[i][j].setWorldPositionX((float)i*SpriteWidth);

			grid[i][j].updateEverything();
		}
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

void FroggyCity::onKeyReleased()
{
	cout << to_string(grid[1][1].getY()) << to_string(grid[1][1].getX()) << endl;
}