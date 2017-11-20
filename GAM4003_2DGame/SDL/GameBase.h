// Copyright Peter Lager and Steve Manning and the University of Bolton
// Do what you like with the program, but please include your
// sourcecode with any distribution and retain my copyright notices
// v1.2 - AWSprite now UBSprite.  AWFont not now used.  
//		TrueTypeFont class used - enables font to be re-sized and colour changed.
//		Multiple key presses can now be used - use isKeyPressed to check each key
//		draw functions added: drawLine, drawDottedLine, drawCircle, drawFilledRect
//		AudioManager updated so that MP3 music can be played alongside sound effects - use addMusic, playMusic
//		setBackround now deletes OLD img, if available
// v1.1 - upgraded to use SDL2 with Renderer and Texture, and audio code added.

#pragma once

#include <iostream>
#include <string>

#include "SDL.h"
#include "SDL_image.h"
#include "audiomanager.h"

#include "UBSprite.h"	//v1.2
class TrueTypeFont;

#include "console.h"
#include "random.h"

using namespace std;
using namespace con;
using namespace random;

#define SAFE_DELETE_PTR(ptr) if( (ptr) != NULL ) delete (ptr); (ptr) = NULL;	//v1.1 Added by A.Simpson
#define SAFE_DELETE_ARY(ary) if( (ary) != NULL ) delete[] (ary); (ary) = NULL;

/**
This has the core functionality to create simple games. <br>

All games should be built from its own game class that inherits
from this class. <br>
*/
class GameBase
{
public:
	GameBase(void);
	virtual ~GameBase(void);

	/**
	You should override this method to provide code to setup your
	game variables and state.
	*/
	virtual void setup(){}
	/**
	Once your game has been created and initialised (using setup)
	call this method to start the game.
	*/
	virtual void start();
//******************************************************	//v1.1
	static SDL_Window *window;
	static SDL_Renderer *renderer;
	static SDL_Texture *texture;
	SDL_Rect m_camera;
	//******************************************************
	SDL_Surface* screen;
	SDL_Surface* bg;

	const Uint8 *kbArray;	//v1.2 Used for multiple key presses

	static SDL_Texture *bgTexture; //v1.1
	SDL_Texture *fontTexture; 
//Globally used font 
	TrueTypeFont *ttFont;	
	/**
	This function sets the Font size and colour.
	For efficiency, it should be called once, e.g. in the setup function.
	*/
	void setFontSizeAndColour(int fs, int r = 255, int g = 255, int b = 255);
	/*
	The values in these variables are updated every iteration of the game loop
	and should be used in the onKey??? and onMouse??? methods to change the game
	variables and state.
	*/
	int mouseX, mouseY;
	int pmouseX, pmouseY;
	int mouseButton;
	int keyDown, keyUp;
	bool ctrlPressed, shiftPressed, altPressed;

	boolean gameover;

	// These are used to store the width and height of the display area
	// and are initialised when you create the game object.
	int width, height;

	virtual void logic(){}
	virtual void draw(){}
	virtual void post(){}

	virtual void getUserInput();

	int enableKeyRepeat(int first = 70, int repeat = 70);

	/**
	Set the background colour for the display. The parameters should
	be in the range 0-255 inclusive.
	*/
	void setBackground(int red, int green, int blue);
	/**
	Set the backgorund image to be used for the display area. It makes
	sense if the image is the same size as the display area as scaling
	is not applied to the image.
	*/
	void setBackground(char *imageFileName);
	void resetBackground(char *imageFileName);	//****added by SM

	/**
	Add a sound effect for later playing.
	@param filename the filename of the audio file to load.
	@param name this should be unique for eah sound effect added.
	*/
	bool addSound(char* filename, string name);
	bool addMusic(char* filename);	//v1.2
	/**
	Play the sound effect (expect some latency)
	@param name the unique name of the sound effect to play
	*/
	void playSound(string name);
	void playMusic(int vilDiv = 1);	//v1.2
	/*
	These  provide null functionality event handlers you should
	override these if needed.
	*/
	virtual void onMousePressed(){}
	virtual void onMouseReleased(){}
	virtual void onMouseMoved(){}
	virtual void onKeyPressed(){}
	virtual void onKeyReleased(){}

	/**
	This is the main gameloop and is called when the game is started. <br>
	Although it is possible to override this method in your own game class
	this is not recommended unless you really know what you are doing!!!
	*/
	void gameLoop();

	/**
	These methods are used to display text on the screen at a given position.
	Three colours are available GREEN, WHITE and RED if you don't specify a
	colour then it will use green text.
	@param value the value to display can be of type string, char array,
	integer or float
	@param x the horizontal pixel position to start printing
	@param y the vertical (character top) pixel position to use
	*/
	int print(string value, int x, int y); //v1.2 colour param removed
	int print(const char* value, int x, int y);
	int print(int value, int x, int y);
	int print(float value, int x, int y);

protected:
	/**
	Get a surface for the display of the given size and colour
	*/
	static SDL_Surface* getSurface(int width, int height, int red, int green, int blue);

public:
	/**
	This method should be called before you create your game object otherwise your
	program will crash. <br>
	It creates the window with a display area width x height and sets the background
	to black. <br>
	You can use the setBackground methods to change the background colour or
	provide an image to use.
	*/
	static int initSDL(int width, int height, bool fullscreen, string title = "SDL Game");	//v1.1
	bool addSound(char* filename, string name, int volDivider);

	static SDL_Surface* screen_surface;	//v1.1
	static SDL_Surface* background_surface;
//v1.2 - functions added 
	int getFontHeight();
	void drawLine(int x0, int y0, int x1, int y1, int r = 255, int g = 255, int b = 255, int a = 255);	
	void drawDottedLine(int x0, int y0, int x1, int y1, int r = 255, int g = 255, int b = 255, int a = 255);	
	void drawCircle(int cx, int cy, int rad, int r = 255, int g = 255, int b = 255, int a = 255, bool fillIn=false);
	void drawFilledRect(int topLeftx, int topLefty, int w, int h, int r = 255, int g = 255, int b = 255, int a = 255);
	bool isKeyDown(int k) {	//for multiple key presses.  Uses kbArray of keys
		return kbArray[SDL_GetScancodeFromKey((SDL_Keycode)k)] == 1;
	}
	private:
		void createCircle(SDL_Point *points, int cx, int cy, int rad, int &count, SDL_Rect *rects, int &rCount);
		void update8CirclePts(SDL_Point *pts, int &currCount, int cx, int cy, int x, int y, SDL_Rect *rects, int &rCount);
};
