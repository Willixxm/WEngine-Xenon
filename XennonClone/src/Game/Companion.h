#pragma once
#include "SpaceShip.h"
//#include "WEngine/GameContext.h"

class Companion : public SpaceShip
{
public:
	Companion();

	void Start() override;
	void HandleMovement(float deltaTime) override;

	void SetPlayer(SpaceShip* player, uint32_t playerID, int companionSlot);
	void Destroy() override;

	bool OnPickUp_Companion(const PowerUp* powerUp) override { return false; }

	int GetSlot() const { return companionSlot; }

	void OnCoroutineUpdate(int ID, float duration) override;
	void OnCoroutineEnd(int ID) override;


protected:
	SpaceShip* player = nullptr;
	uint32_t playerID = 0;

	int companionSlot = -1;
	WE::WVec2 positionOffset = WE::WVec2(0);

	float moveToPlayerDuration = 0.5f;
	bool isMovingToPlayer = false;
	WE::WVec2 initLocation = WE::WVec2(0);

	float randOscillationSpeedMult = 1.f;

private:
	enum CoroutineID : int
	{
		moveToPlayer = 2,
	};

	
};

