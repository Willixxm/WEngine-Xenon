#pragma once

#include "Core.h"

namespace WE
{
	class WE_API WInputSys
	{
		WInputSys();
		~WInputSys();

		bool IsKeyDown(char key);
		bool IsKeyUp(char key);
		bool IsKeyHold(char key);

	};
}

