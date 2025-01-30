#include "Game.h"
#include "GameContext.h"
#include <memory>


namespace WE
{
	Game::Game() 
	{ 
	}

	Game::~Game()
	{
	}

	void Game::Start()
	{ 
		gameContext = std::make_unique<GameContext>(this);

		startTime = std::chrono::high_resolution_clock::now();
		lastFrameTime = startTime;
	}	

	void Game::Update()
	{
		//delta seconds 
		float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>
			(std::chrono::high_resolution_clock::now() - lastFrameTime).count() * 0.000001f; //(nanosecond precision)
		lastFrameTime = std::chrono::high_resolution_clock::now();


		gameContext->SYSTEM_UpdatePhysics(deltaTime);
		gameContext->SYSTEM_UpdateEventSystem();
		gameContext->SYSTEM_UpdateEntities(deltaTime);
		gameContext->SYSTEM_Render(deltaTime);
	}

	
}

