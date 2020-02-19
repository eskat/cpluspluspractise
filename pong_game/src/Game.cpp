#include "Game.h"

//Thickness of the paddle
const int thickness = 15;
const float paddleH = 100.0f;

Game::Game() :mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mPaddleDir(0), mIsRunning(true){

}

bool Game::Initialize() {

	//Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0){
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//Create a window with top left x and y coordinates, width, height, and flags
	mWindow = SDL_CreateWindow(
			"Pong Game",
			100,
			100,
			1024,
			768,
			0
			);

	if(!mWindow){
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//Create the renderer with the Window, what graphics driver (-1 lets SDL decide)
	//Use accelerated renderer and vsync
	mRenderer = SDL_CreateRenderer(
			mWindow,
			-1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
			);

	if(!mRenderer){
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	//Initialize the coordinates of the ball to the centre of the srceen
	mBallPos.x = 1024.0f/2.0f;
	mBallPos.y = 768.0f/2.0f;

	//Initialize the coordinates of the paddle to the left side centre
	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f/2.0f;

	//Initialize the ball velocity
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;

	return true;
}

void Game::RunLoop(){
	while(mIsRunning){
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput(){
	SDL_Event event;
	// While there are still events in the queue
	while (SDL_PollEvent(&event)){

		switch (event.type){
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}

	//Get the state of the keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	//If ESC pressed also end loop
	if (state[SDL_SCANCODE_ESCAPE]){
		mIsRunning = false;
	}

	//Get the state of the paddle movement
	//Add and subtract so both keys pressed at once = 0
	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W]){
		mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S]){
		mPaddleDir += 1;
	}


}

void Game::UpdateGame(){

	//Wait until 16ms has elapsed since last frame (frame limiting) 60fps = 16.6ms per frame
	while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	//Get delta in seconds
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	//Update ticks count for next frame
	mTicksCount = SDL_GetTicks();

	//Clamp maximum delta value
	if (deltaTime > 0.05f){
		deltaTime = 0.05f;
	}

	//Update paddle position 400px per second
	if (mPaddleDir != 0){
		mPaddlePos.y += mPaddleDir * 400.0f * deltaTime;

		//Make sure paddle is bound to the walls
		if (mPaddlePos.y < (paddleH/2.0f + thickness)){
			mPaddlePos.y = paddleH/2.0f + thickness;
		} else if (mPaddlePos.y > (768.0f - paddleH/2.0f - thickness)){
			mPaddlePos.y = 768.0f - paddleH/2.0f - thickness;
		}
	}

	//Move ball
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	//Check if ball hit paddle or not and negate x if it did
	float diff = mPaddlePos.y - mBallPos.y;
	// Take absolute value of difference
	diff = (diff > 0.0f) ? diff : -diff;

	//if ball hits paddle bounce off paddle
	if (diff <= paddleH / 2.0f && mBallPos.x <= 25.0 && mBallPos.x >=20.0f && mBallVel.x < 0.0f){
		mBallVel.x *= -1.0f;
	//if ball hits far wall bounce back
	} else if (mBallPos.x >= 1024.0f && mBallVel.x > 0.0f){
		mBallVel.x *= -1.0f;
	//if you miss, game ends
	} else if (mBallPos.x <= 0.0f){
		mIsRunning = false;
	}

	//If ball hits wall, negate velocity in the y direction only if the ball is moving towards the wall
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f){
		mBallVel.y *= -1.0f;
	} else if (mBallPos.y >= (768 - thickness) && mBallVel.y > 0.0f){
		mBallVel.y *= -1.0f;
	}

}

void Game::GenerateOutput(){

	SDL_SetRenderDrawColor(
			mRenderer,
			0,   //R
			0,   //G
			255, //B
			255  //A
			);

	//Clear back buffer to the current draw colour
	SDL_RenderClear(mRenderer);

	//Draw the walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	SDL_Rect top_wall{0, 0, 1024, thickness};
	SDL_RenderFillRect(mRenderer, &top_wall);

	SDL_Rect bottom_wall{0, 768-thickness, 1024, thickness};
	SDL_RenderFillRect(mRenderer, &bottom_wall);

	//Draw the ball
	//The member vars rep the centre points of the paddle and ball, but this uses the top left point
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness/2),
		static_cast<int>(mBallPos.y - thickness/2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	//Draw the paddle x, y, w, h
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	//Swap the front and back buffers
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown(){
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();

}
