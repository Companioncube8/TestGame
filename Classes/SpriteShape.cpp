#include "SpriteShape.h"

SpriteShape* SpriteShape::create(int row, int col, int index)
{
	auto spriteCache = SpriteFrameCache::getInstance();
	spriteCache->addSpriteFramesWithFile("plist/Tiles.plist");
	SpriteShape* sprite = new SpriteShape();
	sprite->setRow(row);
	sprite->setCol(col);
	sprite->setImageIndex(index);
	sprite->initWithSpriteFrameName(spriteNormal[sprite->getImageIndex()]);
	
	return sprite;
}

SpriteShape::SpriteShape() noexcept
{
	m_row = 0; m_col = 0;m_imageIndex = 0;
}