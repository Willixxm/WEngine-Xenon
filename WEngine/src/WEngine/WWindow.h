#pragma once

#include "Core.h"
#include <string>

class SDL_Window;
class SDL_Renderer;

namespace WE
{
	class WE_API WWindow
	{
	private:
		SDL_Window* window = nullptr;
		SDL_Renderer* renderTarget = nullptr;
	public:
		WWindow(std::string wName, int wWidth, int wHeight);
		~WWindow();

	};
}



