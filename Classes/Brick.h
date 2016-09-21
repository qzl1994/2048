#ifndef __BRICK_H__
#define __BRICK_H__

#include "cocos2d.h"

USING_NS_CC;

class Brick : public Sprite
{
public:
	virtual bool init();

	int getLevel(){ return level; }
	void setLevel(int i);

	CREATE_FUNC(Brick);
private:
	int level;
	static const int nums[12];
	Sprite* background;
	Label* label;
};

#endif