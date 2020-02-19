//============================================================================
// Name        : pong_game.cpp
// Author      : 
// Description : C++ Practise with SDL
//============================================================================

#include "Game.h"
#include <iostream>

int main(int argc, char** argv) {

	Game game;
	bool success = game.Initialize();
	if (success){
		std::cout << "Success" << std::endl;
		game.RunLoop();
	}

	game.Shutdown();

	return 0;
}
