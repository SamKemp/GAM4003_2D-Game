#pragma once

#include "GameBase.h"
#include "json.hpp"

const int NUM_GRID_X = 60, NUM_GRID_Y = 34;

// Game States
const int MENU = 1,
		  SETUP = 2,
		  TUTORIAL = 3,
		  PLAY = 4,
		  PAUSE = 5,
		  END = 6;
// Grid Types
const int GRASS = 0,
		  VERT_ROAD = 1,
	      HOR_ROAD = 2,
		  CROSS_ROAD = 3,
		  FROG = 4,
		  DINO = 5,
		  TURRET = 6,
		  BUILDING = 7,
		  BAR = 8,
		  PLOT = 9,
		  TRANS = 10;


class FroggyCity :
	public GameBase
{
public:
	FroggyCity();
	~FroggyCity();
	void setup();
	void setVariables();
	void logic();
	void draw();
	void showStatus();

	// Meh touchy
	void onKeyReleased();
	void onMousePressed();

	// Sorta touchy
	void onWave();
	void onBuild();
	void onPause();
	void spawnDino(int spawnFrom);
	void spawnFrog(int count);

	// No touchy
	void MenuScreen();
	void SetupScreen();
	void TutorialScreen();
	void PlayScreen();
	void PauseScreen();
	void EndScreen();

	// Don't look, it should be shot
	void ChangeType(int type, int x, int y);
	void Map1();

	// Defo no touch
	// Code for Highscore website
	void SaveHighScore(string username);

// We're not sure touch?
private:

	float Bar,
		  Money;
	int SpriteHeight = 32,
		SpriteWidth = 32,
		Wave,
		Population,
		slainDinos,
		buildTimer,
		Timer = 0,
		num_buildings = 0,
		num_dinos = -1,
		num_spawn_dinos,
		MAX_BUILD_TIME,
		_DinosPerRound,
		gameState;
	bool runTimer = true,
		canBuild = true;
	string data;

	UBSprite ** grid;
	UBSprite UI_Building;
	UBSprite UI_Turret;
};
