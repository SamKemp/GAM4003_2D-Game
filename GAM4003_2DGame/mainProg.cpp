#include <iostream>
#include <string>

#include "FroggyCity.h"

using namespace std;

int main(int argc, char* argv[]) {
	GameBase::initSDL(1840, 1000, true, "Froggy City Game");

	FroggyCity fc;

	fc.setup();
	fc.start();

	return 0;
}