#pragma once

#include "Core.h"

namespace WE
{

	class WE_API Game
	{

	public:
		Game();
		virtual ~Game();

		virtual void Run();

	};

	// Defined by game
	Game* CreateGame(); 


}
