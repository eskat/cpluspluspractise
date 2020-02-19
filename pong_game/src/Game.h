#include <SDL.h>

//For our ball and paddle positions
struct Vector2
	{
		float x;
		float y;
	};

//Game class
class Game {

public:

	Game();
	// Initialize the game
	bool Initialize();
	// Runs game loop until the game is over
	void RunLoop();
	// Shutdown the game
	void Shutdown();

private:
	//Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	//Tick count for delta
	Uint32 mTicksCount;

	// Window created by SDL
	SDL_Window* mWindow;
	// Renderer created by SDL
	SDL_Renderer* mRenderer;

	//Ball and paddle
	Vector2 mBallPos;
	Vector2 mPaddlePos;

	//Direction of paddle
	int mPaddleDir;

	//Ball velocity
	Vector2 mBallVel;

	// Status of game running
	bool mIsRunning;

};
