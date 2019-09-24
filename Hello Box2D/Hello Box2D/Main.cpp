//#include "Box2D/Box2D.lib"
#include "Box2D/Box2D.h"

//#include <stdio.h>
#pragma comment ( lib, "Box2D/Box2D.lib")


int main(int argc, char** argv) {

	b2Vec2 gravity(0.0f, -10.0f);

	b2World world(gravity);
	
	return 0;
}