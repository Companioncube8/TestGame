#ifndef __GAME_DEFINE__
#define __GAME_DEFINE__

#include "cocos2d.h"

USING_NS_CC;

constexpr int designResolutionWidth = 1280;
constexpr int designResolutionHeight = 1024;

constexpr int spriteNum = 3;
constexpr int spriteWidth = 60;
constexpr int spriteHeight = 60;

constexpr int borderWidth = 3;

constexpr int countOfColors = 3;

constexpr char* spriteNormal[spriteNum] =
{
	"blueTile.png","greenTile.png","redTile.png"
};

#endif // !__GAME_DEFINE__