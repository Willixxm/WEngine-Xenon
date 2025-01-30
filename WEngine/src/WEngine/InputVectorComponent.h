#pragma once

#include "WMath.h"
#include <cmath>

namespace WE
{

	enum WE_API InputType : int
	{
		Key = 0,
		Axis = 1
	};

	class WE_API InputVectorComponent
	{
	public:
		InputVectorComponent();
		

		void AddInputValue(InputType source, float xValue, float yValue);
		WVec2 GetInputVectorAndReset();

	private:
		void ProcessInputVector(WVec2& rawVec, float innerDeadZone, float outterDeadZone);

	private:
		WVec2 KeyInput = WVec2();
		WVec2 AxisInput = WVec2();
		WVec2 lastInputVector = WVec2();

	public:
		float axisDeadZoneInner = 0.5f;
		float axisDeadZoneOutter = 0.1f;
		bool preferController = true;

	public:
		bool inputEnabled = true;
		

	};
}


