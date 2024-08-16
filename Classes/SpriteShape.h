#ifndef  __SPRITE_SHAPE__
#define __SPRITE_SHAPE__

#include "cocos2d.h"
#include "time.h"
#include "GameDefine.h"

USING_NS_CC;

class SpriteShape : public Sprite
{
public:
	static SpriteShape* create(int row, int col, int index);
	SpriteShape() noexcept;

	CC_SYNTHESIZE(int, m_row, Row);
	CC_SYNTHESIZE(int, m_col, Col);
	CC_SYNTHESIZE(int, m_imageIndex, ImageIndex);

};
#endif // ! __SPRITE_SHAPE__