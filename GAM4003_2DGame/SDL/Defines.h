//////////////////////////////////////////////////////////////////////////////////
// Defines.h
//////////////////////////////////////////////////////////////////////////////////

#pragma once

// Window related defines //
#define WINDOW_WIDTH   800
#define WINDOW_HEIGHT  600
#define WINDOW_CAPTION "Block Breaker"

// Game related defines //
#define FRAMES_PER_SECOND 30
#define FRAME_RATE        1000/FRAMES_PER_SECOND 

// Location of images within bitmap //
#define PADDLE_BITMAP_X 0
#define PADDLE_BITMAP_Y 0
#define BALL_BITMAP_X   100 
#define BALL_BITMAP_Y   0
#define YELLOW_X		0
#define YELLOW_Y		20
#define RED_X			0
#define RED_Y			40
#define BLUE_X			80
#define BLUE_Y			20
#define GREEN_X			80
#define GREEN_Y			40

// Minimum distance from the side of the screen to a block //
#define BLOCK_SCREEN_BUFFER 40  

// Maximum number of blocks allowed //
#define MAX_BLOCKS 80
#define NUM_ROWS   6
#define NUMCOLS    9

// Location of the player's paddle in the game //
#define PLAYER_Y 550

// Dimensions of a paddle //
#define PADDLE_WIDTH  100
#define PADDLE_HEIGHT 20

// Dimensions of a block //
#define BLOCK_WIDTH  80
#define BLOCK_HEIGHT 20

// Diameter of the ball //
#define BALL_DIAMETER 20

// Paddle speed //
#define PLAYER_SPEED 10

// Ball speeds //
#define BALL_SPEED_MODIFIER 5  // divide location on paddle by this
#define BALL_SPEED_Y        10 // max speed of ball along y axis

// Maximum number of times the player can miss the ball //
#define NUM_LIVES 5

// Number of levels, increase this value to add new levels //
#define NUM_LEVELS 3

// Locations of output text //
#define LIVES_X 5
#define LIVES_Y 5
#define LEVEL_X 75
#define LEVEL_Y 5









