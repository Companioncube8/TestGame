#ifndef __GAME_SCENE__
#define __GAME_SCENE__

#include "cocos2d.h"
#include "SpriteShape.h"

USING_NS_CC;

class GameScene : public cocos2d::Scene
{
public:
	typedef std::map <Point, SpriteShape* > BlockMap;
	typedef std::pair<Point, SpriteShape*> BlockPair;

	static Scene* createScene();
	CREATE_FUNC(GameScene);

protected:
	virtual bool init();
	void initMap();
	void createSprite(int row, int col, int index);

	Point positionOfItem(int row, int col);

	void update(float dt) override;

	void explodeSprite(SpriteShape* sprite);
	void fillSprite();

	void setResolutionScale();
	SpriteShape* findSprite(int row, int col);
	SpriteShape* findSprite(const Point& point);

	void actionEndCallback(Node* node);
	void onMouseDown(Event* event);

	void startCallback(Ref* pSender);
private:
	bool isFillSprite = false;

	int rows = 10;
	int cols = 16;
	Vec2 origin;
	Size visibleSize;
	float scaleRatioX = 1;
	float scaleRatioY = 1;

	Point blockOrigin;

	BlockMap blocks;
	void floodFill(Point point);

	std::set<Point> RemoveBlocks;
};

#endif // !__GAME_SCENE__