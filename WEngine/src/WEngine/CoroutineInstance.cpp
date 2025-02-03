#include "CoroutineInstance.h"
#include "Entity.h"

#include "GameContext.h"
#include <iostream>

namespace WE
{

	CoroutineInstance::CoroutineInstance(Entity* caller, int ID, float Duration)
	{
		CoroutineCaller = caller;
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

		if (!CoroutineCaller)
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