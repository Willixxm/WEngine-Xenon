#pragma once

#include "Core.h"
#include "GameContext.h"
#include <string>
#include <memory>
#include <chrono>

namespace WE
{
	class GameContext;


	class WE_API Game
	{
	public:
		unsigned int windowWidth = 600; //default placeholder values
		unsigned int windowHeight = 600;
		const char* gameName = "Game";	
		WRenderEngine renderEngine = WRenderEngine::OpenGL;
		
	protected:
		std::unique_ptr<GameContext> gameContext;

	private:
		std::chrono::steady_clock::time_point startTime;
		std::chrono::steady_clock::time_point lastFrameTime;

		
	public:
		Game();
		virtual ~Game();

		virtual void Start();
		void Update();


		bool stopGame = false;

	};

	// Defined by game
	Game* CreateGame();


}
