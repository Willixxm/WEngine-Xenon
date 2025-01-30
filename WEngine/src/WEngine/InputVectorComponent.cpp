#include "InputVectorComponent.h"
#include "WMath.h"
#include <iostream>

namespace WE
{

	InputVectorComponent::InputVectorComponent()
	{
		KeyInput = WVec2(0);
	}

	void InputVectorComponent::AddInputValue(InputType source, float xValue, float yValue)
	{
		
		switch (source)
		{
		case InputType::Key:
			KeyInput.x = KeyInput.x + xValue;
			KeyInput.y = KeyInput.y + yValue;
			break;
		case InputType::Axis:
			AxisInput.x = xValue;
			AxisInput.y = yValue;
			break;
		}

	}

	WVec2 InputVectorComponent::GetInputVectorAndReset()
	{
		if (!inputEnabled)
			return WVec2(0);

		ProcessInputVector(AxisInput, axisDeadZoneInner, axisDeadZoneOutter);
		ProcessInputVector(KeyInput, 0, 0);

		WVec2 finalInput; 
		if (preferController)
		{
			finalInput = (AxisInput.x || AxisInput.y) ? AxisInput : KeyInput;
		}
		else
		{
			finalInput = (KeyInput.x || KeyInput.y) ? KeyInput : AxisInput;
		}

		KeyInput = WVec2(0);
		AxisInput = WVec2(0);

		lastInputVector = finalInput;
		return finalInput;
	}

	void InputVectorComponent::ProcessInputVector(WVec2& rawVec, float innerDeadZone, float outterDeadZone)
	{
		WVec2 processedInput = rawVec;

		float vecLength = std::sqrt(processedInput.x * processedInput.x + processedInput.y * processedInput.y);
		WVec2 unitVector = WVec2(processedInput.x / vecLength, processedInput.y / vecLength);

		processedInput = WVec2( //scaling the vector to accout for outer deadzone
			processedInput.x / (1 - axisDeadZoneOutter),
			processedInput.y / (1 - axisDeadZoneOutter));

		//recalculate length due to scaling
		vecLength = std::sqrt(processedInput.x * processedInput.x + processedInput.y * processedInput.y);

		//clamp vector 
		if (vecLength > 1)
		{
			processedInput = WVec2(processedInput.x / vecLength, processedInput.y / vecLength); //clamp inputs to -1, 1
			vecLength = std::sqrt(processedInput.x * processedInput.x + processedInput.y * processedInput.y);
		}
		else if (vecLength < axisDeadZoneInner)
		{
			rawVec = WVec2(0);
			return;
		}

		processedInput = WVec2( //offsetting the vector to accout for inner deadzone
			(processedInput.x - unitVector.x * axisDeadZoneInner) / (1 - axisDeadZoneInner),
			(processedInput.y - unitVector.y * axisDeadZoneInner) / (1 - axisDeadZoneInner));


		rawVec = processedInput;
		
	}
}