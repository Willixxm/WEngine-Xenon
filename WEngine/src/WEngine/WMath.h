#pragma once

#include "Core.h"
#include <cmath>

namespace WE
{
	class WE_API WVec2
	{
	public:
		WVec2(float _x, float _y) : x{ _x }, y{ _y } {}
		WVec2(float xy) : WVec2(xy, xy) {}
		WVec2() : WVec2(0.f) {}

		WVec2& operator+=(const WVec2& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		WVec2& operator*=(float scalar)
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}

		float length()
		{
			return sqrt(pow(x, 2) + pow(y, 2));
		}

	public:
		float x = 0;
		float y = 0;
	};


	inline WVec2 operator*(const WVec2& vec, float scalar)
	{
		return WVec2(vec.x * scalar, vec.y * scalar);
	}

	inline WVec2 operator*(float scalar, const WVec2& vec)
	{
		return vec * scalar;
	}

	inline WVec2 operator+(const WVec2& vec1, const WVec2& vec2)
	{
		return WVec2(vec1.x + vec2.x, vec1.y + vec2.y);
	}

	inline WVec2 operator-(const WVec2& vec1, const WVec2& vec2)
	{
		return WVec2(vec1.x - vec2.x, vec1.y - vec2.y);
	}
	
}
