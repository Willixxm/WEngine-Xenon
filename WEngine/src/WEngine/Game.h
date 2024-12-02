#pragma once

#include "Core.h"
#include <string>

namespace WE
{

	class WE_API Game
	{
	protected:
		int windowWidth = 600; //default placeholder values
		int windowHeight = 600;
		std::string gameName = "Game";
	
		
	public:
		Game();
		virtual ~Game();

		virtual void Start();


		inline int GetWindowWidth() const { return windowWidth; }
		inline int GetWindowHeight() const { return windowHeight; }
		inline std::string GetWindowName() const { return gameName; }

	};

	// Defined by game
	Game* CreateGame(); 


}
