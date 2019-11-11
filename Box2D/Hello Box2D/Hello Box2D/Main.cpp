//#include "Box2D/Box2D.lib"
#include "SDL/include/SDL.h"
#include "Box2D/Box2D.h"

//#include <stdio.h>
#pragma comment ( lib, "SDL/libx86/SDL2.lib")
#pragma comment ( lib, "SDL/libx86/SDL2main.lib")
#pragma comment ( lib, "Box2D/Box2D.lib")



int main(int argc, char* argv[]) //int argc, char* argv[]
{

	//b2Vec2 gravity(0.0f, -10.0f);
	SDL_Window* mWindow = SDL_CreateWindow("Physics", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	//b2World world(gravity);
	
	system("pause");
	return 0;
}