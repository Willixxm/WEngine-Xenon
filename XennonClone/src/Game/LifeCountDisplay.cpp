#include "LifeCountDisplay.h"
#include "WEngine/GameContext.h"

void LifeCountDisplay::SetExtraLifeCount(int extraLives)
{
	extraLifeCount = extraLives;
	if (extraLifeCount < 0)
		extraLifeCount = 0;

	UpdateIcons();

}

void LifeCountDisplay::UpdateIcons()
{
	if (extraLifeCount > icons.size())
	{ // add new icons
		for (size_t i = icons.size(); i < extraLifeCount; ++i)
		{
			auto icon = GetGameContext()->GAME_InstantiateEntity<Entity>(GetLocation() + GetRightVector() * GetInitialSize().length() * iconOffsetMult * i, iconRotation, GetInitialSize());
			GetGameContext()->RENDER_AddRenderComponent(icon, filePath, hTiles, vTiles, tileOffset, tileSpan, 20);
			GetGameContext()->RENDER_SetAnimationParameters(icon, false);
			LifeIcon lifeIcon;
			lifeIcon.icon = icon;
			lifeIcon.id = icon->GetID();
			
			icons.push_back(lifeIcon);
		}
	}
	else
	{ // remove icons
		for (size_t i = extraLifeCount; i < icons.size(); ++i)
		{
			if (GetGameContext()->IsValid(icons[i].icon, icons[i].id))
			{
				icons[i].icon->Destroy();
				icons.erase(icons.begin() + i);
				--i;
			}
		}
	}
}