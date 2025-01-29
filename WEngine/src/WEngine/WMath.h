#pragma once

#include "Core.h"

namespace WE
{
	class WE_API WVec2
	{
	public:
		WVec2(float _x, float _y) : x{ _x }, y{ _y } {}
		WVec2(float xy) : WVec2(xy, xy) {}

	public:
		float x = 0;
		float y = 0;
	};
}
