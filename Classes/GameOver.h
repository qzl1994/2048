#ifndef __GAMEOVER_H__
#define __GAMEOVER_H__

#include "cocos2d.h"
#include "GameLayer.h"

USING_NS_CC;

class GameOver : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();

	void restartMenu(Ref* pSender);

	CREATE_FUNC(GameOver);
};

#endif