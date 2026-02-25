#include "CoroutineInstance.h"
#include "Entity.h"

#include "GameContext.h"
#include <iostream>

namespace WE
{

	CoroutineInstance::CoroutineInstance(Entity* caller, uint32_t callerID, int ID, float Duration)
	{
		CoroutineCaller = caller;
		CallerID = callerID;
		CoroutineID = ID;
		CoroutineDuration = Duration;
	}

	CoroutineInstance::~CoroutineInstance()
	{

	}

	void CoroutineInstance::Start()
	{
		Entity::Start();
	}

	void CoroutineInstance::Update(float deltaTime)
	{
		Entity::Update(deltaTime);

		float timeAlive = GetTimeAlive();

		if (!GetGameContext()->IsValid(CoroutineCaller, CallerID))
			Destroy();
		else if (timeAlive < CoroutineDuration)
			CoroutineCaller->OnCoroutineUpdate(CoroutineID, timeAlive);
		else
		{
			CoroutineCaller->OnCoroutineUpdate(CoroutineID, CoroutineDuration);
			CoroutineCaller->OnCoroutineEnd(CoroutineID);
			Destroy();
		}
	}
}