#include "Game.h"
#undef main

int main(int argc, char** argv)
{
	bool isGameInit = Game::instance().initialize();
	if (isGameInit)
	{
		Game::instance().loop();
	}
	Game::instance().close();

	return 0;
}