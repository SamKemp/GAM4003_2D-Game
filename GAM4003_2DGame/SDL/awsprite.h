// Copyright Andrew Williams, Peter Lager, Steve Manning and the University of Bolton
// Do what you like with the program, but please include your
// sourcecode with any distribution and retain my copyright notices

/* 
Version 5.0	- Major change to SDL2, which includes facility to rotate images.

Changes made in V5.0
	SDL2 now used.
	Association with GameBase class added, so that the renderer object can be accessed.
	Textures rather surfaces now used for rendering. Numerous functions updated.
	For rotation, function rotateAndDraw added.  NOTE: ONLY rotation ABOUT centre of image used.
	Functions bb_collision and pp_collision_32 changed to take into account of rotated image.  
	BBs re-calc'd for both, using transformations. (numerous helper functions added too).
	Functions added: rotateAndDraw, rotatedPt, getBBPts, findRotatedTopBottom, isPtInImage, getLastFrame
	Functions now obsolete: set_surface_alpha, bite
Changes made in V4.70
	A default consructor has been added that calls the AWSprite(width, height)
	constructor to create a solid black sprite 10x10. This code in parameter
	constructor	has been refactored to prevent an exception when sprites using
	these constructors are deleted.
	Several setImage() methods added to enable changing the sprite image
	In the constructors and setImage methods all the Uint32 parameters have been
	changed to Sint32 to avoid 'ambiguous overloading' errors. In VS2010 implicit
	conversion of 'int' parameters to Uint32 meant that the linker could not
	distiguish between some methods.
Changes made in V4.60
	worldX and worldY cast as floats. Where previously used as integers appropriate type casting has been applied.
	Angular methods to set x and y velocities based on an angle and scalar value - Stephen Phillips(2010)
	Radial collision method added - Christopher Rae(2010)
Changes made in V4.51
	Bug in the calculation of the x position of the sprite hitting this one in
	pp_collision_32() fixed.
Changes made in V4.50
	This version can replace earlier versions without user side code changes.
	The major new feature in this version allows AWSprites to share image
	data. The user of AWSprite can choose which sprites share image data and
	which do not (Both modes are allowed in the same program).
	All the constructors now have an additional boolean parameter called 'share'
	which determines whether the image data is available to be shared. The
	default value is false so image data is not shared so existing programs can use
	use this version without alteration.
	If the constructor is called with a value of true for this parameter then the
	image data will be available to be shared by any AWSprite that uses the same
	bitmap file (or combination of bitmap / alpha files)
	Note that if the 'bite', 'restore_images' or an alpha level method is used on
	a sprite it affects any other sprite that shares its image data.
Changes made V4.02
	redundant code removed to fix a bug when run on a PS2
	Changes made in V4.01
	fixes a memory leak found in changes introduced by version 4.0
	*/

#ifndef __AWSPRITEINCLUDED__

#define __AWSPRITEINCLUDED__  (1)

#include "SDL.h"
#include "SDL_image.h"

#include <map>
#include <vector>	//v5.0
#include <string>
#include <iostream>
#include <math.h> //Stephen Phillips
using namespace std;


#include "GameBase.h"	//v5.0


class AWSprite;

// Declares the type 'callback' a function pointer to a class
//  method of AWSprite method that returns void and has no
//  parameters. - pkl
//typedef void(AWSprite::*callback)(void);

typedef struct {
	Uint32 currentFrame;
	Uint32 firstFrame;
	Uint32 lastFrame;
	bool   repeatAnimationForever;
	Uint32 animationRepeats;
	Uint32 animationInterval;
} animationstackentry;

#define ANIMATIONSTACKDEPTH 4

class ImageData
{
public:
	Uint32 fnHash;
	AWSprite* sprite;
	SDL_Surface* wholeSurface;

	ImageData()
	{
		fnHash = 0;
		sprite = NULL;
		wholeSurface = NULL;
	}

	ImageData(Uint32 hvalue, AWSprite* sp, SDL_Surface* ws)
	{
		fnHash = hvalue;
		sprite = sp;
		wholeSurface = ws;
	}

	~ImageData() {}
};

// ImageData - class required to keep track of shared bitmaps
class FileData : public ImageData
{
public:
	SDL_Surface** frames;
	Uint32 cols;
	Uint32 rows;
	int refCount;

	FileData()
	{
		fnHash = 0;
		sprite = NULL;
		wholeSurface = NULL;
		frames = NULL;
		rows = cols = 0;
		refCount = 1;	// Start with a ref count of 1
	}

	FileData(Uint32 hvalue, AWSprite* sp, SDL_Surface* ws, SDL_Surface** f, Uint32 cols, Uint32 rows)
	{
		fnHash = hvalue;
		sprite = sp;
		wholeSurface = ws;
		frames = f;
		this->cols = cols;
		this->rows = rows;
		refCount = 1;	// Start with a ref count of 1
	}

	~FileData() {}
};

// typedefs used by image sharing
typedef map<Uint32, FileData*> FileMap;
typedef FileMap::iterator FileMapIt;

typedef multimap<SDL_Surface*, ImageData*> ImageMap;
typedef ImageMap::iterator ImageMapIt;

// =====================================================
// Declaration of sprite class
// =====================================================
class AWSprite {
private:
	static FileMap fileMap;
	static ImageMap imageMap;
	static FileData* findImageByFilename(Uint32 filename);
	static FileData* removeFromFileMap(Uint32 filename);

	static ImageData* findImageBySurface(SDL_Surface* wholeSurface, AWSprite* sprite);
	static ImageData* removeFromImageMap(SDL_Surface* wholeSurface, AWSprite* sprite);

	static void restoreImageDataFromFileData(SDL_Surface* wholeSurface, FileData* fdata);

	bool   visible;
	int	   height, width;
	Uint32 firstFrame;
	Uint32 lastFrame;
	Uint32 currentFrame;
	bool   repeatAnimationForever;
	Uint32 animationRepeats;
	Uint32 lastMoveTicks;
	Uint32 lastAnimationTicks;
	Uint32 lastAccelerationTicks;
	Uint32 movementInterval;
	Uint32 animationInterval;
	Uint32 accelerationInterval;

	void push_animation_stack();
	void pop_animation_stack();
	void clear_animation_stack();
	int stackTop;
	animationstackentry stack[ANIMATIONSTACKDEPTH];
	bool autoDestructActivated;

	float speed;
	float angle;

	// If uses_per_pixel_alpha == true then the surface uses per-pixel alpha
	//  blending this value is set in the ctor and MUST not be subsequently
	//  changed.
	bool uses_per_pixel_alpha;

	// Defines whether this graphic is shared among several AWSprites
	// new to version 4.5
	bool sharedImage;

	// Callback for delayed actions
	callback delayAction;

protected:
	Uint32 howManyFrames;

	// Used to remember how images are laid out in wholeSurface - needed if
	//  we wish to be able to reset the frame images without reloading the
	//  image file - pkl
	Uint32 nbr_of_img_rows, nbr_of_img_cols;

	// These record the pixel coordinates for the collisions detected
	//  in pp_slow_collision and pp_collision
	Uint32 hit_x;
	Uint32 hit_y;

	// Attributes to enable control over transparency
	//  alphaLevel and alphaChange are floats to enable very slow fades when
	//  using new pulse methods
	// 0 = fully transparent : 255 fully opaque
	float alphaLevel;		// will be cast to int when used to set transparency
	float alphaChange;		// amount to change after each interval
	int	minPulseLevel;		// Default value 0 set in ctor
	int	maxPulseLevel;		// Default value 255 set in ctor
	int	pulseDir;			// +1 make more opaque; -1 make more transparent
	Uint32 pulseTime;		// Time in milliseconds to go from minPulseLevel to
	// MaxPulseLevel

	// Additional attributes to support auto pulse methods and to match
	// original naming style
	Uint32 lastPulseTicks;
	Uint32 pulseInterval;
	Sint32 pulseRepeats;		// 0 = forever
	Sint32 pulseRepeatCounter;	// initially set to pulseRepeats
	bool   repeatPulseForever;

	// Additional attributes for delayed actions
	Uint32 lastDelayTicks;
	Uint32 delayInterval;
	Sint32 delayRepeats;		// 0 = forever
	Sint32 delayRepeatCounter;	// initially set to delayRepeats
	bool   repeatDelayForever;

	// Required for synchronised actions
	Uint32 moveTicks;
	Uint32 animateTicks;
	Uint32 accelerateTicks;
	Uint32 pulseTicks;
	Uint32 delayTicks;

	// The create_frames() methods have been modified to return a pointer
	//  to an array of SDL_Surface pointers. This change enables the functions
	//  to be used to reset the sprite pixel. The 2 additional methods
	//  ..._with_alpha() are used for per-pixel transparency sprites.
	// The constructors have been modified to make use of the modified functions
	//  without affecting the final outcome - pkl
	// The methods from v3.5 have been made protected rather than private - pkl
	SDL_Surface** create_frames();
	SDL_Surface** create_frames(Uint32 cols, Uint32 rows);
	SDL_Surface** create_frames_with_alpha();
	SDL_Surface** create_frames_with_alpha(Uint32 cols, Uint32 rows);

public:
	// Creates a solid black sprite 10x10
	AWSprite();
	// Create a solid black sprite with user defined size
	AWSprite(Sint32 w, Sint32 h);
	AWSprite(Sint32 w, Sint32 h, Sint32 red, Sint32 green, Sint32 blue);

	// ctor for sprites that can use per-surface alpha
	AWSprite(char *imageFileName, Sint32 hmf, bool share = false);
	AWSprite(char *imageFileName, Sint32 cols, Sint32 rows, bool share = false);

	// ctor for sprites that use per-pixel alpha
	// The alpha mask image should be the same size as the image OR the same
	//  size as a single frame. Other sizes will not geneate an error but
	//  the results are unpredictable.
	// The alpha mask image should be a greyscale image where a black pixel
	//  (RGB 0,0,0) represents fully transparent and a white pixel (RGB 255,255,255)
	//  represents fully opaque. - pkl
	AWSprite(char *imageFileName, Sint32 hmf, char *alphaMaskFileName, bool share = false);
	AWSprite(char *imageFileName, Sint32 cols, Sint32 rows, char *alphaMaskFileName, bool share = false);

	void setImage(int w, int h, Uint32 red, Uint32 green, Uint32 blue);
	void setImage(char *imageFileName, Sint32 hmf = 1, bool share = false);
	void setImage(char *imageFileName, Sint32 cols, Sint32 rows, bool share = false);
	void setImage(char *imageFileName, Sint32 hmf, char *alphaMaskFileName, bool share = false);
	void setImage(char *imageFileName, Sint32 cols, Sint32 rows, char *alphaMaskFileName, bool share = false);

private:
	// Series of private functions called by constructors
	void makeDefaultImage(int w, int h, Uint32 red, Uint32 green, Uint32 blue);
	void makePrivateImage(char *imageFileName, Uint32 cols, Uint32 rows);
	void makePrivateImageWithAlpha(char *imageFileName, Uint32 cols, Uint32 rows, char *alphaMaskFileName);
	void makeSharedImage(char *imageFileName, Uint32 cols, Uint32 rows);
	void makeSharedImageWithAlpha(char *imageFileName, Uint32 cols, Uint32 rows, char *alphaMaskFileName);
	// Common code for makeSharedImage functions
	void useSharedImage(bool withAlpha, Uint32 cols, Uint32 rows, SDL_Surface* ws, SDL_Surface** f);
	// Release all SDL image  memory associated with this sprite or if shared reduce ref count
	void freeImageMemory();

	// Function to hash the filenames
	Uint32 hash(char* str, Uint32 seed = 5381);

public:

	// Use the greyscale image (mask) to perform pixel level transparency to
	// the SDL_Surface image - pkl
	void AWSprite::apply_alpha_mask(SDL_Surface* image, SDL_Surface* mask);

	// Gets RGB values from a colour value for a specific surface format
	void AWSprite::get_RGBA_components(SDL_Surface* s, Uint32 colour,
		Uint8 &red, Uint8 &green, Uint8 &blue, Uint8 &alpha);

	virtual ~AWSprite(void);	// now virtual - pkl

	void initialize_sprite();	// public so sprites can be re-initialised by user - pkl

	void clear_frame(Uint32 f, Uint32 colour);
	bool dead;
	Uint32 updateEverythingTicks;
	SDL_Surface *wholeSurface;
	SDL_Surface *surface;
	SDL_Surface **frames;

//**********************************************************	//5.0
	SDL_Texture **frameTextures;	//SDL2 now uses textures for rendering
	float m_angle;
	void rotateAndDraw(float ang, float rx, float ry);	
	void findRotatedTopBottom(float ang, float cx, float cy, int x1, int y1, int x2, int y2, int &lx, int &rx, int &ty, int &by);
	bool isPtInImage(float ang, float cx, float cy, float w, float h, float x, float y, Uint32 &origX, Uint32 &origY);
	int getLastFrame(){ return lastFrame; }
//**********************************************************

		Uint32 transparentColour;
	float worldX, worldY;
	float velX, velY;
	float accelX, accelY;
	float screenX, screenY;
	Uint32 percentHeight;
	Uint32 percentWidth;

	void set_surface_alpha(Uint32 value);
	void make_visible(); // Note you still have to draw() it!!
	void hide(); // This just calls make_invisible()
	void make_invisible();
	void set_visibility(bool newVisibility);
	bool toggle_visibility(); // This returns the value it SETS visible to

	void set_world_position(int wX, int wY);
	void set_world_position_x(int wX);
	void set_world_position_y(int wY);

	void set_world_position(float wX, float wY);
	void set_world_position_x(float wX);
	void set_world_position_y(float wY);

	void set_velocities(float vX, float vY);
	void set_x_velocity(float vX);
	void set_y_velocity(float vY);

	void set_accelerations(float aX, float aY);
	void set_x_acceleration(float aX);
	void set_y_acceleration(float aY);

	float get_x();
	float get_y();
	int get_height();
	int get_width();
	Uint32 get_currentFrame();
	void set_currentFrame(Uint32 frameNumber);
	float get_x_velocity();
	float get_y_velocity();
	float get_x_acceleration();
	float get_y_acceleration();

	// Pixel collision methods
	void set_hit_xy(Uint32 x, Uint32 y);
	Uint32 get_hit_x() { return hit_x; }
	Uint32 get_hit_y() { return hit_y; }

	// Retrieve intervals
	Uint32 get_move_interval();
	Uint32 get_animate_interval();
	Uint32 get_accelerate_interval();
	Uint32 get_pulse_interval();
	Uint32 get_delay_interval();

	// Get the current transparency for the sprite
	int get_surface_alpha();

	/*
	The following methods are used in moving and animating the sprite and are
	unchanged from version 3.5
	Every sprite is has totally independant movement so it is not possible to
	synchronise the movement/animation of a large number of sprites.
	*/
	// Movement methods
	virtual void movement_callback();
	void move();
	void auto_move(); // Move the sprite if it is time to do so
	void set_auto_move(Uint32 interval);

	// Animation methods
	virtual void animation_callback();
	void animate();
	void auto_animate(); // Animate the sprite if it is time to do so
	void set_auto_animate(Uint32 interval);
	// This version allows you to repeat the animation a limited number of times
	void set_auto_animate(Uint32 interval, Uint32 repeats);
	// Set a subset of the available frames to animate
	void set_animation_subset(Uint32 first, Uint32 last);

	// Acceleration methods
	virtual void acceleration_callback();
	void accelerate();
	void auto_accelerate(); // Accelerate the sprite if it is time to do so
	void set_auto_accelerate(Uint32 interval);

	/*Angular methods to set x and y velocities
	based on an angle and scalar value
	Sets speed and updates x and y velocities*/
	virtual void setSpeed(float newSpeed);
	//Sets angle and updates x and y velocities
	virtual void setAngle(float newAngle);

	float getSpeed() { return speed; }
	float getAngle() { return angle; }

	//End of Angular methods - Stephen Phillips

	// Pulse transparency methods : new to v4.0
	//  allows you to pulse the transparency of a sprite - these are used
	//  in same way as other v3.5 methods.
	// NOTE: pulsing per-pixel alpha sprites is not supported since we would
	//  have to process each pixel in the sprite and tis is too slow
	void pulse();
	void auto_pulse(void);			// Match existing animation style
	void set_auto_pulse(Uint32 interval, int repeats = 0);
	void set_pulse_time(Uint32 ticks = 10);	// pulse time milliseconds
	// NOTE the total pulse time = repeats * pulse time (ie ticks)
	// When defining the range it endAlpha maybe less OR more than
	//  startAlpha. If less then begin with fade-out if more begin with
	//  fade-in
	void set_pulse_range(int startAlpha, int endAlpha);

	// Delayed action methods : new to v4.0
	// used to provide delayed events
	void delay();
	void auto_delay(void);

	/*
	The following methods are used in moving and animating the sprite and are
	new to version 4.0
	Sprites are no longer independant. The auto_ methods have a parameter the
	value of which represents the number of ticks since the last time the
	method was called. This enables the movement/animation of a large number
	of sprites to be synchronised.
	*/
	void auto_move(Uint32 delta);
	void auto_animate(Uint32 delta);
	void auto_accelerate(Uint32 delta);
	void auto_pulse(Uint32 delta);
	void auto_delay(Uint32 delta);

	// Set up the destruction animation of the sprite
	// This cannot be countermanded!! Once the "countdown" starts, the sprite
	//  WILL die...
	void set_auto_destruct(Uint32 first, Uint32 last, Uint32 interval, Uint32 repeats);
	void set_auto_destruct(Uint32 first, Uint32 last, Uint32 interval);

	/*
	Methods to draw to an SDL surface
	*/
	void draw();
	void draw_onto(SDL_Surface *onto);

	void set_transparent_colour(int R, int G, int B);

	// Set screenX and screenY according to where in the world the top-left pixel of the screen is
	void map_world_to_screen(int topleftX, int topleftY);
	void map_world_to_screen(); // As above, but assume top-left is (0,0) (ie non-scrolling games)

	/*
	Every auto sprite should call 1 of the 4 update_everything methods once per game cycle.
	The first 2 use the v3.5 methods for animating the sprite. The last 2 are used for
	synchronised sprite movement v4.0 and 'delta' is the number of milliseconds since
	the last time the method was called e.g. for a sprite called 'monster'.

	Uint32 currTicks;
	Uint32 deltaTicks;
	Uint32 lastTicks = SDL_GetTicks();

	gameOver = false;
	while(!gameOver)
	{
	SDL_Delay(10);
	currTicks = SDL_GetTicks();
	deltaTicks = currTicks - lastTicks;
	lastTicks = currTicks;

	monster->update_everything(deltaTicks);
	}

	*/
	// Set screenX and screenY according to where in the world the top-left pixel
	// of the screen is
	void update_everything(int topleftX, int topleftY);
	// As above, but assume top-left is (0,0) (ie non-scrolling games)
	void update_everything();

	// Set screenX and screenY according to where in the world the top-left pixel
	//  of the screen is
	void update_everything(Uint32 delta, int topLeftX, int topLeftY);
	// As above, but assume top-left is (0,0) (ie non-scrolling games)
	void update_everything(Uint32 delta);

	// Collision methods - callback methods are available in the event of
	//  a collision. Note that when a collision is deteced the callback method
	//  is called for BOTH sprites.
	// Pixel level detection works with all sprites irrespective of transparency
	//  mechanism i.e. per-surface or per-pixel.
	bool bb_collision(AWSprite *spriteB);
	bool bb_80_collision(AWSprite *spriteB);
	bool pp_slow_collision(AWSprite *spriteB);
	bool pp_collision(AWSprite *spriteB);
	bool pp_collision_32(AWSprite *spriteB);
	bool pp_collision_16(AWSprite *spriteB);	// does not work with per-pixel alpha
	Uint32 get_pixel_32(Uint32 x, Uint32 y);	// used by pp_slow_collision

	/*Radial collision by Christopher Rae

	1. Tests whether objects are on screen
	2. Tests whether objects are moving torwards each other
	3.If the 2 sprites are colliding their (position^2 is < their radius^2)
	using squared because it is faster than sqrt.
	If step 3 is reached the callback functions for radial collision will be called
	*/
	bool radial_collision(AWSprite *spriteB);

	bool bb_collision(AWSprite &spriteB) { return bb_collision(&spriteB); }
	bool bb_80_collision(AWSprite &spriteB) { return bb_80_collision(&spriteB); }
	bool pp_slow_collision(AWSprite &spriteB) { return pp_slow_collision(&spriteB); }
	bool pp_collision(AWSprite &spriteB) { return pp_collision(&spriteB); }
	bool pp_collision_32(AWSprite &spriteB) { return pp_collision_32(&spriteB); }
	//	bool radial_collision(AWSprite *spriteB) { return radial_collision(&spriteB); }

	// Virtual methods for end_of_pulse and collision events you should
	//  override these in derived (child) classes if you want to provide
	//  class specific actions
	virtual void pulse_end_callback() {}
	virtual void bb_collision_callback() {}
	virtual void pp_collision_callback() {}

	/*Radial callback method using an AWSprite here is an example of how
	to handle a callback in a class:

	void Ball::radial_collision_callback(AWSprite *spriteB)
	{
	//This code finds the angle that 2 ball objects hitting each other should bounce off each other.
	float xDiff = worldX - spriteB->get_x();
	float yDiff = worldY - spriteB->get_y();
	float dir = 0;
	dir = atan2(yDiff, xDiff); //atan2 will work out the angle which the balls collide (it does the trigonometry for you!)
	setAngle(dir);
	//what you want objects to do here e.g. speed up, slow down etc.
	}
	*/
	virtual void radial_collision_callback(AWSprite *spriteB){}
	virtual void radial_collision_callback(){}

	// This method makes a circular area of pixels centerd arround x, y
	//  transparent. This works fine with sprites using per-pixel transparency.
	// To work correctly on per-surface transparency you must have previously set
	//  a transparent colour.
	void AWSprite::bite(int x, int y, int biteRadius);
	// As above but centred arrond hit_x, hit_y
	void AWSprite::bite(int biteRadius);
	// Restore the sprite pixel data back to its original state i.e. undos bite
	void restore_images();

	// This simply returns the visible attribute. Notice that it doesn't say whether
	//  the sprite is on the screen. All it says is that the sprite would be visible
	//  if it was within the boundary of the screen.
	bool is_visible();
	// This simply returns true if the sprite is currently on screen. This function
	//  also takes into account visibility. If the sprite is not visible, it is
	//  considered to be OFF-screen.
	bool is_on_screen();
	bool is_on_screen(Uint32 topLeftX, Uint32 topLeftY);

	// Set the delay event callback - it replaces any prior delay event calback
	// Parameters:
	// action - pointer to event handler ( a NULL value cancels current delay event)
	// dticks - time between events
	// times - number of times to repeat event (0 = cancel current delay event,
	//			<0 repeat forever or until cancelled)
	void set_delay_event(callback action, Uint32 dticks, Sint32 times);

	// The following virtual methods can be over-ridden in your derived
	//  classes to provide additional methods that can be used
	//  as delayed action events
	virtual void e0_handler() {}
	virtual void e1_handler() {}
	virtual void e2_handler() {}
	virtual void e3_handler() {}
	virtual void e4_handler() {}
	virtual void e5_handler() {}
	virtual void e6_handler() {}
	virtual void e7_handler() {}
	virtual void e8_handler() {}
	virtual void e9_handler() {}

	// These methods allow you to get the memory address of the objects
	//  event handler from outside the class. This allows the set_delay_event
	//  method to be called from outside of the class definition.
	virtual callback addr_e0_handler() { return &AWSprite::e0_handler; }
	virtual callback addr_e1_handler() { return &AWSprite::e1_handler; }
	virtual callback addr_e2_handler() { return &AWSprite::e2_handler; }
	virtual callback addr_e3_handler() { return &AWSprite::e3_handler; }
	virtual callback addr_e4_handler() { return &AWSprite::e4_handler; }
	virtual callback addr_e5_handler() { return &AWSprite::e5_handler; }
	virtual callback addr_e6_handler() { return &AWSprite::e6_handler; }
	virtual callback addr_e7_handler() { return &AWSprite::e7_handler; }
	virtual callback addr_e8_handler() { return &AWSprite::e8_handler; }
	virtual callback addr_e9_handler() { return &AWSprite::e9_handler; }

	/* HOW TO USE THE TIMER EVENT
	This class enables you to specify delay events specific objects of classes
	that inherits from AWSprite. Using delay events is more complex than the
	callback methods such as pp_collision_callback() because you can specify the
	callback method at runtime.
	The following eplains how to use this feature. It assumes that you have a
	class called 'Alien' which inherits from AWSprite.
	We can override any of the handler methods specified above, in this example
	we will override the first two e.g.

	class Alien : public AWSprite
	{
	public:
	void e0_handler();
	void e1_handler();
	}

	In the implementation file

	void Alien::e0_handler() {
	set_x_velocity(-get_x_velocity());	// reverse x direction
	}

	void Alien::e1_handler() {
	set_y_velocity(-get_y_velocity());	// reverse y direction
	}

	If we want to reverse the x velocity every 1.5 seconds and repeat this 9
	times we can use the following statement anywhere WITHIN the alien class

	set_delay_event(&AWSprite::e0_handler, 1500, 9);

	To reverse the y velocity every 1.25 seconds forever we can use:

	set_delay_event(&AWSprite::e1_handler, 1250, -1);

	Setting the delay event from outside of the class is not much harder. Assume
	in your Game class (you do have one don't you!) you have a dynamically
	created object of type Alien which is referenced by an Alien pointer variable
	called an_alien e.g.

	AWSprite* an_alien = new Alien();

	To set a delay event using an_alien use the following statement from outside the
	Alien class

	an_alien->set_delay_event(addr_e0_handler(), 1500, 9);

	*/
	//
	// Added to draw lines 
	void drawLine(int x0, int y0, int x1, int y1, unsigned int colour);
	void drawFilledRect(SDL_Rect &r, unsigned int colour);
	void drawCross(int x, int y, int len);
};

#endif
