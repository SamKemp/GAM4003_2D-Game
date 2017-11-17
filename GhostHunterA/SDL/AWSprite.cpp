// Copyright Andrew Williams, Peter Lager, Steve Manning and the University of Bolton
// Do what you like with the program, but please include your
// sourcecode with any distribution and retain my copyright notices

/* Version 5.0
	See AWSprite.h for information on changes between versions
	*/

#include "AWSprite.h"

// This is used in bb_80_collision(). In fact, bb_80_collision only
//  does 80% C.D. if we set COLLISIONDIVISOR to 10. If we set this
//  constant to 20, we would have 90% collision detection. If we set
//  it to 5, we would have 60% collision detection
#define COLLISIONDIVISOR (10)

// ========================================================================================
// ============= ATTRIBUTES AND FUNCTIONS FOR BITMAP SHARING STARTS HERE ==================
// ========================================================================================

FileMap AWSprite::fileMap;
ImageMap AWSprite::imageMap;

// String hash algorithm djb2 - Dan Bernstein
// adapted by Peter Lager to enable multiple char
// arrays to be hased.
Uint32 AWSprite::hash(char* str, Uint32 seed) {
	Uint32 hash = seed;
	int pos = 0, c = (int)str[pos++];

	while (c != 0)	{
		hash = ((hash << 5) + hash) + c;
		c = (int)str[pos++];
	}
	return hash;
}

FileData* AWSprite::findImageByFilename(Uint32 filename) {
	FileMapIt it = fileMap.find(filename);
	FileData* fdata = NULL;
	if (it != fileMap.end()) {
		fdata = it->second;
	}
	return fdata;
}

FileData* AWSprite::removeFromFileMap(Uint32 filename) {
	FileMapIt it = fileMap.find(filename);
	FileData* fdata = NULL;
	if (it != fileMap.end()) {
		fdata = it->second;
		fileMap.erase(it);
	}
	return fdata;
}

ImageData* AWSprite::findImageBySurface(SDL_Surface* wholeSurface, AWSprite* sprite) {
	ImageMapIt it, p2 = imageMap.upper_bound(wholeSurface);
	ImageData* idata = NULL;
	for (it = imageMap.lower_bound(wholeSurface); it != p2; ++it)	{
		if (it->second->sprite == sprite)		{
			idata = it->second;
			return idata;
		}
	}
	return idata;
}

ImageData* AWSprite::removeFromImageMap(SDL_Surface* wholeSurface, AWSprite* sprite) {
	ImageMapIt it = imageMap.lower_bound(wholeSurface);
	ImageMapIt end = imageMap.upper_bound(wholeSurface);
	ImageData* idata = NULL;
	for (; it != end; ++it)
	{
		if (it->second->sprite == sprite)
		{
			idata = it->second;
			imageMap.erase(it);
			return idata;
		}
	}
	return idata;
}

// ========================================================================================
// ============= CONSTRUCTORS START HERE  =================================================
// ========================================================================================

AWSprite::AWSprite(){
	makeDefaultImage(10, 10, 0, 0, 0);
}

// Construct a sprite using SDL_CreateRGBSurface
//  In other words, no image file is used. NB this
//  must be a one-frame sprite and furthermore, the
//  transparent colour will perforce be BLACK
AWSprite::AWSprite(Sint32 w, Sint32 h) {
	makeDefaultImage(w, h, 0, 0, 0);
}

AWSprite::AWSprite(Sint32 w, Sint32 h, Sint32 red, Sint32 green, Sint32 blue){
	makeDefaultImage(w, h, red, green, blue);
}

// Construct a sprite from a bitmap file and a number of Frames
AWSprite::AWSprite(char *imageFileName, Sint32 hmf, bool share) {
	if (share)
		makeSharedImage(imageFileName, hmf, 1);
	else
		makePrivateImage(imageFileName, hmf, 1);
}

// This constructor is used for a sprite where the frames are not in a long line
//  In other words, we have to say how many columns and how many rows the sprite is
//  divided into
AWSprite::AWSprite(char *imageFileName, Sint32 cols, Sint32 rows, bool share) {
	if (share)
		makeSharedImage(imageFileName, cols, rows);
	else
		makePrivateImage(imageFileName, cols, rows);
}

// Construct a sprite from a bitmap file and a number of Frames
// Uses greyscale mask image to create per-pixel alpha
AWSprite::AWSprite(char *imageFileName, Sint32 hmf, char *alphaMaskFileName, bool share) {
	if (share)
		makeSharedImageWithAlpha(imageFileName, hmf, 1, alphaMaskFileName);
	else
		makePrivateImageWithAlpha(imageFileName, hmf, 1, alphaMaskFileName);
}

// This constructor is used for a sprite where the frames are not in a long line
//  In other words, we have to say how many columns and how many rows the sprite is
//  divided into. Uses greyscale mask image to create per-pixel alpha
AWSprite::AWSprite(char *imageFileName, Sint32 cols, Sint32 rows, char *alphaMaskFileName, bool share) {
	if (share)
		makeSharedImageWithAlpha(imageFileName, cols, rows, alphaMaskFileName);
	else
		makePrivateImageWithAlpha(imageFileName, cols, rows, alphaMaskFileName);
}

void AWSprite::setImage(int w, int h, Uint32 red, Uint32 green, Uint32 blue){
	freeImageMemory();
	makeDefaultImage(w, h, red, green, blue);
}

void AWSprite::setImage(char *imageFileName, Sint32 hmf, bool share){
	freeImageMemory();
	if (share)
		makeSharedImage(imageFileName, hmf, 1);
	else
		makePrivateImage(imageFileName, hmf, 1);
}

void AWSprite::setImage(char *imageFileName, Sint32 cols, Sint32 rows, bool share){
	freeImageMemory();
	if (share)
		makeSharedImage(imageFileName, cols, rows);
	else
		makePrivateImage(imageFileName, cols, rows);
}

void AWSprite::setImage(char *imageFileName, Sint32 hmf, char *alphaMaskFileName, bool share){
	freeImageMemory();
	if (share)
		makeSharedImageWithAlpha(imageFileName, hmf, 1, alphaMaskFileName);
	else
		makePrivateImageWithAlpha(imageFileName, hmf, 1, alphaMaskFileName);
}

void AWSprite::setImage(char *imageFileName, Sint32 cols, Sint32 rows, char *alphaMaskFileName, bool share){
	freeImageMemory();
	if (share)
		makeSharedImageWithAlpha(imageFileName, cols, rows, alphaMaskFileName);
	else
		makePrivateImageWithAlpha(imageFileName, cols, rows, alphaMaskFileName);
}

void AWSprite::makeDefaultImage(int w, int h, Uint32 red, Uint32 green, Uint32 blue){
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
	sharedImage = false;
	frames = NULL;

	frameTextures = NULL;	//v5.0
	m_angle = 0.0f;

	// Added to prevent exception errors for restore_images() method - pkl
	nbr_of_img_rows = 1;
	nbr_of_img_cols = 1;
	uses_per_pixel_alpha = false;

	width = w;
	height = h;

	howManyFrames = 1;
	firstFrame = lastFrame = 0;
	frames = new SDL_Surface*[1];
	wholeSurface = SDL_GetWindowSurface(GameBase::window);	//v5.0

	SDL_Surface *temp;
	temp = SDL_CreateRGBSurface(wholeSurface->flags, w, h, 32, rmask, gmask, bmask, amask);	//v5.0
	wholeSurface = SDL_ConvertSurface(temp, wholeSurface->format, NULL);
	SDL_FreeSurface(temp);

	SDL_FillRect(wholeSurface, NULL,
		SDL_MapRGB(wholeSurface->format, red, green, blue));

	howManyFrames = 1;
	firstFrame = 0;
	lastFrame = howManyFrames - 1;

	frames = create_frames(1, 1);
	set_currentFrame(0);
	initialize_sprite();

	// These are used for bb_80_collision detection
	percentHeight = height / COLLISIONDIVISOR;
	percentWidth = width / COLLISIONDIVISOR;

	clear_animation_stack();
	push_animation_stack();
}

void AWSprite::makePrivateImageWithAlpha(char *imageFileName, Uint32 cols, Uint32 rows, char *alphaMaskFileName) {
	sharedImage = false;
	wholeSurface = NULL;
	frames = NULL;

	m_angle = 0.0f;	//v5.0

	nbr_of_img_rows = rows;
	nbr_of_img_cols = cols;
	uses_per_pixel_alpha = true;

	// Load in the image and ensure there is an alpha channel
	SDL_Surface* temp = IMG_Load(imageFileName);
//	wholeSurface = SDL_DisplayFormatAlpha(temp);

	SDL_Surface *screen = SDL_GetWindowSurface(GameBase::window);	//v5.0
	SDL_PixelFormat fmt = *screen->format;
	fmt.Amask = 0xff000000;	//set alpha
	SDL_FreeSurface(screen);

	wholeSurface = SDL_ConvertSurface(temp, &fmt, NULL);	//v5.0
	SDL_FreeSurface(temp);


	temp = IMG_Load(alphaMaskFileName);
//	SDL_Surface* maskSurface = SDL_DisplayFormatAlpha(temp);
//	SDL_Surface* maskSurface = SDL_ConvertSurface(temp, screen->format, NULL);
	SDL_Surface* maskSurface = SDL_ConvertSurface(temp, &fmt, NULL);
	SDL_FreeSurface(temp);

	apply_alpha_mask(wholeSurface, maskSurface);
	SDL_FreeSurface(maskSurface);

	// Divide the image into frames
	howManyFrames = cols*rows;
	firstFrame = 0;
	lastFrame = howManyFrames - 1;
	height = wholeSurface->h / rows;
	width = wholeSurface->w / cols;
	frames = create_frames_with_alpha(cols, rows);

	set_currentFrame(0);
	initialize_sprite();

	// These are used for bb_80_collision detection
	percentHeight = height / COLLISIONDIVISOR;
	percentWidth = width / COLLISIONDIVISOR;

	clear_animation_stack();
	push_animation_stack();
}

void AWSprite::makePrivateImage(char *imageFileName, Uint32 cols, Uint32 rows) {
	sharedImage = false;
	wholeSurface = NULL;
	frames = NULL;

	m_angle = 0.0f;	//v5.0

	nbr_of_img_rows = rows;	//pkl
	nbr_of_img_cols = cols;	//pkl
	uses_per_pixel_alpha = false;	//pkl

	// Load in the image
	SDL_Surface* temp = IMG_Load(imageFileName);
	wholeSurface = SDL_ConvertSurface(temp, temp->format, NULL);	//v5.0
	SDL_FreeSurface(temp);

	// Divide the image into frames
	howManyFrames = cols*rows;
	firstFrame = 0;
	lastFrame = howManyFrames - 1;
	height = wholeSurface->h / rows;
	width = wholeSurface->w / cols;
	frames = create_frames(cols, rows);

	set_currentFrame(0);
	initialize_sprite();

	// These are used for bb_80_collision detection
	percentHeight = height / COLLISIONDIVISOR;
	percentWidth = width / COLLISIONDIVISOR;

	clear_animation_stack();
	push_animation_stack();
}

void AWSprite::makeSharedImageWithAlpha(char *imageFileName, Uint32 cols, Uint32 rows, char *alphaMaskFileName) {
	// convert char[] to string
	Uint32 fileNameHashed = hash(imageFileName);
	fileNameHashed = hash(alphaMaskFileName, fileNameHashed);

	// See if this image has been used already
	FileData* fdata = findImageByFilename(fileNameHashed);
	ImageData* idata = NULL;

	if (fdata == NULL) {			// New bitmap file
		// File unused : so create private image from scratch then make shared
		makePrivateImageWithAlpha(imageFileName, cols, rows, alphaMaskFileName);
		sharedImage = true;
		// Create and log iData for later use
		idata = new ImageData(fileNameHashed, this, wholeSurface);
		imageMap.insert(make_pair(wholeSurface, idata));
		// Create and log fData for later use
		fdata = new FileData(fileNameHashed, this, wholeSurface, frames, nbr_of_img_cols, nbr_of_img_rows);
		fileMap.insert(make_pair(fileNameHashed, fdata));
	}
	else {						// bitmap file has already been loaded
		useSharedImage(true, fdata->cols, fdata->rows, fdata->wholeSurface, fdata->frames);
		// Create and log iData for later use
		idata = new ImageData(fileNameHashed, this, wholeSurface);
		imageMap.insert(make_pair(wholeSurface, idata));
		fdata->refCount++; // increment reference counter
	}
}

void AWSprite::makeSharedImage(char *imageFileName, Uint32 cols, Uint32 rows) {
	// convert char[] to string
	Uint32 fileNameHashed = hash(imageFileName);

	// See if this image has been used already
	FileData* fdata = findImageByFilename(fileNameHashed);
	ImageData* idata = NULL;

	if (fdata == NULL) {			// New bitmap file
		// File unused : so create private image from scratch then make shared
		makePrivateImage(imageFileName, cols, rows);
		sharedImage = true;
		// Create and log iData for later use
		idata = new ImageData(fileNameHashed, this, wholeSurface);
		imageMap.insert(make_pair(wholeSurface, idata));
		// Create and log fData for later use
		fdata = new FileData(fileNameHashed, this, wholeSurface, frames, nbr_of_img_cols, nbr_of_img_rows);
		fileMap.insert(make_pair(fileNameHashed, fdata));
	}
	else {		// bitmap file has already been loaded
		useSharedImage(false, fdata->cols, fdata->rows, fdata->wholeSurface, fdata->frames);
		// Create and log iData for later use
		idata = new ImageData(fileNameHashed, this, wholeSurface);
		imageMap.insert(make_pair(wholeSurface, idata));
		fdata->refCount++; // increment reference counter
	}
}

void AWSprite::useSharedImage(bool withAlpha, Uint32 cols, Uint32 rows, SDL_Surface* ws, SDL_Surface** f) {
	sharedImage = true;
	wholeSurface = ws;
	frames = f;

	nbr_of_img_rows = rows;	//pkl
	nbr_of_img_cols = cols;	//pkl
	uses_per_pixel_alpha = withAlpha;	//pkl

	// Divide the image into frames
	howManyFrames = cols*rows;
	firstFrame = 0;
	lastFrame = howManyFrames - 1;
	height = wholeSurface->h / rows;
	width = wholeSurface->w / cols;

	set_currentFrame(0);
	initialize_sprite();

	// These are used for bb_80_collision detection
	percentHeight = height / COLLISIONDIVISOR;
	percentWidth = width / COLLISIONDIVISOR;

	clear_animation_stack();
	push_animation_stack();
}

// Initialises alpha values for image based on greycsale levels in mask
void AWSprite::apply_alpha_mask(SDL_Surface* image, SDL_Surface* mask) {
	int i_bpp = image->format->BytesPerPixel;
	if (i_bpp != 4) return;

	int m_bpp = mask->format->BytesPerPixel;

	Uint32 i_width = image->w;
	Uint32 i_height = image->h;
	Uint32 m_width = mask->w;
	Uint32 m_height = mask->h;

	Uint8 red, green, blue, alpha;
	Uint8 mred, mgreen, mblue, malpha;
	Uint32 colour;

	Uint8 *p, *mp;				// pointer to a single pixel
	Uint8 *pData, *mpData;		// pointer to pixel data
	Uint16 pPitch, mpPitch;		// bytes per row

	SDL_LockSurface(image);
	SDL_LockSurface(mask);

	pData = (Uint8*)image->pixels;
	pPitch = image->pitch;
	mpData = (Uint8*)mask->pixels;
	mpPitch = mask->pitch;

	Uint32 mx = 0;
	Uint32 my = 0;

	for (Uint32 px = 0; px < i_width; px++) {
		for (Uint32 py = 0; py < i_height; py++) {
			p = pData + (py * pPitch) + (px * i_bpp);
			colour = *(Uint32 *)p;
			get_RGBA_components(image, colour, red, green, blue, alpha);

			mp = mpData + (my * mpPitch) + (mx * m_bpp);
			colour = *(Uint32 *)mp;
			get_RGBA_components(mask, colour, mred, mgreen, mblue, malpha);

			malpha = (mred + mgreen + mblue) / 3;
			colour = SDL_MapRGBA(image->format, red, green, blue, malpha);
			(*(Uint32 *)p) = colour;
			my = (++my) % m_height;
		}
		mx = (++mx) % m_width;
	}
	SDL_UnlockSurface(mask);
	SDL_UnlockSurface(image);
}

void AWSprite::restoreImageDataFromFileData(SDL_Surface* oldWholeSurface, FileData* fdata) {
	ImageMapIt it = imageMap.lower_bound(oldWholeSurface);
	ImageMapIt end = imageMap.upper_bound(oldWholeSurface);
	ImageData* idata = NULL;

	for (; it != end; ++it)	{
		idata = it->second;
		idata->sprite->frames = fdata->frames;
		// The next statement will cause surface to be set.
		idata->sprite->set_currentFrame(idata->sprite->currentFrame);
		if (idata->sprite->uses_per_pixel_alpha == false)
			idata->sprite->set_surface_alpha((Uint32)idata->sprite->alphaLevel);
	}
}

// Gets RGB values for a colour value depending on surface format
void AWSprite::get_RGBA_components(SDL_Surface* s, Uint32 colour,
	Uint8 &red, Uint8 &green, Uint8 &blue, Uint8 &alpha) {
	blue = (colour & s->format->Bmask) >> s->format->Bshift;
	green = (colour & s->format->Gmask) >> s->format->Gshift;
	red = (colour & s->format->Rmask) >> s->format->Rshift;
	alpha = (colour & s->format->Amask) >> s->format->Ashift;
}

// Clear a specific frame to the colour supplied
void AWSprite::clear_frame(Uint32 f, Uint32 colour) {
	SDL_FillRect(frames[f], NULL, colour);
}

void AWSprite::initialize_sprite() {
	velX = 0.0;
	velY = 0.0;
	accelX = 0.0;
	accelY = 0.0;
	Uint32 ticks = SDL_GetTicks();
	lastMoveTicks = ticks;
	lastAnimationTicks = ticks;
	lastAccelerationTicks = ticks;
	movementInterval = 0;
	animationInterval = 0;
	accelerationInterval = 0;
	repeatAnimationForever = false;
	animationRepeats = 0;
	autoDestructActivated = false;
	dead = false;
	visible = true;
	worldX = 0.0;
	worldY = 0.0;
	screenX = 0.0;
	screenY = 0.0;
	hit_x = 0;
	hit_y = 0;
	moveTicks = 0;
	animateTicks = 0;
	accelerateTicks = 0;
	pulseTicks = 0;
	delayTicks = 0;

	pulseInterval = 0;
	delayInterval = 0;

	minPulseLevel = 255;
	maxPulseLevel = 255;

	delayAction = NULL;
	lastDelayTicks = 0;
	delayRepeats = 0;

	angle = speed = 0;
}

/**
If this is a shared image then decrement reference count. If
reference count is zero or it is a private image then
release all memory reserved for graphic data.
*/
void AWSprite::freeImageMemory() {
	if (!sharedImage){
		SDL_FreeSurface(wholeSurface);
		for (Uint32 f = 0; f < howManyFrames; f++) {
			SDL_FreeSurface(frames[f]);
		}
		delete[] frames;
		wholeSurface = NULL;
		surface = NULL;
		frames = NULL;
		for (Uint32 f = 0; f < howManyFrames; f++)	//****
			SDL_DestroyTexture(frameTextures[f]);	//v1.1
	}
	else {
		ImageData* idata = removeFromImageMap(wholeSurface, this);
		FileData* fdata = findImageByFilename(idata->fnHash);
		fdata->refCount--;
		if (fdata->refCount == 0) {
			freeImageMemory();
			removeFromFileMap(idata->fnHash);
			delete fdata;
		}
		delete idata;
	}
}

// Release all dynamic memory
AWSprite::~AWSprite(void) {
	freeImageMemory();
}

// Create a series of frames from an image
SDL_Surface** AWSprite::create_frames(Uint32 cols, Uint32 rows) {
	SDL_Surface** frames = new SDL_Surface*[cols*rows];

	frameTextures = new SDL_Texture*[cols*rows];	//v5.0

	Uint32 rmask, gmask, bmask, amask;
	SDL_Rect src, dest;

	dest.x = 0;
	dest.y = 0;
	dest.w = width;
	dest.h = height;
	src = dest;	  // src will change, depending on which frame

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

	SDL_Surface* screen = SDL_GetWindowSurface(GameBase::window);	//v5.0
	SDL_Surface *temp;
	for (Uint32 c = 0; c < cols; c++) {
		for (Uint32 r = 0; r < rows; r++) {
			temp = SDL_CreateRGBSurface(screen->flags, width, height,
				32,
				rmask, gmask, bmask, amask);
//			frames[r*cols + c] = SDL_DisplayFormat(temp);
			frames[r*cols + c] = SDL_ConvertSurface(temp, screen->format, NULL);	//v5.0
			src.x = c*width;
			src.y = r*height;
			SDL_BlitSurface(wholeSurface, &src, frames[r*cols + c], &dest);
			frameTextures[r*cols + c] = SDL_CreateTextureFromSurface(GameBase::renderer, frames[r*cols + c]);	//v5.0
			SDL_FreeSurface(temp);
		}
	}
	SDL_FreeSurface(screen);	//v5.0
	return frames;
}

// Create a series of frames from an image for per-pixel alpha
SDL_Surface** AWSprite::create_frames_with_alpha(Uint32 cols, Uint32 rows) {
	SDL_Surface** frames = new SDL_Surface*[cols*rows];
	frameTextures = new SDL_Texture*[cols*rows];	//v5.0

	Uint32 rmask, gmask, bmask, amask;
	SDL_Rect src, dest;

	dest.x = 0;
	dest.y = 0;
	dest.w = width;
	dest.h = height;
	src = dest;	  // src will change, depending on which frame

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

	SDL_Surface *screen = SDL_GetWindowSurface(GameBase::window);	//v5.0
	SDL_PixelFormat fmt = *screen->format;
	fmt.Amask = 0xff000000;	//set alpha
	SDL_FreeSurface(screen);

	SDL_Surface *temp;
	for (Uint32 c = 0; c < cols; c++) {
		for (Uint32 r = 0; r < rows; r++) {
			temp = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, rmask, gmask, bmask, amask);	//v5.0
//			frames[r*cols + c] = SDL_DisplayFormatAlpha(temp);
			frames[r*cols + c] = SDL_ConvertSurface(temp, &fmt, NULL);	//v5.0
			src.x = c*width;
			src.y = r*height;
			SDL_BlitSurface(wholeSurface, &src, frames[r*cols + c], &dest);
			frameTextures[r*cols + c] = SDL_CreateTextureFromSurface(GameBase::renderer, wholeSurface);	//v5.0
			SDL_FreeSurface(temp);
		}
	}
	return frames;
}

SDL_Surface** AWSprite::create_frames_with_alpha() {
	return create_frames_with_alpha(howManyFrames, 1);
}

SDL_Surface** AWSprite::create_frames() {
	return create_frames(howManyFrames, 1);
}

void AWSprite::set_surface_alpha(Uint32 value) {	//v5.0	****REMOVE???
	if (value < 0)
		value = 0;
	else
		value %= 256;
	alphaLevel = (float)value;	// required for pulse
	//for (int f = 0; f < (int)howManyFrames; f++)	//****
	//	SDL_SetAlpha(frames[f], SDL_SRCALPHA | SDL_RLEACCEL, value);
}

int AWSprite::get_surface_alpha() {
	return (int)alphaLevel;
}

void AWSprite::set_world_position(int wX, int wY) {
	worldX = (float)wX;
	worldY = (float)wY;
}

void AWSprite::set_world_position_x(int wX) {
	worldX = (float)wX;
}

void AWSprite::set_world_position_y(int wY) {
	worldY = (float)wY;
}

void AWSprite::set_world_position(float wX, float wY) {
	worldX = wX;
	worldY = wY;
}

void AWSprite::set_world_position_x(float wX) {
	worldX = wX;
}

void AWSprite::set_world_position_y(float wY) {
	worldY = wY;
}

float AWSprite::get_x() {
	return worldX;
}

float AWSprite::get_y() {
	return worldY;
}

int AWSprite::get_height() {
	return height;
}

int AWSprite::get_width() {
	return width;
}

Uint32 AWSprite::get_currentFrame() {
	return currentFrame;
}

void AWSprite::set_currentFrame(Uint32 frameNumber) {
	currentFrame = frameNumber;
	if (currentFrame >= howManyFrames) {
		currentFrame = howManyFrames - 1;
	}
	surface = frames[currentFrame];
}

void AWSprite::set_velocities(float vX, float vY) {
	velX = vX;
	velY = vY;
}

void AWSprite::set_x_velocity(float vX) {
	velX = vX;
}

void AWSprite::set_y_velocity(float vY) {
	velY = vY;
}

float AWSprite::get_x_velocity() {
	return velX;
}

float AWSprite::get_y_velocity() {
	return velY;
}

void AWSprite::set_accelerations(float aX, float aY) {
	accelX = aX;
	accelY = aY;
}

void AWSprite::set_x_acceleration(float aX) {
	accelX = aX;
}

void AWSprite::set_y_acceleration(float aY) {
	accelY = aY;
}

float AWSprite::get_x_acceleration() {
	return accelX;
}

float AWSprite::get_y_acceleration() {
	return accelY;
}

void AWSprite::acceleration_callback() {
	return;
}

void AWSprite::setSpeed(float newSpeed)//added by Stephen phillips
{
	speed = newSpeed;
	velX = cos(angle) * speed;
	velY = sin(angle) * speed;
}

void AWSprite::setAngle(float newAngle)//added by Stephen phillips
{
	angle = newAngle;
	velX = cos(angle) * speed;
	velY = sin(angle) * speed;
}

void AWSprite::accelerate() {
	velX += accelX;
	velY += accelY;
	acceleration_callback();
}

void AWSprite::auto_accelerate() {
	if (accelerationInterval == 0) return;
	if ((updateEverythingTicks - lastAccelerationTicks) > accelerationInterval) {
		lastAccelerationTicks = updateEverythingTicks;
		accelerate();
	}
}

void AWSprite::set_auto_accelerate(Uint32 interval) {
	accelerationInterval = interval;
}

void AWSprite::make_visible() {
	visible = true;
}

void AWSprite::make_invisible() {
	visible = false;
}

void AWSprite::hide() {
	make_invisible();
}

void AWSprite::set_visibility(bool newVisibility) {
	visible = newVisibility;
}

bool AWSprite::toggle_visibility() {
	if (visible) visible = false;
	else visible = true;

	return visible;
}

void AWSprite::draw_onto(SDL_Surface *onto) {
	SDL_Rect src, dest;

	if (!visible) return;
	src.x = 0;
	src.y = 0;
	src.w = width;
	src.h = height;
	dest.x = (Uint32)screenX;
	dest.y = (Uint32)screenY;

	//	SDL_Texture *texture = SDL_CreateTextureFromSurface(GameBase::renderer, frames[currentFrame]);	//**** SLOWS FPS!!!
	dest.w = src.w; dest.h = src.h;	//v5.0
	SDL_SetTextureBlendMode(frameTextures[currentFrame], SDL_BLENDMODE_BLEND);	//****
	SDL_RenderCopy(GameBase::renderer, frameTextures[currentFrame], &src, &dest);
}

//***********************************************************	//v5.0
void AWSprite::rotateAndDraw(float ang, float rx, float ry){
	if (!visible) return;
	updateEverythingTicks = SDL_GetTicks();
	auto_move();
	auto_animate();
	auto_accelerate();
	auto_pulse();	// added by pkl
	auto_delay();	// added by pkl
	map_world_to_screen(0,0);

	SDL_Rect src, dest;
	src.x = src.y = 0;
	src.w = width;
	src.h = height;
	dest.x = (Uint32)screenX;
	dest.y = (Uint32)screenY;

	dest.w = src.w; dest.h = src.h;	//v5.0
	SDL_Point cp = { src.x + src.w / 2, src.y + src.h / 2 };
	m_angle = ang;
	SDL_SetTextureBlendMode(frameTextures[currentFrame], SDL_BLENDMODE_BLEND);	//****
	SDL_RenderCopyEx(GameBase::renderer, frameTextures[currentFrame], &src, &dest, ang, &cp, SDL_FLIP_NONE);
}
//***********************************************************


void AWSprite::draw() {
	this->draw_onto(NULL);	//v5.0
}

void AWSprite::set_transparent_colour(int R, int G, int B) {
	SDL_Surface* onto = SDL_GetWindowSurface(GameBase::window);	//v5.0
	transparentColour = SDL_MapRGB(onto->format, R, G, B);
	SDL_SetColorKey(onto, SDL_TRUE, SDL_MapRGB(onto->format, R, G, B));	//v5.0
	for (Uint32 f = 0; f < howManyFrames; f++) {
		SDL_SetColorKey(frames[f], SDL_TRUE, SDL_MapRGB(frames[f]->format, R, G, B));	//v5.0
		SDL_DestroyTexture(frameTextures[f]);	//v5.0
		frameTextures[f] = SDL_CreateTextureFromSurface(GameBase::renderer, frames[f]);	//v5.0
	}
}

// map (worldX, worldY) to (screenX, screenY)
//  The function needs to know where in the world the top-left
//  of the screen currently is. Once we have mapped, we can just
//  use draw().
void AWSprite::map_world_to_screen(int topleftX, int topleftY) {
	screenX = worldX - topleftX;
	screenY = worldY - topleftY;
}

// This version assumes that the world is the same size as the screen
void AWSprite::map_world_to_screen() {
	screenX = worldX;
	screenY = worldY;
}

void AWSprite::movement_callback() {
	return;
}

void AWSprite::move() {
	worldX += velX;
	worldY += velY;
	movement_callback();
	map_world_to_screen();
}

void AWSprite::auto_move() {
	if (movementInterval == 0) return;

	if ((updateEverythingTicks - lastMoveTicks) > movementInterval) {
		lastMoveTicks = updateEverythingTicks;
		move();
	}
}

void AWSprite::set_auto_move(Uint32 interval) {
	movementInterval = interval;
}

void AWSprite::animation_callback() {
	return;
}

void AWSprite::animate() {
	//set_currentFrame(get_currentFrame()+1);
	currentFrame++;
	if (repeatAnimationForever) {
		// In ths case, we are just animating over and over
		// and we don't care about how many times we repeat
		if (get_currentFrame() > lastFrame) {
			set_currentFrame(firstFrame);
		}
	}
	else {
		if (get_currentFrame() > lastFrame) {
			animationRepeats = animationRepeats - 1;
			if (animationRepeats == 0) {
				if (!autoDestructActivated) {
					pop_animation_stack();
				}
				else {
					this->make_invisible();
					this->set_auto_accelerate(0);
					this->set_auto_move(0);
					autoDestructActivated = false;
					clear_animation_stack();
					this->set_auto_animate(0);
					push_animation_stack();
				}
			}
			else {
				set_currentFrame(firstFrame);
			}
		}
	}
	animation_callback();
	set_currentFrame(currentFrame);
}

/* Callback section for the auto animation feature.
   It is very complex. You don't need to understand it.
   It is this complex because I wanted to keep currentFrame,
   lastFrame, firstFrame and howmanyFrames private
   */
void AWSprite::auto_animate() {
	if (animationInterval == 0) return;

	if ((updateEverythingTicks - lastAnimationTicks) > animationInterval) {
		lastAnimationTicks = updateEverythingTicks;
		animate();
	}
}

// This allows us to set up an animation to repeat a limited
// number of times. If you set the second parameter to zero,
// the animation will repeat FOREVER. If you want to *stop*
// animation, set the FIRST parameter to zero
void AWSprite::set_auto_animate(Uint32 interval, Uint32 repeats) {
	if (repeats == 0) repeatAnimationForever = true;
	else repeatAnimationForever = false;
	animationInterval = interval;
	animationRepeats = repeats;
	// Remember what we were doing before
	push_animation_stack();
}

// Calling this function means we want to repeat forever...
void AWSprite::set_auto_animate(Uint32 interval) {
	set_auto_animate(interval, 0);
}

// This allows you to use a subset of the frames available for an animation
void AWSprite::set_animation_subset(Uint32 first, Uint32 last) {
	if (first < 0) first = 0;
	if (first >= howManyFrames) first = howManyFrames - 1;
	if (last < first) last = first;
	if (last >= howManyFrames) last = howManyFrames - 1;

	firstFrame = first;
	lastFrame = last;
	set_currentFrame(firstFrame);
}

// Set up the destruction animation of the sprite
// ******** This cannot be countermanded!! ***********
// Once the "countdown" starts, the sprite WILL die...
void AWSprite::set_auto_destruct(Uint32 first, Uint32 last, Uint32 interval, Uint32 repeats) {
	// No need to do it twice!!
	if (autoDestructActivated) return;
	this->make_visible();
	// It makes no sense to have an infinite destruction animation!
	if (repeats < 1) repeats = 1;
	// Once the auto-destruct starts, the stack is irrelevant
	clear_animation_stack();
	set_animation_subset(first, last);
	set_auto_animate(interval, repeats);
	autoDestructActivated = true;
}

void AWSprite::set_auto_destruct(Uint32 first, Uint32 last, Uint32 interval) {
	set_auto_destruct(first, last, interval, 1);
}

// This function moves, animates, maps and draws!!
void AWSprite::update_everything(int topLeftX, int topLeftY) {
	updateEverythingTicks = SDL_GetTicks();
	auto_move();
	auto_animate();
	auto_accelerate();
	auto_pulse();	// added by pkl
	auto_delay();	// added by pkl
	map_world_to_screen(topLeftX, topLeftY);
	draw();
}

// This function moves, animates, maps and draws!!
void AWSprite::update_everything() {
	update_everything(0, 0);
}
//************************************************************	//v5.0
void rotatedPt(float ang, float cx, float cy, float x, float y, float &nx, float &ny){
	float angInRads = ang * (float)M_PI / 180.0f;
	x -= cx; y -= cy;
	nx = cx + x*cos(angInRads) + y*sin(angInRads);
	ny = cy - x*sin(angInRads) + y*cos(angInRads);
//	cout << "\n\tnx = " << nx << " ny= " << ny;
}

void getBBPts(int x1, int y1, int &lx, int &rx, int &ty, int &by){
	if (x1 < lx) lx = x1;
	if (y1 < ty) ty = y1;
	if (x1 > rx) rx = x1;
	if (y1 > by) by = y1;
}

void AWSprite::findRotatedTopBottom(float ang, float cx, float cy, int x1, int y1, int x2, int y2, int &lx, int &rx, int &ty, int &by){
//	float cx = x1 + width / 2.0f, cy = y1 + height / 2.0f, nx, ny;
	cx += x1; cy += y1;
	float nx, ny;
	rotatedPt(ang, cx, cy, (float)x1, (float)y1, nx, ny);
	lx = (int)nx, rx = (int)nx;	ty = (int)ny, by = (int)ny;
	getBBPts((int)nx, (int)ny, lx, rx, ty, by);
	rotatedPt(ang, cx, cy, (float)x2, (float)y1, nx, ny);
	getBBPts((int)nx, (int)ny, lx, rx, ty, by);
	rotatedPt(ang, cx, cy, (float)x2, (float)y2, nx, ny);
	getBBPts((int)nx, (int)ny, lx, rx, ty, by);
	rotatedPt(ang, cx, cy, (float)x1, (float)y2, nx, ny);
	getBBPts((int)nx, (int)ny, lx, rx, ty, by);
//	cout << "\nlx = " << lx << " rx= " << rx << " ty= " << ty << " by= " << by;
}
//************************************************************

// On detecting a collision bb_collision_callback() is called for BOTH
// sprites.
// The callback function can be over-ridden in any class that inherits
// from AWSprite to perform class specific actions on collisions
bool AWSprite::bb_collision(AWSprite *spriteB) {
	Sint32 topA, botA, leftA, rightA;
	Sint32 topB, botB, leftB, rightB;

	if (false == is_visible()) return false;
	if (false == spriteB->is_visible()) return false;

	topA = (Sint32)worldY;
	botA = (Sint32)worldY + height;
	leftA = (Sint32)worldX;
	rightA = (Sint32)worldX + width;
	topB = (Sint32)spriteB->get_y();
	leftB = (Sint32)spriteB->get_x();
	rightB = (Sint32)spriteB->get_x() + spriteB->get_width();
	botB = (Sint32)spriteB->get_y() + spriteB->get_height();


	if (m_angle != 0.0f){	//v5.0
//		cout << "\ninit: la = " << leftA << " ta= " << topA << " ra= " << rightA << " ba= " << botA;
		findRotatedTopBottom(-m_angle, width/2.0f, height/2.0f, leftA, topA, rightA, botA, leftA, rightA, topA, botA);
	}
	if (spriteB->m_angle != 0.0f)	//v5.0
		findRotatedTopBottom(-spriteB->m_angle, spriteB->width / 2.0f, spriteB->height / 2.0f, leftB, topB, rightB, botB, leftB, rightB, topB, botB);

	if (botA < topB) return false;
	if (botB < topA) return false;
	if (rightA < leftB) return false;
	if (rightB < leftA) return false;

	// call collision handler event - added pkl
	bb_collision_callback();
	spriteB->bb_collision_callback();

	return true;
}

// On detecting a collision bb_collision_callback() is called for BOTH
// sprites.
// The callback function can be over-ridden in any class that inherits
// from AWSprite to perform class specific actions on collisions
bool AWSprite::bb_80_collision(AWSprite *spriteB) {
	Sint32 topA, botA, leftA, rightA;
	Sint32 topB, botB, leftB, rightB;

	if (false == is_visible()) return false;
	if (false == spriteB->is_visible()) return false;

	topA = (Sint32)worldY + percentHeight;
	botA = (Sint32)worldY + height - percentHeight;
	leftA = (Sint32)worldX + percentWidth;
	rightA = (Sint32)worldX + width - percentWidth;
	topB = (Sint32)spriteB->get_y() + spriteB->percentHeight;
	leftB = (Sint32)spriteB->get_x() + spriteB->percentWidth;
	rightB = (Sint32)spriteB->get_x() + spriteB->get_width() - spriteB->percentWidth;
	botB = (Sint32)spriteB->get_y() + spriteB->get_height() - spriteB->percentHeight;

	if (botA < topB) return false;
	if (botB < topA) return false;
	if (rightA < leftB) return false;
	if (rightB < leftA) return false;

	// call collision handler event - added pkl
	bb_collision_callback();
	spriteB->bb_collision_callback();

	return true;
}

bool AWSprite::pp_slow_collision(AWSprite *spriteB) {
	Sint32 topA, botA, leftA, rightA;
	Sint32 topB, botB, leftB, rightB;
	Sint32 topO, botO, leftO, rightO;
	Uint32 ax, ay, bx, by;
	Uint32 APx, APy, ASx, ASy;
	Uint32 BPx, BPy, BSx, BSy;
	Uint32 aColour, bColour;
	Uint32 bTC;

	if (false == is_visible()) return false;
	if (false == spriteB->is_visible()) return false;

	topA = (Sint32)worldY;
	botA = (Sint32)worldY + height;
	leftA = (Sint32)worldX;
	rightA = (Sint32)worldX + width;
	topB = (Sint32)spriteB->get_y();
	leftB = (Sint32)spriteB->get_x();
	rightB = (Sint32)spriteB->get_x() + spriteB->get_width();
	botB = (Sint32)spriteB->get_y() + spriteB->get_height();

	if (botA < topB) return false;
	if (botB < topA) return false;
	if (rightA < leftB) return false;
	if (rightB < leftA) return false;

	// If we get here, we know that there is an overlap
	// So we work out where the sides of the ovelap are
	if (leftA < leftB) leftO = leftB;
	else leftO = leftA;
	if (rightA > rightB) rightO = rightB;
	else rightO = rightA;
	if (botA > botB) botO = botB;
	else botO = botA;
	if (topA < topB) topO = topB;
	else topO = topA;

	// P is the top-left, S is the bottom-right of the overlap
	APx = leftO - leftA;   APy = topO - topA;
	ASx = rightO - leftA; ASy = botO - topA;
	BPx = leftO - leftB;   BPy = topO - topB;
	BSx = rightO - leftB; BSy = botO - topB;
	bTC = spriteB->transparentColour;
	by = BPy;
	for (ay = APy; ay < ASy; ay++) {
		bx = BPx;
		for (ax = APx; ax < ASx; ax++) {
			aColour = get_pixel_32(ax, ay);
			bColour = spriteB->get_pixel_32(bx, by);
			if (aColour != transparentColour && bColour != bTC)
			{ //pkl
				hit_x = ax;
				hit_y = ay;
				spriteB->set_hit_xy(bx, by);
				// call collision handler event - added pkl
				pp_collision_callback();
				spriteB->pp_collision_callback();
				return true;
			}
			bx++;
		}
		by++;
	};
	return false;
}

/*
 * Return the pixel value at (x, y)
 * Notes:
 *  1. This has been translated into AWSprite terms.
 *  2. This function only handles 32-bit colour.
 *  3. The surface must be locked before the pixels are read.
 */
Uint32 AWSprite::get_pixel_32(Uint32 x, Uint32 y) {
	/* bpp will be 1 if it's 8-bit colour, 2 if it's 16-bit colour,
	 *  3 if it's 24-bit colour and 4 if it's 32-bit colour. In
	 *  this case, we have said that we are only dealing with 32-bit
	 *  colour, so if it's not 4 we return the transparent colour,
	 *  (because we're saying we can't interpret the colour).
	 */
	int bpp = surface->format->BytesPerPixel;
	if (bpp != 4) return transparentColour;
	SDL_LockSurface(surface);

	/* pixels is the data structure where the pixels are stored
	 *  it is effectively an array of numbers, where each number
	 *  represents a pixel. The size of the number (8-bit, 16-bit
	 *  24-bit, 32-bit) determines the colour depth of the pixel.
	 * pitch is the number of bytes per scanline of the screen; if
	 *  our screen is 640 pixels wide, and we have 32-bit colour,
	 *  pitch will be 640*4 = 2560.
	 * p is a pointer which will point at the pixel at position
	 *  (x,y) on the surface
	 */
	Uint8 *p;
	p = (Uint8 *)surface->pixels + (y * surface->pitch) + (x * bpp);

	/* This says: Despite the fact that we declared it as a pointer
	 *  to an 8-bit number, p is actually pointing at a 32-bit number
	 *  and we want to return THE NUMBER THAT p IS POINTING AT
	 */
	Uint32 colour = *(Uint32 *)p;
	SDL_UnlockSurface(surface);
	return colour;
}

// By default we check for 32 bit colour
bool AWSprite::pp_collision(AWSprite *spriteB) {
	return pp_collision_32(spriteB);
}
//**************************************************************	//v5.0
bool AWSprite::isPtInImage(float ang, float cx, float cy, float w, float h, float x, float y, Uint32 &origX, Uint32 &origY){
	origX = origY = 0;
	float angInRads = ang * (float)M_PI / 180.0f;
	x -= cx; y -= cy;
	float a = x*cos(angInRads) - y*sin(angInRads);
	float b = x*sin(angInRads) + y*cos(angInRads);
	origX = (Uint32)(a + w / 2);
	origY = (Uint32)(b + h / 2);
//	cout << "\nox: " << origX << " oy: " << origY;
	if (a + w / 2 < 0 || a + w / 2 > w || b + h / 2 < 0 || b + h / 2 > h)
		return false;
	return true;
}
//**************************************************************
// This pixel collision detection works with non-alpha, per-surface and
// per-pixel alpha blended spites. For BOTH sprites the location of the
// first none-transparent pixel collision detected is recorded in hit_x
// and hit_y and the pp_collision_callback() method is called. The callback
// function can be over-ridden in any class that inherits from AWSprite to
// perform class specific actions on collisions
bool AWSprite::pp_collision_32(AWSprite *spriteB) {
	Sint32 topA, botA, leftA, rightA;
	Sint32 topB, botB, leftB, rightB;
	Sint32 topO, botO, leftO, rightO;
	Uint32 ax, ay;
	Uint32 bx, by;		//pkl
	Uint32 APx, APy, ASx, ASy;
	Uint32 BPx, BPy, BSx, BSy;
	Uint32 aColour, bColour;
	Uint32 bTC;

	if (false == is_visible()) return false;
	if (false == spriteB->is_visible()) return false;

	topA = (Sint32)worldY;
	botA = (Sint32)worldY + height;
	leftA = (Sint32)worldX;
	rightA = (Sint32)worldX + width;
	topB = (Sint32)spriteB->get_y();
	leftB = (Sint32)spriteB->get_x();
	rightB = (Sint32)spriteB->get_x() + spriteB->get_width();
	botB = (Sint32)spriteB->get_y() + spriteB->get_height();

	if (m_angle != 0.0f){	//v5.0
//		cout << "\ninit: la = " << leftA << " ta= " << topA << " ra= " << rightA << " ba= " << botA;
		findRotatedTopBottom(m_angle, width/2.0f, height/2.0f, leftA, topA, rightA, botA, leftA, rightA, topA, botA);
	}
	if (spriteB->m_angle != 0.0f)	//v5.0
		findRotatedTopBottom(spriteB->m_angle, spriteB->width / 2.0f, spriteB->height / 2.0f, leftB, topB, rightB, botB, leftB, rightB, topB, botB);

	if (botA < topB) return false;
	if (botB < topA) return false;
	if (rightA < leftB) return false;
	if (rightB < leftA) return false;

	float acx = (rightA - leftA) / 2.0f, acy = (botA - topA) / 2.0f;	//v5.0 get ctr of bounding boxes
	float bcx = (rightB - leftB) / 2.0f, bcy = (botB - topB) / 2.0f;
	
	// If we get here, we know that there is an overlap
	// So we work out where the sides of the ovelap are
	if (leftA < leftB) leftO = leftB;
	else leftO = leftA;
	if (rightA > rightB) rightO = rightB;
	else rightO = rightA;
	if (botA > botB) botO = botB;
	else botO = botA;
	if (topA < topB) topO = topB;
	else topO = topA;
	Uint32 widthO = rightO - leftO;

	// P is the top-left, S is the bottom-right of the overlap
	APx = leftO - leftA;   APy = topO - topA;
	ASx = rightO - leftA; ASy = botO - topA;
	BPx = leftO - leftB;   BPy = topO - topB;
	BSx = rightO - leftB; BSy = botO - topB;
	bTC = spriteB->transparentColour;

	bool foundCollision = false;

	SDL_LockSurface(surface);
	SDL_LockSurface(spriteB->surface);

	// These two are pointers into the pixel areas for
	//  spriteA and spriteB, respectively. We set them
	//  up at first then move them onwards by incrementing
	//  this version of the function only works with 32-bit
	//  colour depth, although adapting it would not be hard
	Uint8 *pA, *pB;
	// We only need to do this once, as loading a sprite in AWSprite
	//  forces it to have the same colour depth as the screen surface.
	//  Whatever this colour depth is, it will be the same for both
	//  spriteA and spriteB
	int bpp = surface->format->BytesPerPixel;
	Uint32 widthObytes = widthO*bpp;

	// These are widths in BYTES. They are used inside the loop
	//  to avoid the need to do the slow multiplications
	Uint32 surfaceWidthA = surface->pitch;
	Uint32 surfaceWidthB = spriteB->surface->pitch;

	bool pixelAtransparent = false;
	bool pixelBtransparent = false;

	bx = BPx; by = BPy;	//pkl
	Uint32 orgX, orgY;	//v5.0

	for (ay = APy; ay < ASy; ay++) {
		bx = BPx;
		for (ax = APx; ax < ASx; ax++) {
			orgX = orgY = 0;	//v5.0
			if (isPtInImage(-m_angle, acx, acy, (float)width, (float)height, (float)ax, (float)ay, orgX, orgY)){	//v5.0
//	cout << "\nox: " << orgX << " oy: " << orgY;
				pA = (Uint8 *)surface->pixels + (orgY * surfaceWidthA) + (orgX * bpp);
				if (pA == NULL){	//v5.0
					cout << "\n\nError reading pixel A from sprite";
					return false;
				}
				aColour = *(Uint32 *)pA;
				pixelAtransparent = (!uses_per_pixel_alpha && (aColour == transparentColour))
					|| (uses_per_pixel_alpha && (aColour & surface->format->Amask) == 0);
			}
			else
				pixelAtransparent = true;

			if (isPtInImage(-spriteB->m_angle, bcx, bcy, (float)spriteB->width, (float)spriteB->height, (float)bx, (float)by, orgX, orgY)){	//v5.0
				pB = (Uint8 *)spriteB->surface->pixels + (orgY * surfaceWidthB) + (orgX * bpp);
				if (pB == NULL){	//v5.0
					cout << "\n\nError reading pixel B from sprite";
					return false;
				}
				bColour = *(Uint32 *)pB;
				pixelBtransparent = (!spriteB->uses_per_pixel_alpha && (bColour == bTC))
					|| (spriteB->uses_per_pixel_alpha && (bColour & spriteB->surface->format->Amask) == 0);
			}
			else
				pixelBtransparent = true;

			if (!pixelAtransparent && !pixelBtransparent) {
				hit_x = ax;
				hit_y = ay;
				spriteB->set_hit_xy(bx, by);
				foundCollision = true;
				// call collision handler event - added pkl
				pp_collision_callback();
				spriteB->pp_collision_callback();
				break;
			}
			//pA += bpp;	//v5.0
			//pB += bpp;
			bx++; //pkl
		}
		if (foundCollision) break;
		//pA = pA + (surfaceWidthA - widthObytes);	//v5.0
		//pB = pB + (surfaceWidthB - widthObytes);
		by++;	//pkl
	}
	SDL_UnlockSurface(surface);
	SDL_UnlockSurface(spriteB->surface);

	return foundCollision;
}

// Same comments as pp_collision_32 but currently this method will not
// detect collisions with per-pixel alpha blended sprites. See further
// comments in this function regarding this limitation.
bool AWSprite::pp_collision_16(AWSprite *spriteB) {
	Uint32 topA, botA, leftA, rightA;
	Uint32 topB, botB, leftB, rightB;
	Uint32 topO, botO, leftO, rightO;
	Uint32 ax, ay;
	Uint32 bx, by;		//pkl
	Uint32 APx, APy, ASx, ASy;
	Uint32 BPx, BPy, BSx, BSy;
	Uint32 aColour, bColour;
	Uint32 bTC;

	if (false == is_visible()) return false;
	if (false == spriteB->is_visible()) return false;

	topA = (Uint32)worldY;
	botA = (Uint32)worldY + height;
	leftA = (Uint32)worldX;
	rightA = (Uint32)worldX + width;
	topB = (Uint32)spriteB->get_y();
	leftB = (Uint32)spriteB->get_x();
	rightB = (Uint32)spriteB->get_x() + spriteB->get_width();
	botB = (Uint32)spriteB->get_y() + spriteB->get_height();

	if (botA < topB) return false;
	if (botB < topA) return false;
	if (rightA < leftB) return false;
	if (rightB < leftA) return false;

	// If we get here, we know that there is an overlap
	// So we work out where the sides of the ovelap are
	if (leftA < leftB) leftO = leftB;
	else leftO = leftA;
	if (rightA > rightB) rightO = rightB;
	else rightO = rightA;
	if (botA > botB) botO = botB;
	else botO = botA;
	if (topA < topB) topO = topB;
	else topO = topA;
	Uint32 widthO = rightO - leftO;

	// P is the top-left, S is the bottom-right of the overlap
	APx = leftO - leftA;   APy = topO - topA;
	ASx = rightO - leftA; ASy = botO - topA;
	BPx = leftO - leftB;   BPy = topO - topB;
	BSx = rightO - leftB; BSy = botO - topB;
	bTC = spriteB->transparentColour;

	bool foundCollision = false;

	SDL_LockSurface(surface);
	SDL_LockSurface(spriteB->surface);

	// These two are pointers into the pixel areas for
	//  spriteA and spriteB, respectively. We set them
	//  up at first then move them onwards by incrementing
	//  this version of the function only works with 16-bit
	//  colour depth, although adapting it would not be hard
	Uint8 *pA, *pB;
	// We only need to do this once, as loading a sprite in AWSprite
	//  forces it to have the same colour depth as the screen surface.
	//  Whatever this colour depth is, it will be the same for both
	//  spriteA and spriteB
	int bpp = surface->format->BytesPerPixel;
	Uint32 widthObytes = widthO*bpp;

	// These are widths in BYTES. They are used inside the loop
	//  to avoid the need to do the slow multiplications
	Uint32 surfaceWidthA = surface->pitch;
	Uint32 surfaceWidthB = spriteB->surface->pitch;

	/* These 2 lines are only required for sprites using per-pixel alpha transparency
																		pkl
																		bool pixelAtransparent = false;
																		bool pixelBtransparent = false;
																		*/

	pA = (Uint8 *)surface->pixels + (APy * surfaceWidthA) + (APx * bpp);
	pB = (Uint8 *)spriteB->surface->pixels + (BPy * surfaceWidthB) + (BPx * bpp);
	bx = BPx; by = BPy;	//pkl

	for (ay = APy; ay < ASy; ay++) {
		for (ax = APx; ax < ASx; ax++) {
			aColour = *(Uint16 *)pA;
			bColour = *(Uint16 *)pB;
			/* These lines of code should replace the following if statement if we detect
			collisions with sprite using per-pixel transparency. There is no logical
			reason why the code should not work but I have been unable to test it.
			pkl

			pixelAtransparent = (!uses_per_pixel_alpha && (aColour == transparentColour))
			|| (uses_per_pixel_alpha && (aColour & surface->format->Amask) == 0);
			pixelBtransparent = (!spriteB->uses_per_pixel_alpha && (bColour == bTC))
			|| (spriteB->uses_per_pixel_alpha && (bColour & spriteB->surface->format->Amask) == 0);

			if(!pixelAtransparent && !pixelBtransparent) {
			*/
			if (aColour != transparentColour && bColour != bTC) {
				hit_x = ax;
				hit_y = ay;
				spriteB->set_hit_xy(bx, by);
				foundCollision = true;
				// call collision handler event - added pkl
				pp_collision_callback();
				spriteB->pp_collision_callback();
				break;
			}
			pA += bpp;
			pB += bpp;
			bx++;
		}
		if (foundCollision) break;
		pA = pA + (surfaceWidthA - widthObytes);
		pB = pB + (surfaceWidthB - widthObytes);
		by++;	//pkl
	};
	SDL_UnlockSurface(surface);
	SDL_UnlockSurface(spriteB->surface);

	return foundCollision;
}

// On detecting a collision radial_collision_callback()is called for BOTH
// sprites.
// The callback function can be over-ridden in any class that inherits
// from AWSprite to perform class specific actions on collisions

bool AWSprite::radial_collision(AWSprite *spriteB)
{
	if (!is_on_screen() || !spriteB->is_on_screen())
		return false;

	float rVelx = spriteB->velX - velX;
	float rVely = spriteB->velY - velY;
	float compX = worldX - spriteB->get_x();
	float compY = worldY - spriteB->get_y();

	float dotProduct = (compX * rVelx) + (compY * rVely);
	//Tests if balls are heading torwards each other.
	if (dotProduct < 0)
		return false;

	float radiusA = (width + height) / 4.0f;
	float radiusB = (spriteB->get_width() + spriteB->get_height()) / 4.0f;
	float sumOfRadii = radiusA + radiusB;

	if ((compX*compX + compY*compY) < (sumOfRadii*sumOfRadii))
	{
		radial_collision_callback(spriteB);
		spriteB->radial_collision_callback(this);
		return true;
	}
	return false;
}

void AWSprite::pop_animation_stack() {
	// We CANNOT countermand an auto-destruct!
	if (autoDestructActivated) return;

	// Trying to pop from an empty stack is a serious error,
	//  so we do nothing and just return
	stackTop--;
	if (stackTop < 0) return;
	set_currentFrame(stack[stackTop].currentFrame);
	firstFrame = stack[stackTop].firstFrame;
	lastFrame = stack[stackTop].lastFrame;
	repeatAnimationForever = stack[stackTop].repeatAnimationForever;
	animationRepeats = stack[stackTop].animationRepeats;
	animationInterval = stack[stackTop].animationInterval;
}

void AWSprite::push_animation_stack() {
	// We CANNOT countermand an auto-destruct!
	if (autoDestructActivated) return;

	stackTop++;
	if (stackTop == ANIMATIONSTACKDEPTH) stackTop--;
	if (repeatAnimationForever) stackTop = 0;
	stack[stackTop].currentFrame = get_currentFrame();
	stack[stackTop].firstFrame = firstFrame;
	stack[stackTop].lastFrame = lastFrame;
	stack[stackTop].repeatAnimationForever = repeatAnimationForever;
	stack[stackTop].animationRepeats = animationRepeats;
	stack[stackTop].animationInterval = animationInterval;
}

void AWSprite::clear_animation_stack() {
	stackTop = -1;
}

bool AWSprite::is_visible() {
	return visible;
}

bool AWSprite::is_on_screen(Uint32 topLeftX, Uint32 topLeftY) {
	SDL_Surface* videoSurface = SDL_GetWindowSurface(GameBase::window);	//v5.0
	map_world_to_screen(topLeftX, topLeftY);
	if (visible &&
		screenX >= 0 && screenX < videoSurface->w &&
		screenY >= 0 && screenY < videoSurface->h)
	{
		return true;
	}
	else {
		return false;
	}
}

bool AWSprite::is_on_screen() {
	return is_on_screen(0, 0);
}

// ##################################################################
// The folowing code has been added pkl
// ##################################################################

void AWSprite::set_hit_xy(Uint32 x, Uint32 y) {
	hit_x = x;
	hit_y = y;
}

Uint32 AWSprite::get_move_interval() {
	return movementInterval;
}

Uint32 AWSprite::get_animate_interval() {
	return animationInterval;
}

Uint32 AWSprite::get_accelerate_interval() {
	return accelerationInterval;
}

Uint32 AWSprite::get_pulse_interval() {
	return pulseInterval;
}

Uint32 AWSprite::get_delay_interval() {
	return delayInterval;
}

// This function moves, animates, maps and draws!!
void AWSprite::update_everything(Uint32 delta, int topLeftX, int topLeftY) {
	auto_move(delta);
	auto_animate(delta);
	auto_accelerate(delta);
	auto_pulse(delta);
	auto_delay(delta);
	map_world_to_screen(topLeftX, topLeftY);
	draw();
}

// This function moves, animates, maps and draws!!
void AWSprite::update_everything(Uint32 delta) {
	update_everything(delta, 0, 0);
}

void AWSprite::pulse() {	//v5.0	****REMOVE???
	alphaLevel += pulseDir * alphaChange;
	if (alphaLevel <= minPulseLevel) {
		pulseDir = -pulseDir;
		alphaLevel = 2 * minPulseLevel - alphaLevel;
		pulseRepeatCounter--;
	}
	else if (alphaLevel >= maxPulseLevel) {
		pulseDir = -pulseDir;
		alphaLevel = 2 * maxPulseLevel - alphaLevel;
		pulseRepeatCounter--;
	}
	if (pulseRepeatCounter == 0) {
		pulseInterval = 0;	// effectively halts pulsing
		// Set the alphalevel to the nearest rnage limit
		if (alphaLevel - minPulseLevel < maxPulseLevel - alphaLevel)
			alphaLevel = (float)minPulseLevel;
		else
			alphaLevel = (float)maxPulseLevel;

		pulse_end_callback();
	}
	//for (int f = 0; f < (int)howManyFrames; f++)	//v5.0
	//	SDL_SetAlpha(frames[f], SDL_SRCALPHA | SDL_RLEACCEL, (int)alphaLevel);
}

void AWSprite::auto_pulse() {
	if (pulseInterval == 0) return;

	Uint32 ticks = SDL_GetTicks();

	if ((ticks - lastPulseTicks) > pulseInterval) {
		lastPulseTicks = ticks;
		pulse();
	}
}

void AWSprite::auto_pulse(Uint32 delta) {
	if (pulseInterval == 0) return;
	pulseTicks += delta;
	while (pulseTicks >= pulseInterval && pulseInterval != 0) {
		pulseTicks -= pulseInterval;
		pulse();
	}
}

void AWSprite::set_auto_pulse(Uint32 interval, int repeats) {
	if (repeats <= 0) {
		pulseRepeats = 0;
		pulseRepeatCounter = 0;
	}
	else {
		pulseRepeats = repeats;
		pulseRepeatCounter = repeats;
	}
	pulseInterval = interval;
	if (pulseTime != 0)
		alphaChange = (float)(pulseInterval * (maxPulseLevel - minPulseLevel) / pulseTime);
}

void AWSprite::set_pulse_time(Uint32 ticks) {	// pulse time in milliseconds
	// Prevent ridiculously small pulse times
	if (ticks < 10) ticks = 10;
	pulseTime = ticks;
	// Need to modify the alphaChange if the interval is set
	if (pulseInterval != 0)
		alphaChange = (float)(pulseInterval * (maxPulseLevel - minPulseLevel) / pulseTime);
}

void AWSprite::set_pulse_range(int startAlpha, int endAlpha) {
	if (startAlpha < 0) startAlpha = 0;
	if (endAlpha < 0) endAlpha = 0;
	if (startAlpha > 255) startAlpha = 255;
	if (endAlpha > 255) endAlpha = 255;

	if (startAlpha < endAlpha) {
		minPulseLevel = startAlpha;
		maxPulseLevel = endAlpha;
		alphaLevel = (float)startAlpha;
		pulseDir = 1;
	}
	else {
		minPulseLevel = endAlpha;
		maxPulseLevel = startAlpha;
		alphaLevel = (float)startAlpha;
		pulseDir = -1;
	}
	set_surface_alpha((int)alphaLevel);

	if (pulseInterval != 0)
		alphaChange = (float)(pulseInterval * (maxPulseLevel - minPulseLevel) / (pulseTime * 1000));
}

// Delayed action methods
void AWSprite::delay() {
	if (repeatDelayForever) {
		if (delayAction != NULL)
			(this->*delayAction)();
	}
	else {
		if (delayRepeatCounter > 0) {
			if (delayAction != NULL) {
				(this->*delayAction)();
				delayRepeatCounter--;
			}
		}
		else
			delayInterval = 0;
	}
}

void AWSprite::auto_delay(void) {
	if (delayInterval == 0) return;

	Uint32 ticks = SDL_GetTicks();

	if ((ticks - lastDelayTicks) > delayInterval) {
		lastDelayTicks = ticks;
		delay();
	}
}

void AWSprite::auto_delay(Uint32 delta) {
	if (delayInterval == 0) return;
	delayTicks += delta;
	while (delayTicks >= delayInterval && delayInterval != 0) {
		delayTicks -= delayInterval;
		delay();
	}
}

void AWSprite::auto_move(Uint32 delta) {
	if (movementInterval == 0) return;
	moveTicks += delta;
	while (moveTicks >= movementInterval) {
		moveTicks -= movementInterval;
		move();
	}
}

void AWSprite::auto_animate(Uint32 delta) {
	if (animationInterval == 0) return;
	animateTicks += delta;
	while (animateTicks > animationInterval) {
		animateTicks -= animationInterval;
		animate();
	}
}

void AWSprite::auto_accelerate(Uint32 delta) {
	if (accelerationInterval == 0) return;
	accelerateTicks += delta;
	while (accelerateTicks > accelerationInterval) {
		accelerateTicks -= accelerationInterval;
		accelerate();
	}
}

void AWSprite::set_delay_event(callback action, Uint32 dticks, Sint32 times) {
	lastDelayTicks = SDL_GetTicks();
	delayTicks = delayInterval = 0;
	if (times < 0) {
		delayRepeats = 0;
		delayRepeatCounter = 0;
		repeatDelayForever = true;
	}
	else if (times > 0) {
		delayRepeats = times;
		delayRepeatCounter = times;	// if goes negative makes it infinite
		repeatDelayForever = false;
	}
	delayAction = action;
	if (action != NULL)
		delayInterval = dticks;
}

void AWSprite::restore_images() {
	SDL_Surface ** oldFrames = frames;

	// Release modified images
	for (Uint32 f = 0; f < howManyFrames; f++) {
		SDL_FreeSurface(frames[f]);
	}
	delete[] frames;
	frames = NULL;
	// See if we we are using per-surface transparency if we are we will have to
	// restore transparency after restoring the frame pixel data.
	bool usingColorKey = true;	//v5.0
	//bool usingColorKey = ((wholeSurface->flags & SDL_SRCCOLORKEY) == SDL_SRCCOLORKEY);	//v5.0

	//// Get the flags for alpha control and the per-surface colorkey
	Uint32 alphaFlags = (wholeSurface->flags & (SDL_RLEACCEL));	//v5.0
	Uint32 alphaColorKey = wholeSurface->format->format;

	// Recreate images taking into account per-pixel or per-surface alpha blending
	if (uses_per_pixel_alpha)
		frames = create_frames_with_alpha(nbr_of_img_cols, nbr_of_img_rows);	// pp-alpha
	else {
		frames = create_frames(nbr_of_img_cols, nbr_of_img_rows);	// no pp-alpha
		if (usingColorKey) {	// ps-alpha (so reset it)
			SDL_SetColorKey(wholeSurface, alphaFlags, alphaColorKey);
			for (Uint32 f = 0; f < howManyFrames; f++) {
				SDL_SetColorKey(frames[f], alphaFlags, alphaColorKey);
			}
		}
	}
	surface = frames[currentFrame];

	if (sharedImage) {
		ImageData* idata = findImageBySurface(wholeSurface, this);
		FileData* fdata = findImageByFilename(idata->fnHash);
		// Set new values for frames
		fdata->frames = frames;
		restoreImageDataFromFileData(wholeSurface, fdata);
	}
}

// This method makes a circular area of pixels centerd arround hit_x, hit_y
// transparent. This works fine with sprites using per-pixel transparency.
// To work correctly on other sprites you must have previously set a transparent
// colour.
void AWSprite::bite(int biteRadius) {
	bite(hit_x, hit_y, biteRadius);
}

// This method makes a circular area of pixels centerd arround x, y
//  transparent. This works fine with sprites using per-pixel transparency.
// To work correctly on othr sprites you must have previously set a transparent
//  colour.
void AWSprite::bite(int x, int y, int biteRadius) {		//v5.0	****REMOVE???
	// If we do not have 32 bit graphics ie 4 bytes per pixel forget it
	int bpp = surface->format->BytesPerPixel;
	if (bpp != 4) return;

	int height = this->get_height();
	int width = this->get_width();

	int x1 = x - biteRadius;
	int y1 = y - biteRadius;
	int x2 = x + biteRadius;
	int y2 = y + biteRadius;
	if (x1 < 0) x1 = 0;
	if (y1 < 0) y1 = 0;
	if (x2 >= width) x2 = width;
	if (y2 >= height) y2 = height;

	int br2 = biteRadius * biteRadius;

//	Uint8 *p;

	//bool uses_per_pixel_alpha = ((wholeSurface->flags & SDL_SRCALPHA) == SDL_SRCALPHA);	//****
	//Uint8 r, g, b, a;
	//Uint32 colour;

	//for (Uint32 h = 0; h < howManyFrames; h++) {
	//	SDL_LockSurface(frames[h]);
	//	for (int px = x1; px < x2; px++) {
	//		for (int py = y1; py < y2; py++) {
	//			if ((px - x)*(px - x) + (py - y)*(py - y) <= br2) {
	//				p = (Uint8 *)frames[h]->pixels + (py * frames[h]->pitch) + (px * bpp);
	//				if (!uses_per_pixel_alpha)
	//					(*(Uint32 *)p) = transparentColour;
	//				else {
	//					colour = *(Uint32 *)p;
	//					get_RGBA_components(frames[h], colour, r, g, b, a);
	//					(*(Uint32 *)p) = SDL_MapRGBA(frames[h]->format, r, g, b, 0);
	//				}
	//			}
	//		}
	//	}
	//	SDL_UnlockSurface(frames[h]);
	//}
}

/*  Bresenham algorithm.  Taken from Wikipedia
function line(x0, y0, x1, y1)
dx := abs(x1-x0)
dy := abs(y1-y0)
if x0 < x1 then sx := 1 else sx := -1
if y0 < y1 then sy := 1 else sy := -1
err := dx-dy

loop
setPixel(x0,y0)
if x0 = x1 and y0 = y1 exit loop
e2 := 2*err
if e2 > -dy then
err := err - dy
x0 := x0 + sx
end if
if e2 <  dx then
err := err + dx
y0 := y0 + sy
end if
end loop

draw line from x0,y0 to x1, y1 in specified colour
*/
//void AWSprite::drawLine(int x0, int y0, int x1, int y1, unsigned int colour) {
//	int dx, dy, sx, sy, err, err2, e2;
//	unsigned int drawpos;
//	dx = abs(x1 - x0);
//	dy = abs(y1 - y0);
//
//	sx = -1;
//	if (x0 < x1) sx = 1;
//	sy = -1;
//	if (y0 < y1) sy = 1;
//	err = dx - dy;
//
//	SDL_SetRenderDrawColor(GameBase::renderer, 255, 255, 255, 255);	//****
//	do {	//     setPixel(x0,y0)
//		SDL_RenderDrawPoint(GameBase::renderer, x0, y0); //Renders at x,y in curr colour
//		e2 = 2 * err;
//		if (e2 > -dy) {
//			err = err - dy;
//			x0 = x0 + sx;
//		}
//		if (e2 <  dx) {
//			err = err + dx;
//			y0 = y0 + sy;
//		}
//	} while (x0 != x1 || y0 != y1);
////	SDL_RenderPresent(GameBase::renderer);	//should ONLY be called ONCE, in GameBase, else FLICKER!
//}
///*
//Function to draw a filled in rectangle ON a surface.
//
//Note: colour created as follows:  colour = SDL_MapRGB(surface->format,red, green, blue);
//SDL_Rect obj has 4 values x, y, w and h representing the start pt and width & height
//*/
//void AWSprite::drawFilledRect(SDL_Rect &r, unsigned int colour) {
//	//if (r.x < 0 || r.w > surf->w || r.y < 0 || r.h > surf->h) return; //don't draw outside surf!
//	//SDL_FillRect(surf, &r, colour);
//	SDL_SetRenderDrawColor(GameBase::renderer, 255, 0, 0, 255);	//****
//	SDL_RenderFillRect(GameBase::renderer, &r);
//}

//void AWSprite::drawCross(int x, int y, int len){
//	drawLine(x - len, y, x + len, y, 1);
//	drawLine(x, y-len, x, y + len, 1);
//}
