#include "GameBase.h"
#include "TrueTypeFont.h"

SDL_Surface* GameBase::screen_surface = NULL;
SDL_Surface* GameBase::background_surface = NULL;

SDL_Window* GameBase::window = NULL;	//v1.1
SDL_Renderer* GameBase::renderer = NULL;
SDL_Texture* GameBase::texture = NULL;
SDL_Texture* GameBase::bgTexture = NULL;	//****

/*
Initialise the SDL.
SDL functions  return 0 on success and <0 for failure so do the same here.
return 0 if successful
*/
int GameBase::initSDL(int width, int height, string title){	//v1.1
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0){
		cout << "Unable to initialise SDL\n\nPROGRAM TERMINATED\n\n";
		system("pause");
		return -1;
	}
	// So we have managed to initialise the SDL systems so make sure we
	// uninitialise them when the program terminates
	atexit(SDL_Quit);

//**************************************************	//v1.1 Based on lazyfoo.net SDL2 tutorials
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (window == NULL){
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}else{
//		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	//use GPU & sync with screen
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);	//use GPU
		if (renderer == NULL){
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			return false;
		}else{
			//Initialize renderer color
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

			//Initialize PNG loading
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags)){
				printf("SDL_image could not initialise! SDL_image Error: %s\n", IMG_GetError());
				return false;
			}

			//Initialize SDL_ttf 
			if( TTF_Init() == -1 ) { //v1.2
				printf( "SDL_ttf could not initialise! SDL_ttf Error: %s\n", TTF_GetError() ); 
				return false; 
			}
		}
	}
//**************************************************
	// Now initialise the screen. This will initialise event handling as well

	screen_surface = SDL_GetWindowSurface(window);	//v1.1
	// Make sure we have got the screen before moving on
	if (screen_surface == NULL) {
		cout << "Unable to initialise display window\n\nPROGRAM TERMINATED\n\n";
		system("pause");
		return -1;
	}
	// Resize the console window to make it less obvious
	console.setSize(50, 30, false);
	// Get default grey background
	background_surface = getSurface(width, height, 0, 0, 0);

	// SDL functions return 0 on success
	return 0;
}

SDL_Surface* GameBase::getSurface(int width, int height, int red, int green, int blue){
	Uint32 rmask, gmask, bmask, amask;
	/* SDL interprets each pixel as a 32-bit number, so our masks must depend
	   on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif
	SDL_Surface* newSurface = SDL_GetWindowSurface(window);	//v1.1
	SDL_FillRect(newSurface, NULL,
		SDL_MapRGB(newSurface->format, red, green, blue));
	return newSurface;
}

GameBase::GameBase(void){
	screen = screen_surface;
	bg = background_surface;
	width = screen->w;
	height = screen->h;
	mouseX = mouseY = 0;
	cout << "Screen size " << width << " x " << height << endl;

	m_camera.x = m_camera.y = 0;	//v1.1
	m_camera.w = width;
	m_camera.h = height;
	bgTexture = NULL;
	ttFont = new TrueTypeFont(this, "SDL/9Systema.ttf");	//v1.2
	kbArray = SDL_GetKeyboardState(NULL);	//get ALL key states
}

GameBase::~GameBase(void){
	SDL_FreeSurface(bg);	
	SDL_FreeSurface(screen_surface);	
	SDL_FreeSurface(background_surface);
	//Free loaded image 
	SDL_DestroyTexture(texture);	//v1.1
	texture = NULL;
	SDL_DestroyTexture(bgTexture);	//v1.1
	bgTexture = NULL;
	
	//Destroy window 
	SDL_DestroyRenderer( renderer ); 
	SDL_DestroyWindow( window ); 
	window = NULL; 
	renderer = NULL; 

	//Delete the font that was used
	delete ttFont;	//v1.2
	//Quit SDL_ttf
	TTF_Quit();

	//Quit SDL subsystems 
	IMG_Quit();
	SDL_Quit();
}

void GameBase::start(){
	gameLoop();
}
void GameBase::setFontSizeAndColour(int fs, int r, int g, int b){	//v1.2
	ttFont->setFontSizeAndColour(fs, r, g, b);
}

void GameBase::setBackground(int red, int green, int blue){
	bg = getSurface(width, height, red, green, blue);
}

//void GameBase::resetBackground(char *imageFileName){
//	if (bgTexture != NULL){
//		bg = IMG_Load(imageFileName);	//v1.1
//		if (bg != NULL){
//			SDL_DestroyTexture(bgTexture);	//****
//			SDL_SetColorKey(bg, SDL_TRUE, SDL_MapRGB(bg->format, 0, 0xFF, 0xFF));
//
//			//Create texture from surface pixels
//			texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);	//v1.1
//
//			if (texture == NULL)	{
//				printf("Unable to create texture from %s! SDL Error: %s\n", imageFileName, SDL_GetError());
//			}
//			bgTexture = SDL_CreateTextureFromSurface(renderer, bg);
//		}
//		else{
//			cout << "Unable to locate image file " << imageFileName << endl;
//		}
//	}
//}

void GameBase::setBackground(char *imageFileName){
	if (bgTexture != NULL)	//v1.2. Release memory, if req'd, before creating
		SDL_DestroyTexture(bgTexture);	
	if (bg)		//v1.2
		SDL_FreeSurface(bg);
	// Get the background image
	bg = IMG_Load(imageFileName);	//v1.1
	if (bg != NULL){
		SDL_SetColorKey(bg, SDL_TRUE, SDL_MapRGB(bg->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);	//v1.1

		if (texture == NULL)
			printf("Unable to create texture from %s! SDL Error: %s\n", imageFileName, SDL_GetError());
		bgTexture = SDL_CreateTextureFromSurface(renderer, bg);	
	}
	else
		cout << "Unable to locate image file " << imageFileName << endl;
}

bool GameBase::addSound(char* filename, string name){
	return AudioManager::getInstance()->addSound(filename, name);
}

bool GameBase::addSound(char* filename, string name, int volDivider) {	//v1.1
	return AudioManager::getInstance()->addSound(filename, name, volDivider);
}

bool GameBase::addMusic(char* filename) {	//v1.2
	return AudioManager::getInstance()->addMusic(filename);
}

void GameBase::playSound(string name) {
	AudioManager::getInstance()->playSound(name);
}

void GameBase::playMusic(int volDivider) {	//v1.2
	AudioManager::getInstance()->playMusic(volDivider);
}


void GameBase::getUserInput(){
	SDL_Event an_event;
	if (SDL_PollEvent(&an_event)){
		keyDown = keyUp = 0;
		switch (an_event.type)
		{
		case SDL_QUIT:
			gameover = true;
			break;
		case SDL_KEYDOWN:
			ctrlPressed = ((an_event.key.keysym.mod & KMOD_CTRL) == KMOD_CTRL);
			shiftPressed = ((an_event.key.keysym.mod & KMOD_SHIFT) == KMOD_SHIFT);
			altPressed = ((an_event.key.keysym.mod & KMOD_ALT) == KMOD_ALT);
			keyDown = an_event.key.keysym.sym;
			SDL_PumpEvents();	//v1.2. Update kbArray, init in SDL_GetKeyboardState
			gameover = ((keyDown & SDLK_ESCAPE) == SDLK_ESCAPE);
			onKeyPressed();
			break;
		case SDL_KEYUP:
			ctrlPressed = ((an_event.key.keysym.mod & KMOD_CTRL) == KMOD_CTRL);
			shiftPressed = ((an_event.key.keysym.mod & KMOD_SHIFT) == KMOD_SHIFT);
			altPressed = ((an_event.key.keysym.mod & KMOD_ALT) == KMOD_ALT);
			keyUp = an_event.key.keysym.sym;
			onKeyReleased();
			break;
		case SDL_MOUSEMOTION:
			pmouseX = mouseX;
			pmouseY = mouseY;
			mouseX = an_event.motion.x;
			mouseY = an_event.motion.y;
			onMouseMoved();
			break;
		case SDL_MOUSEBUTTONDOWN:
			mouseButton = an_event.button.button; // SDL_BUTTON_LEFT)
			mouseX = an_event.button.x;
			mouseY = an_event.button.y;
			onMousePressed();
			break;
		case SDL_MOUSEBUTTONUP:
			mouseButton = an_event.button.button; // SDL_BUTTON_LEFT)
			mouseX = an_event.button.x;
			mouseY = an_event.button.y;
			onMouseReleased();
			break;
		default:
			// An event that we do not need to process
			break;
		} // end of poll event switch
	} // end if
}

void GameBase::gameLoop(){
	gameover = false;
	while (!gameover) {
		SDL_Delay(1);
		getUserInput();
		logic();

		SDL_RenderClear(renderer);	//v1.1
		SDL_RenderCopy(renderer, bgTexture, &m_camera, NULL);	//render background

		draw();

		SDL_RenderPresent(renderer);	//v1.1
		SDL_DestroyTexture(texture);
		texture = NULL;
		post();
	}
}

int GameBase::enableKeyRepeat(int first, int repeat){
//	return SDL_EnableKeyRepeat(first, repeat);	//v1.1 func NOT req'd in SDL2
	return 0;
}

int GameBase::print(string value, int x, int y){	//v1.2. Colour param removed
	ttFont->renderFont(value.c_str(), x, y);
	return x + ttFont->getTextWidth();
}

int GameBase::print(const char* value, int x, int y){	//v1.2
	ttFont->renderFont(value, x, y);
	return x + ttFont->getTextWidth();
}

int GameBase::print(int value, int x, int y){	//v1.2
	ttFont->renderFont(value, x, y);
	return x + ttFont->getTextWidth();
}

int GameBase::print(float value, int x, int y){	//v1.2
	ttFont->renderFont(value, x, y);
	return x + ttFont->getTextWidth();

}
/////////////////////////////////////////////////////////////////////////////////////
/*  Bresenham Line algorithm.  Taken from Wikipedia
	draw line from x0,y0 to x1, y1 in specified colour.	v1.2.
*/
void GameBase::drawDottedLine(int x0, int y0, int x1, int y1, int r, int g, int b, int a) {
	int dx, dy, sx, sy, err, e2, c = 0;
	unsigned int n = 0;
	dx = abs(x1 - x0);
	dy = abs(y1 - y0);

	sx = -1;
	if (x0 < x1) sx = 1;
	sy = -1;
	if (y0 < y1) sy = 1;
	err = dx - dy;
	//int maxV = dx > dy ? dx : dy;
	//SDL_Point *points = new SDL_Point[maxV << 1];	//store ALL pixels 
	SDL_Point *points = new SDL_Point[(int)sqrt(dx*dx+dy*dy)];	//store ALL pixels 

	do {	//     setPixel(x0,y0)
		if (n++ % 4 == 0) {	//draw every 4 pixels
			points[c].x = x0; 
			points[c++].y = y0; //store x,y
		}
		e2 = err << 1; //multiply err by 2
		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (e2 <  dx) {
			err += dx;
			y0 += sy;
		}
	} while (x0 != x1 || y0 != y1);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);	
	SDL_RenderDrawPoints(renderer, points, c); //faster than calling each time
	delete[] points;
}

void GameBase::drawLine(int x0, int y0, int x1, int y1, int r, int g, int b, int a) {	//v1.2
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderDrawLine(renderer, x0, y0, x1, y1); //Renders at line in curr colour
}

void GameBase::drawCircle(int cx, int cy, int rad, int r, int g, int b, int a, bool fillIn) {	//v1.2
	int c, rc;
	SDL_Rect *rects = NULL;	//for filling in circle
	if(fillIn)
		rects = new SDL_Rect[rad * 3];	//for filling in circle
	SDL_Point *points = new SDL_Point[rad * 7];	//store ALL pixels 
	createCircle(points, cx, cy, rad, c, rects, rc);	//store ALL pixels 
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	if(fillIn)
		SDL_RenderFillRects(renderer, rects, rc);
	else
		SDL_RenderDrawPoints(renderer, points, c); //array is faster than calling each time
	delete[] points;
	delete[] rects;
}

void GameBase::drawFilledRect(int tlx, int tly, int w, int h, int r, int g, int b, int a) {
	SDL_Rect rect;
	rect.x = tlx; rect.y = tly; rect.w = w; rect.h = h;
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderFillRect(renderer, &rect);
}

//
// Uses Bresenham's Circle drawing algorithm.  
// Based on: https://www.tutorialspoint.com/computer_graphics/circle_generation_algorithm.htm
// 
void GameBase::createCircle(SDL_Point *points, int cx, int cy, int rad, int &c, SDL_Rect *rects, int &rCount){	//v1.2
	int x = 0, y = rad, d = 3 - 2 * rad;
	c = rCount = 0;
	do {
		update8CirclePts(points, c, cx, cy, x, y, rects, rCount);
		if (d < 0)
			d += (x << 2) + 6;	//<<2 multiplies by 4
		else {
			d += ((x - y) << 2) + 10;
			y--;
		}
		c += 8;
	} while (++x < y);
}

void GameBase::update8CirclePts(SDL_Point *pts, int &currCount, int cx, int cy, int x, int y, SDL_Rect *rects, int &rCount){	//v1.2
	pts[currCount+0].x = cx + x; pts[currCount+0].y = cy + y; //upd 2 pts in each quadrant
	pts[currCount+1].x = cx + x; pts[currCount+1].y = cy - y;
	pts[currCount+2].x = cx - x; pts[currCount+2].y = cy + y;
	pts[currCount+3].x = cx - x; pts[currCount+3].y = cy - y;
	pts[currCount+4].x = cx + y; pts[currCount+4].y = cy + x;
	pts[currCount+5].x = cx + y; pts[currCount+5].y = cy - x;
	pts[currCount+6].x = cx - y; pts[currCount+6].y = cy + x;
	pts[currCount+7].x = cx - y; pts[currCount+7].y = cy - x;
//
//rects for drawing filled in rects to fill circle
	if (rects) { //if NOT null update.  This can significantly slow FPS!
		rects[rCount].x = cx - x; rects[rCount].y = cy - y;	//top quad. In SDL y +ve downward
		rects[rCount].w = x << 1; rects[rCount].h = y - x + 1;

		rects[++rCount].x = cx - x; rects[rCount].y = cy + y;	//bot quad
		rects[rCount].w = x << 1; rects[rCount].h = 1 - x - y;

		rects[++rCount].x = cx - y; rects[rCount].y = cy - x;	//left quad
		rects[rCount].w = 1 + y - x; rects[rCount].h = x << 1;

		rects[++rCount].x = cx + x; rects[rCount].y = cy - x;	//right quad
		rects[rCount].w = y - x - 1; rects[rCount++].h = x << 1;
	}
}
/////////////////////////////////////////////////////////////////////////////////////
int GameBase::getFontHeight() {	//v1.2	
	return ttFont->getFontHeight();
}
