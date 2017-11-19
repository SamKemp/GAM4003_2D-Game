#pragma once
#include <string>
#include "GameBase.h"
#include "SDL_ttf.h"	//****

using namespace std;

class TrueTypeFont{
	GameBase *m_game;
	SDL_Color m_fontColour;
	int m_fontSize;
	TTF_Font *m_font = NULL;	//****
	int m_textWidth, m_fontHeight;
	string m_fontTTFFile, currText;
	SDL_Texture *m_fontTexture; //****
public:
	TrueTypeFont();
	void init();
	TrueTypeFont(GameBase *game, string fontFile = "SDL/9Systema.ttf");
	~TrueTypeFont();

	//Creates image from font string 
	bool loadFromRenderedText(std::string textureText);
	bool loadFont(int fontSize);
	int renderFont(string txt, int x, int y);
	int renderFont(int num, int x, int y);
	int renderFont(float num, int x, int y, int nod = 1);
	void setFontSizeAndColour(int fs, int r = 255, int g = 255, int b = 255);
	void setFontFile(string ttfFileName){ m_fontTTFFile = ttfFileName;  }
	void setGamePtr(GameBase *game){ m_game = game;	}
	int getTextWidth(){ return m_textWidth; }
	int getFontHeight(){ return m_fontHeight; }
	int getFontSize() { return m_fontSize; }
};

