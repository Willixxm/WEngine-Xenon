#pragma once

#include "Core.h"
#include <string>

class SDL_Window;
class SDL_Renderer;

namespace WE
{
	class WE_API WSDLWindow
	{
	private:
		SDL_Window* window = nullptr;
		SDL_Renderer* renderTarget = nullptr;
	public:
		WSDLWindow(std::string wName, int wWidth, int wHeight);
		~WSDLWindow();

	};
}



