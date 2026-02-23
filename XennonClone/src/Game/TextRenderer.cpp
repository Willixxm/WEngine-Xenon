#include "TextRenderer.h"
#include "WEngine/WMath.h"
#include "WEngine/GameContext.h"
#include <iostream>

using namespace WE;

void TextRenderer::Start()
{
	
}

void TextRenderer::Update(float deltaTime)
{

}

void TextRenderer::SetLocation(WVec2 pos)
{
	Entity::SetLocation(pos);

	RenderText(); //updates location
}

void TextRenderer::SetRotation(float newRotationRad)
{
	Entity::SetRotation(newRotationRad);

	RenderText();
}

void TextRenderer::SetFont(bool largeFont)
{
	useLargeFont = largeFont;
	RenderText();
}

void TextRenderer::SetText(std::string newText)
{
	inputVectorText = newText;
	RenderText();
}

void TextRenderer::RenderText()
{
	float rotation = GetRotation();
	WVec2 location;
	WVec2 centeredOffset = WVec2(0);
	if (textCentered)
		centeredOffset = WVec2( -((int)inputVectorText.size() - 1) * GetInitialSize().x / 2.0f, 0);

	auto rightVec = GetRightVector();

	for (size_t i = 0; i < inputVectorText.size(); ++i)
	{			
		location = GetLocation() + rightVec * GetInitialSize().x * i + centeredOffset;
		
		if (i + 1 > textTiles.size())
		{
			Entity* tile = GetGameContext()->GAME_InstantiateEntity<Entity>(location, rotation, GetInitialSize());
			GetGameContext()->RENDER_AddRenderComponent(tile, 
				useLargeFont ? largeFontPath : smallFontPath, 
				hTileCount, 
				useLargeFont ? vTileCount : vTileCount_smallFont, 
				inputVectorText[i] - 32, 1, renderLayer);
			GetGameContext()->RENDER_SetAnimationParameters(tile, false);
			textTiles.push_back(tile);
		}
		else
		{
			textTiles[i]->SetLocation(location);
			textTiles[i]->SetRotation(rotation);
			GetGameContext()->RENDER_SetAnimationTileParameters(textTiles[i], inputVectorText[i] - 32, 1);
		}
	}
	if (textTiles.size() > inputVectorText.size())
	{
		for (size_t i = inputVectorText.size(); i < textTiles.size(); ++i)
		{
			if (autoDeleteUnusedTiles)
			{
				auto tile = textTiles[i];
				textTiles.erase(textTiles.begin() + i);
				tile->Destroy();
				--i;
			}
			else
			{
				GetGameContext()->RENDER_SetAnimationTileParameters(textTiles[i], 0, 1);
			}
		}
	}
}

void TextRenderer::DestroyWithFloat(WVec2 floatHere, float floatDuration)
{
	anim_startLocation = GetLocation();
	anim_floatDuration = floatDuration;
	anim_floatHere = floatHere;
	GetGameContext()->GAME_StartCoroutine(this, CoroutineID::floatAnimation, floatDuration);
}

void TextRenderer::OnCoroutineUpdate(int ID, float duration)
{
	switch (ID)
	{
	case CoroutineID::floatAnimation:
	{
		float state = duration / anim_floatDuration;
		SetLocation(anim_startLocation + anim_floatHere * state);
	}
		break; 
		
	}

}

void TextRenderer::OnCoroutineEnd(int ID)
{
	switch (ID)
	{
	case CoroutineID::floatAnimation:
		Destroy();
		break;
	}
}

void TextRenderer::Destroy()
{
	for (auto entity : textTiles)
		entity->Destroy();
	
	Entity::Destroy();
}