#include "Net.h"

//initialize SDL_net
void CNet::Init()
{
	//initialize sdl_net
	SDLNet_Init();
}

//quit SDL_net
void CNet::Quit()
{
	//quit sdl_net
	SDLNet_Quit();
}
