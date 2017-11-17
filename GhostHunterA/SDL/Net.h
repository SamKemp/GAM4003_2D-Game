#ifndef NET_H__
#define NET_H__

//include sdl_net
#include "sdl_net.h"

//CNet class
//initializes and quits SDL_net
//singleton
class CNet  
{
public:
	//initialize SDL_net
	static void Init();
	//quit SDL_net
	static void Quit();
};

#endif
