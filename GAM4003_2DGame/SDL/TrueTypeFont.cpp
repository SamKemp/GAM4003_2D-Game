#include "TrueTypeFont.h"

TrueTypeFont::TrueTypeFont(){
	init();
	m_fontTTFFile = "SDL/9Systema.ttf";
	loadFont(m_fontSize);
}

void TrueTypeFont::init(){
	m_fontColour = { 255, 255, 255 };	//set default values
	m_fontSize = 20;
	m_fontTexture = NULL;
}

TrueTypeFont::TrueTypeFont(GameBase *game, string fontFile){
	m_game = game;
	init();
	m_fontTTFFile = fontFile;
	loadFont(m_fontSize);
}

TrueTypeFont::~TrueTypeFont(){
	//Close the font that was used
	TTF_CloseFont(m_font);	//****
	if (m_fontTexture)
		SDL_DestroyTexture(m_fontTexture);
}

bool TrueTypeFont::loadFromRenderedText(std::string textureText){
	//Render text surface 
	if (m_font == NULL) return false;
	if (currText == textureText) return true; //
	currText = textureText;
	SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, textureText.c_str(), m_fontColour);
	if (textSurface == NULL) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	} else {
		if (m_fontTexture) {
			SDL_DestroyTexture(m_fontTexture);
			m_fontTexture = NULL;
		}
		//Create texture from surface pixels 
		m_fontTexture = SDL_CreateTextureFromSurface(m_game->renderer, textSurface);
		if (m_fontTexture == NULL) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		} else {
			//Get image dimensions 
			m_textWidth = textSurface->w;
			m_fontHeight = textSurface->h;
		}
		//Get rid of old surface 
		SDL_FreeSurface(textSurface);
	}
	//Return success 
	return m_fontTexture != NULL;
}

bool TrueTypeFont::loadFont(int fontSize){
	if (m_font != NULL)	//Open the font 
		TTF_CloseFont(m_font);
	m_font = TTF_OpenFont(m_fontTTFFile.c_str(), m_fontSize);
	if (m_font == NULL) {
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	} 
	TTF_SetFontHinting(m_font, TTF_HINTING_NORMAL);	//TTF_HINTING_NORMAL
	return true;
}

int TrueTypeFont::renderFont(string txt, int x, int y){
	if (!loadFromRenderedText(txt)) {	//Render text 
		printf("Failed to render text texture!\n");
		return false;
	}

	SDL_Rect src, dest;
	src.x = 0;
	src.y = 0;
	src.w = m_textWidth;
	src.h = m_fontHeight;
	dest.x = x;
	dest.y = y;
	dest.w = src.w; dest.h = src.h;

	if (m_fontTexture != NULL) {
		SDL_RenderCopy(m_game->renderer, m_fontTexture, &src, &dest);	//render
		return x + m_textWidth;
	}
	return false;
}

int TrueTypeFont::renderFont(int num, int x, int y){
	char s[20];
	sprintf_s(s, "%d", num);
	return renderFont(s, x, y);
}

int TrueTypeFont::renderFont(float num, int x, int y, int nod){
	char s[20], fmt[20];
	sprintf_s(fmt, "%%.%df", nod);
	sprintf_s(s, fmt, num);
	return renderFont(s, x, y);
}

void TrueTypeFont::setFontSizeAndColour(int fs, int r, int g, int b){
	if (fs > 1) {
		m_fontSize = fs;
		loadFont(m_fontSize);
	}
	m_fontColour = { (Uint8)r,(Uint8)g,(Uint8)b };
}
