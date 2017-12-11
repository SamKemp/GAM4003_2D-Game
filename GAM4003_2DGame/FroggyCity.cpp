#include "FroggyCity.h"

// ***************************************************
// **************** Global Game Settings *************
// ***************************************************
void FroggyCity::setVariables()
{
	// Sets the multiplier for the number of dinos each wave
	_DinosPerRound = 10;

	// Sets the length of time the player has to build (in seconds)
	MAX_BUILD_TIME = 60;

	// Sets the starting monies
	Money = 100.0f;

	// Sets the starting population
	Population = 0;

	// Sets the starting wave number
	Wave = 0;

	// Sets the starting number of slain dinos
	slainDinos = 0;

	// Sets the build timer to the MAX_BUILD_TIME
	buildTimer = MAX_BUILD_TIME;
}
// ***************************************************
// ************************ END **********************
// ***************************************************

FroggyCity::FroggyCity(void)
{

}


FroggyCity::~FroggyCity(void)
{
    
}

void FroggyCity::setup()
{
	gameState = MENU;

	// Dynamic creaton of sprite
	grid = new UBSprite*[NUM_GRID_X]; // grid[60]
	for(int i = 0; i < NUM_GRID_X;i++)
	{
		grid[i] = new UBSprite[NUM_GRID_Y]; // grid[60][34]
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

	// UI_Building setup
	UI_Building.setImage("images/Building.png");
	UI_Building.setWorldPositionX(((width/2)-300) - 150);
	UI_Building.setWorldPositionY(height - 230);

	// UI_Turret setup
	UI_Turret.setImage("images/Turret.png");
	UI_Turret.setWorldPositionX(((width/2)-300) + 150);
	UI_Turret.setWorldPositionY(height - 230);

	Timer = time(NULL) + 1;

	setVariables();

	
}

void FroggyCity::logic()
{
	if(runTimer)
	{
		if (time(NULL) == Timer)
		{
			Timer = time(NULL) + 1;
			buildTimer--;
		}
	}

	if (num_spawn_dinos > 0)
	{
		//spawnDino();
		num_dinos++;
		num_spawn_dinos--;
	}

	if(buildTimer == -1)
	{
		onWave();
	}

	if(num_dinos == 0)
	{
		num_dinos = -1;
		cout << "Dinos dead: " << num_dinos << endl;

		onBuild();
	}
}

void FroggyCity::onWave()
{
	runTimer = false;
	buildTimer = MAX_BUILD_TIME;
	Wave++;

	// Stop Build actions
	canBuild = false;

	// Spawn Dinos
	num_spawn_dinos = Wave * _DinosPerRound;
	num_dinos = 0;
	cout << "Set dinos: " << num_dinos << endl;
}

void FroggyCity::onBuild()
{
	buildTimer = MAX_BUILD_TIME;
	Timer = time(NULL) + 1;
	runTimer = true;
	Money = Money + (num_buildings * 50);

	// Allow build actions
	canBuild = true;
}

void FroggyCity::spawnDino(int spawnFrom)
{
	int x = 0, y = 0;

	switch(spawnFrom){
		case 1:
			x = 22;
			y = 1;
			break;
		case 2:
			x = 22;
			y = 1;
			break;
		case 3:
			x = 22;
			y = 1;
			break;
		case 4:
			x = 22;
			y = 1;
			break;
	}

	//Some code here
}
void FroggyCity::spawnFrog(int count)
{
	//Some code here
}

void FroggyCity::draw()
{
	switch(gameState)
	{
		case MENU:
			MenuScreen();
			break;
		case SETUP:
			SetupScreen();
			break;
		case TUTORIAL:
			TutorialScreen();
			break;
		case PLAY:
			PlayScreen();
			break;
		case PAUSE:
			PauseScreen();
			break;
		case END:
			EndScreen();
			break;
	}
}

void FroggyCity::FireTurret(int type, int x, int y)
{

	int fx = x - 10, fy = y - 10,
		lx = x + 10, ly = y + 10;
	int gradient = 0;
 
	for (int i = fx; i != lx; i++)
	{
		for (int j = fy; j != ly; j++)
		{
			if (type == TURRET && grid[i][j].getType() == DINO)
			{
				Bullet.setWorldPosition(x, y);
				Bullet.setAutoMove(30);
				Bullet.setVelocities(i - x, j - y);
			}
			else if (type == DINO && grid[i][j].getType() == TURRET)
			{
				Bullet.setWorldPosition(x, y);
				Bullet.setAutoMove(30);
				Bullet.setVelocities(i - x, j - y);
			}
		}
	}
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
	print(buildTimer, width - 100, height - 80);

	print("|", width - 350, height - 284);
	print("|", width - 350, height - 268);
	print("|", width - 350, height - 252);
	print("|", width - 350, height - 236);
	print("|", width - 350, height - 220);
	print("|", width - 350, height - 204);
	print("|", width - 350, height - 188);
	print("|", width - 350, height - 172);
	print("|", width - 350, height - 156);
	print("|", width - 350, height - 140);
	print("|", width - 350, height - 124);
	print("|", width - 350, height - 108);
	print("|", width - 350, height - 92);
	print("|", width - 350, height - 76);
	print("|", width - 350, height - 60);
	print("|", width - 350, height - 44);
	print("|", width - 350, height - 28);
	print("|", width - 350, height - 12);
}

void FroggyCity::onKeyReleased()
{
	if(num_dinos > 0)
	{
		num_dinos--;
		cout << "Killed dino: " << num_dinos << endl;
		slainDinos++;
	}
}

void FroggyCity::onMousePressed()
{

	if(canBuild)
	{
		// Building management code
	}
}


// No touchy

void FroggyCity::MenuScreen()
{
	for (int i = 0; i < NUM_GRID_X; i++)
	{
		for (int j = 0; j < NUM_GRID_Y; j++)
		{
			ChangeType(10, i, j);
		}
	}

	setBackground("images/MenuScreen.png");
}

void FroggyCity::SetupScreen()
{
	// Fill the map
	Map1();

	//Start the game
	onBuild();
}

void FroggyCity::TutorialScreen()
{
	
}

void FroggyCity::PlayScreen()
{
	for (int i = 0; i < NUM_GRID_X; i++)
	{
		for (int j = 0; j < NUM_GRID_Y; j++)
		{
			grid[i][j].updateEverything();

			if (grid[i][j].getType() == TURRET) 
			{
				FireTurret(TURRET, i, j);
			}
 
			if (grid[i][j].getType() == DINO)
			{
				FireTurret(DINO, i, j);
			}
		}
	}

	// UI_Building update
	UI_Building.updateEverything();
	// UI_Turret update
	UI_Turret.updateEverything();

    showStatus();
}

void FroggyCity::PauseScreen()
{

}

void FroggyCity::EndScreen()
{
	for (int i = 0; i < NUM_GRID_X; i++)
	{
		for (int j = 0; j < NUM_GRID_Y; j++)
		{
			ChangeType(10, i, j);
		}
	}

	setBackground("images/EndScreen.png");
}

// ***************************************************
// *********** Seriously Don't Touch *****************
// ***************************************************

void FroggyCity::ChangeType(int type, int i, int j)
{
	grid[i][j].setType(type);

	if(type == VERT_ROAD)
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
	else if(type == HOR_ROAD)
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
	else if(type == CROSS_ROAD)
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
	else if (type == FROG)
	{
		grid[i][j].setImage("images/grid/frog.png");
		grid[i][j].setWorldPositionY(0 + (SpriteHeight*j));
		grid[i][j].setWorldPositionX((float)i*SpriteWidth);
	}
	else if (type == DINO)
	{
		grid[i][j].setImage("images/grid/dino.png");
		grid[i][j].setWorldPositionY(0 + (SpriteHeight*j));
		grid[i][j].setWorldPositionX((float)i*SpriteWidth);
	}
	else if(type == TURRET)
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
	else if(type == BUILDING)
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

		Population = Population + 6;
		num_buildings++;
	}
	else if (type == BAR)
	{
		grid[i][j].setImage("images/grid/bar.png");
		grid[i][j].setWorldPositionY(0 + (SpriteHeight*j));
		grid[i][j].setWorldPositionX((float)i*SpriteWidth);
	}
	else if(type == PLOT)
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
	else if (type == GRASS)
	{
		grid[i][j].setImage("images/grid/grass.png");
		grid[i][j].setWorldPositionY(0 + (SpriteHeight*j));
		grid[i][j].setWorldPositionX((float)i*SpriteWidth);
	}
	else if (type == TRANS)
	{
		grid[i][j].setImage("images/grid/empty.png");
		grid[i][j].setTransparentColour(255, 255, 255);
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
			else if(i == 17 && j == 2) { ChangeType(9, i, j); }
			else if(i == 27 && j == 2) { ChangeType(9, i, j); }
			else if(i == 3 && j == 7) { ChangeType(9, i, j); }
			else if(i == 10 && j == 7) { ChangeType(9, i, j); }
			else if(i == 17 && j == 7) { ChangeType(9, i, j); }
			else if(i == 27 && j == 7) { ChangeType(7, i, j); }
			else if(i == 33 && j == 7) { ChangeType(9, i, j); }
			else if(i == 39 && j == 7) { ChangeType(9, i, j); }
			else if(i == 45 && j == 7) { ChangeType(9, i, j); }
			else if(i == 51 && j == 7) { ChangeType(9, i, j); }
			else if(i == 57 && j == 7) { ChangeType(9, i, j); }
			else if(i == 3 && j == 17) { ChangeType(9, i, j); }
			else if(i == 10 && j == 17) { ChangeType(9, i, j); }
			else if(i == 17 && j == 17) { ChangeType(7, i, j); }
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

// Defo no touch
// Code for Highscore stuff
void FroggyCity::SaveHighScore(string username)
{
	
}