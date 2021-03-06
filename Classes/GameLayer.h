#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "cocos2d.h"
#include "Brick.h"
#include "GameOver.h"

USING_NS_CC;
using namespace std;

class GameLayer : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();
	void gameRestart(Ref* pSender);
	void gameExit(Ref* pSender);
	void gameInit();

	int score;
	Label* scoreLabel;
	Label* highscoreLabel;
	Point touchDown;

	CREATE_FUNC(GameLayer);
private:
	Brick* tables[4][4];

	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	// 四个移动方向，返回是否有砖块移动过
	bool moveToTop();
	bool moveToDown();
	bool moveToLeft();
	bool moveToRight();

	void addBricks();
	bool isOver();
};

#endif