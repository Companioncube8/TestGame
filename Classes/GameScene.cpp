#include "GameScene.h"
#include <CCDirector.h>
#include <iostream>

#include "ui/UITextField.h"

cocos2d::Scene* GameScene::createScene()
{
	return GameScene::create();
}

bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	else
	{
		setResolutionScale();

		auto touchListener = EventListenerMouse::create();
		touchListener->onMouseDown = CC_CALLBACK_1(GameScene::onMouseDown, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

		initMap();
		
		if (auto startItem = MenuItemImage::create("button/BlankNormal.png", "button/BlankNormal.png",CC_CALLBACK_1(GameScene::startCallback, this)))
		{
			startItem->setScale(scaleRatioX, scaleRatioY);
			startItem->setPosition(Vec2(origin.x + visibleSize.width - startItem->getContentSize().width, origin.y + visibleSize.height - startItem->getContentSize().height));
			auto startMenu = Menu::create(startItem, NULL);
			startMenu->setPosition(Vec2::ZERO);
			this->addChild(startMenu);
		}
		
		scheduleUpdate();
		return true;
	}
}

void GameScene::initMap()
{
	blockOrigin.x = origin.x + visibleSize.width / 2 - (scaleRatioX * spriteWidth + scaleRatioX * borderWidth) * cols / 2;
	blockOrigin.y = origin.y + visibleSize.height / 2 - (scaleRatioY * spriteHeight + scaleRatioY * borderWidth) * rows / 2;
	
	srand(time(0));
	int index = 0;
	for (int row = 0; row < rows; row++)
		for (int col = 0; col < cols; col++)
		{
			index = rand() % countOfColors;
			this->createSprite(row, col, index);
		}
}

void GameScene::createSprite(int row, int col, int index)
{
	SpriteShape* sprite = SpriteShape::create(row, col, index);

	sprite->setScale(scaleRatioX, scaleRatioY);
	sprite->setPosition(positionOfItem(row, col));
	this->addChild(sprite, 0);
	
	blocks.insert(BlockPair(Point(row, col), sprite));
}

Point GameScene::positionOfItem(int row, int col)
{
	float const x = blockOrigin.x + (scaleRatioX * spriteWidth + scaleRatioX * borderWidth) * col + scaleRatioX * spriteWidth / 2;
	float const y = blockOrigin.y + (scaleRatioY * spriteHeight + scaleRatioX * borderWidth) * row + scaleRatioY * spriteHeight / 2;
	return Point(x, y);
}

void GameScene::update(float dt)
{
	if (RemoveBlocks.size() == 0)
	{
		if (isFillSprite)
		{
			fillSprite();
			isFillSprite = false;
		}
	}
}

void GameScene::explodeSprite(SpriteShape* sprite)
{
	if (sprite == nullptr)
		return;
	auto explodeSpawn = Spawn::createWithTwoActions( ScaleTo::create(0.25, 0), FadeOut::create(0.25));
	auto explodeSequence = Sequence::create(explodeSpawn, CallFuncN::create(CC_CALLBACK_1(GameScene::actionEndCallback, this)), NULL);
	sprite->runAction(explodeSequence);
}

void GameScene::fillSprite()
{
	std::vector<int> colEmptyNum;
	SpriteShape* sprite = nullptr;
	for (int col = 0; col <cols; col++)
	{
		int removedCounter = 0;
		for (int row = 0; row < rows; row++)
		{
			sprite = findSprite(row, col);
			if (sprite == nullptr)
				removedCounter++;
			else
			{
				if (removedCounter > 0)
				{
					int const newRow = row - removedCounter;
					blocks.insert(BlockPair(Point(newRow, col), sprite));
					blocks.erase(Point(row, col));

					Point const endPosition = positionOfItem(newRow, col);
					Point const startPosition = sprite->getPosition();

					float const speed = (startPosition.y - endPosition.y) / visibleSize.height * 3;
					sprite->stopAllActions();
					sprite->runAction(MoveTo::create(speed, endPosition));
					sprite->setRow(newRow);
				}
			}
		}
		colEmptyNum.push_back(removedCounter);
	}
}

void GameScene::setResolutionScale()
{
	auto const winSize = Director::getInstance()->getWinSize();
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	scaleRatioX = winSize.width / designResolutionWidth;
	scaleRatioY = winSize.height / designResolutionHeight;
}

SpriteShape* GameScene::findSprite(int row, int col)
{
	BlockMap::iterator iter;
	iter = blocks.find(Point(row, col));
	if (iter != blocks.end())
		return ((*iter).second);
	return nullptr;
}

SpriteShape* GameScene::findSprite(const Point& point)
{
	SpriteShape* sprite = nullptr;
	Rect rectangle = Rect(0, 0, 0, 0);
	for (int row = 0; row < rows; row++)
		for (int col = 0; col < cols; col++)
		{
			sprite = findSprite(row, col);
			if (sprite != nullptr)
			{
				rectangle.origin.set(sprite->getPositionX() - scaleRatioX * spriteWidth / 2, sprite->getPositionY() - scaleRatioY * spriteHeight / 2);
				rectangle.size.setSize(scaleRatioX * spriteWidth, scaleRatioY * spriteHeight);
				if (rectangle.containsPoint(point))
				{
					return sprite;
				}
			}
		}
	return nullptr;
}

void GameScene::actionEndCallback(Node* node)
{
	SpriteShape* sprite = dynamic_cast<SpriteShape*>(node);
	blocks.erase(Point(sprite->getRow(), sprite->getCol()));
	sprite->removeFromParent();
	if (RemoveBlocks.size() == 0)
	{
		isFillSprite = true;
	}
}

void GameScene::onMouseDown(Event* event)
{
	EventMouse* e = (EventMouse*)event;
	Point location = e->getLocationInView();

	if (SpriteShape* Sprite = findSprite(location))
	{
		int row = Sprite->getRow();
		int col = Sprite->getCol();

		floodFill(Point(row, col));
		if (RemoveBlocks.size() < 3)
		{
			RemoveBlocks.clear();
			return;
		}
		for (Point point : RemoveBlocks)
		{
			if (SpriteShape* sprite = blocks[point])
			{
				explodeSprite(sprite);
			}
		}
		RemoveBlocks.clear();
	}
}


void GameScene::floodFill(Point point)
{
	std::queue<Point> queue;
	queue.push(point);
	SpriteShape* sprite = blocks[point];

	while (queue.size() > 0) {
		Point currentSquare = queue.front();
		queue.pop();
 
		float posX = currentSquare.x;
		float posY = currentSquare.y;
		if (blocks[Point(posX, posY)] == nullptr)
		{
			continue;
		}

		if (RemoveBlocks.find(Point(posX, posY)) != RemoveBlocks.end())
		{
			continue;
		}

		if (sprite->getImageIndex() != blocks[Point(posX, posY)]->getImageIndex())
		{
			continue;
		}

		if (posX + 1 < rows)
		{
			queue.push(Point(posX + 1, posY));
		}
		if (posX - 1 >= 0)
		{
			queue.push(Point(posX - 1, posY));
		}
		
		if (posY + 1 < cols)
		{
			queue.push(Point(posX, posY + 1));
		}

		if (posY - 1 >= 0)
		{
			queue.push(Point(posX, posY - 1));
		}
		RemoveBlocks.insert(Point(posX, posY));
	}
}

void GameScene::startCallback(Ref* pSender)
{
	for (BlockPair block : blocks)
	{
		block.second->removeFromParent();
	}
	blocks.clear();
	initMap();
}