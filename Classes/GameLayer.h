#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class GameLayer : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();
	void gameRestart(Ref* pSender);
	void gameExit(Ref* pSender);
	void gameInit();

	CREATE_FUNC(GameLayer);
private:
	int score;


};

#endif