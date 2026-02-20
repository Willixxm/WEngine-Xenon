#pragma once

#include "WEngine/Entity.h"
#include <string>

using namespace WE;
class WE::WVec2;

class TextRenderer : public WE::Entity
{
public:
	void Start() override;
	void Update(float deltaTime) override;

	void SetText(std::string newText);

	void SetLocation(WE::WVec2 pos) override;
	void SetRotation(float newRotationRad) override;

	void OnCoroutineUpdate(int ID, float duration) override;
	void OnCoroutineEnd(int ID) override;

	void Destroy() override;

public:
	void DestroyWithFloat(WVec2 floatHere_offsetPos, float floatDuration);
private:
	WVec2 anim_startLocation;
	WVec2 anim_floatHere;
	float anim_floatDuration;


private:
	void RenderText();

protected:
	std::string inputVectorText;

	int renderLayer = 20;
	std::vector<Entity*> textTiles;
	std::string filePath = "graphics/font16x16.bmp";

	int hTileCount = 8;
	int vTileCount = 12;

public:
	bool textCentered = false;
	bool autoDeleteUnusedTiles = false;
	unsigned int maxTiles = 24;

private:
	enum CoroutineID : int
	{
		floatAnimation = 0
	};

};

