#ifndef __LOADING_H__
#define __LOADING_H__

#include "cocos2d.h"
#include "GameLayer.h"

USING_NS_CC;

class Loading : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();
	void loadComplete(Texture2D* texture);

	CREATE_FUNC(Loading);
};

#endif