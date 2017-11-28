#include "FroggyCity.h"

int SpriteHeight = 32;
int SpriteWidth = 32;

// Just for fun stuff
int playerX = 1, playerY = 1;

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

	//setBackground("images/grass.jpg");

	for (int i = 0; i < NUM_GRID_X; i++)
	{
		for (int j = 0; j < NUM_GRID_Y; j++)
		{
			grid[i][j].setImage("images/grid/empty-g.png");
			grid[i][j].setTransparentColour(0,0,0);
			grid[i][j].setWorldPositionY(0 + (SpriteHeight*j));
			grid[i][j].setWorldPositionX((float)i*SpriteWidth);
		}
	}

	Map1();

	// Just for fun
	grid[playerX][playerY].setImage("images/grid/player.png");
	grid[playerX][playerY].setTransparentColour(0,0,0);
	grid[playerX][playerY].setWorldPositionY(0 + (SpriteHeight*playerY));
	grid[playerX][playerY].setWorldPositionX((float)playerX*SpriteWidth);
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
	if(keyDown == SDLK_SPACE)
	{
		Map1();
	}
	else if(keyDown == SDLK_UP)
	{
		grid[playerX][playerY].setImage("images/grid/empty-b.png");
		grid[playerX][playerY].setTransparentColour(0,0,0);
		grid[playerX][playerY].setWorldPositionY(0 + (SpriteHeight*playerY));
		grid[playerX][playerY].setWorldPositionX((float)playerX*SpriteWidth);

		playerX = playerX;
		playerY = playerY - 1;

		CheckPlayer();

		grid[playerX][playerY].setImage("images/grid/player.png");
		grid[playerX][playerY].setTransparentColour(0,0,0);
		grid[playerX][playerY].setWorldPositionY(0 + (SpriteHeight*playerY));
		grid[playerX][playerY].setWorldPositionX((float)playerX*SpriteWidth);
	}
	else if(keyDown == SDLK_DOWN)
	{
		grid[playerX][playerY].setImage("images/grid/empty-b.png");
		grid[playerX][playerY].setTransparentColour(0,0,0);
		grid[playerX][playerY].setWorldPositionY(0 + (SpriteHeight*playerY));
		grid[playerX][playerY].setWorldPositionX((float)playerX*SpriteWidth);

		playerX = playerX;
		playerY = playerY + 1;

		CheckPlayer();

		grid[playerX][playerY].setImage("images/grid/player.png");
		grid[playerX][playerY].setTransparentColour(0,0,0);
		grid[playerX][playerY].setWorldPositionY(0 + (SpriteHeight*playerY));
		grid[playerX][playerY].setWorldPositionX((float)playerX*SpriteWidth);
	}
	else if(keyDown == SDLK_LEFT)
	{
		grid[playerX][playerY].setImage("images/grid/empty-b.png");
		grid[playerX][playerY].setTransparentColour(0,0,0);
		grid[playerX][playerY].setWorldPositionY(0 + (SpriteHeight*playerY));
		grid[playerX][playerY].setWorldPositionX((float)playerX*SpriteWidth);

		playerX = playerX - 1;
		playerY = playerY;
		
		CheckPlayer();

		grid[playerX][playerY].setImage("images/grid/player.png");
		grid[playerX][playerY].setTransparentColour(0,0,0);
		grid[playerX][playerY].setWorldPositionY(0 + (SpriteHeight*playerY));
		grid[playerX][playerY].setWorldPositionX((float)playerX*SpriteWidth);
	}
	else if(keyDown == SDLK_RIGHT)
	{
		grid[playerX][playerY].setImage("images/grid/empty-b.png");
		grid[playerX][playerY].setTransparentColour(0,0,0);
		grid[playerX][playerY].setWorldPositionY(0 + (SpriteHeight*playerY));
		grid[playerX][playerY].setWorldPositionX((float)playerX*SpriteWidth);

		playerX = playerX + 1;
		playerY = playerY;
		
		CheckPlayer();

		grid[playerX][playerY].setImage("images/grid/player.png");
		grid[playerX][playerY].setTransparentColour(0,0,0);
		grid[playerX][playerY].setWorldPositionY(0 + (SpriteHeight*playerY));
		grid[playerX][playerY].setWorldPositionX((float)playerX*SpriteWidth);
	}
	else
	{
		int x = mouseX / 32;
		int y = mouseY / 32;

		grid[x][y].setImage("images/grid/empty-b.png");
		grid[x][y].setTransparentColour(0,0,0);
		grid[x][y].setWorldPositionY(0 + (SpriteHeight*y));
		grid[x][y].setWorldPositionX((float)x*SpriteWidth);
	}
}

void FroggyCity::Map1()
{
	for (int i = 0; i < NUM_GRID_X; i++)
	{
		for (int j = 0; j < NUM_GRID_Y; j++)
		{
			if(i < 20 && j == 10) {grid[i][j].setImage("images/grid/pavement.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i < 21 && j == 11) {grid[i][j].setImage("images/grid/ph-road1.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i < 21 && j == 12) {grid[i][j].setImage("images/grid/ph-road2.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i < 21 && j == 13) {grid[i][j].setImage("images/grid/ph-road3.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i < 20 && j == 14) {grid[i][j].setImage("images/grid/pavement.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i == 20 && j < 11) {grid[i][j].setImage("images/grid/pavement.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i == 21 && j < 11) {grid[i][j].setImage("images/grid/ph-road1.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i == 22 && j < 11) {grid[i][j].setImage("images/grid/ph-road2.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i == 23 && j < 11) {grid[i][j].setImage("images/grid/ph-road3.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i == 24 && j < 11) {grid[i][j].setImage("images/grid/pavement.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i == 21 && j == 11) {grid[i][j].setImage("images/grid/ph-RoadCross_0.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i == 22 && j == 11) {grid[i][j].setImage("images/grid/ph-RoadCross_1.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i == 23 && j == 11) {grid[i][j].setImage("images/grid/ph-RoadCross_2.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i == 21 && j == 12) {grid[i][j].setImage("images/grid/ph-RoadCross_3.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i == 22 && j == 12) {grid[i][j].setImage("images/grid/ph-RoadCross_4.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i == 23 && j == 12) {grid[i][j].setImage("images/grid/ph-RoadCross_5.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i == 21 && j == 13) {grid[i][j].setImage("images/grid/ph-RoadCross_6.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i == 22 && j == 13) {grid[i][j].setImage("images/grid/ph-RoadCross_7.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i == 23 && j == 13) {grid[i][j].setImage("images/grid/ph-RoadCross_8.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i == 20 && (j > 13 && j < (NUM_GRID_Y-9))) {grid[i][j].setImage("images/grid/pavement.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i == 21 && (j > 13 && j < (NUM_GRID_Y-9))) {grid[i][j].setImage("images/grid/ph-road1.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i == 22 && (j > 13 && j < (NUM_GRID_Y-9))) {grid[i][j].setImage("images/grid/ph-road2.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i == 23 && (j > 13 && j < (NUM_GRID_Y-9))) {grid[i][j].setImage("images/grid/ph-road3.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i == 24 && (j > 13 && j < (NUM_GRID_Y-9))) {grid[i][j].setImage("images/grid/pavement.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i > 24 && j == 10) {grid[i][j].setImage("images/grid/pavement.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i > 23 && j == 11) {grid[i][j].setImage("images/grid/ph-road1.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i > 23 && j == 12) {grid[i][j].setImage("images/grid/ph-road2.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i > 23 && j == 13) {grid[i][j].setImage("images/grid/ph-road3.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(i > 24 && j == 14) {grid[i][j].setImage("images/grid/pavement.png"); grid[i][j].setTransparentColour(0,0,0);}
			else if(j > (NUM_GRID_Y - 10)) {grid[i][j].setImage("images/grid/bar.png");}
			else {grid[i][j].setImage("images/grid/empty-g.png"); grid[i][j].setTransparentColour(0,0,0);}
			grid[i][j].setWorldPositionY(0 + (SpriteHeight*j));
			grid[i][j].setWorldPositionX((float)i*SpriteWidth);
		}
	}
}

void FroggyCity::CheckPlayer()
{
	if(playerX < 0)
	{
		playerX = NUM_GRID_X;
	}
	if(playerY < 0)
	{
		playerY = NUM_GRID_Y;
	}
	if(playerX >= NUM_GRID_X)
	{
		playerX = 0;
	}
	if(playerY >= NUM_GRID_Y)
	{
		playerY = 0;
	}
}