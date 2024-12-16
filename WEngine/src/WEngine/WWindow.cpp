#include "WWindow.h"
#include <SDL.h>
#include <iostream>


namespace WE
{
	WWindow::WWindow(std::string wName, int wWidth, int wHeight)
	{
		// INIT SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0) std::cout << "failed to initialize video" << std::endl;
		else std::cout << "video initialized" << std::endl;

		window = SDL_CreateWindow(wName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wWidth, wHeight, SDL_WINDOW_OPENGL);
		if (window == nullptr) std::cout << "failed to create window" << std::endl;
		else std::cout << "window created" << std::endl;

		renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderTarget == nullptr) std::cout << "failed to create renderTarget" << std::endl;
		else std::cout << "renderTarget created" << std::endl;

	}

	WWindow::~WWindow()
	{
		SDL_DestroyWindow(window);
		window = nullptr;
		SDL_DestroyRenderer(renderTarget);
		renderTarget = nullptr;

		SDL_Quit;
	}
}

