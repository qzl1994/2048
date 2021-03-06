#include "Brick.h"

const int Brick::nums[12] = { 0, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048 };

bool Brick::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	level = 0;

	// 砖块背景
	background = Sprite::createWithSpriteFrameName("num_0.png");
	background->setPosition(Vec2::ZERO);
	this->addChild(background);

	// 数字
	label = Label::create(String::createWithFormat("%d", Brick::nums[level])->getCString(), "Arial", 20);
	label->setPosition(Vec2::ZERO);
	this->addChild(label, 1);

	return true;
}

void Brick::setLevel(int i)
{
	this->level = i;
	this->background->setDisplayFrame(SpriteFrameCache::getInstance()->spriteFrameByName(String::createWithFormat("num_%d.png", Brick::nums[level])->getCString()));
	this->label->setString(String::createWithFormat("%d", Brick::nums[level])->getCString());
}