#include "GameLayer.h"

Scene* GameLayer::createScene()
{
	auto scene = Scene::create();

	auto layer = GameLayer::create();

	scene->addChild(layer);

	return scene;
}

bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	// 初始化得分
	score = 0;

	// 背景
	Sprite* bg = Sprite::createWithSpriteFrameName("bg.png");
	bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bg);

	// 重新开始菜单项
	Sprite* restart_norm = Sprite::createWithSpriteFrameName("restart_norm.png");
	Sprite* restart_press = Sprite::createWithSpriteFrameName("restart_press.png");
	MenuItemSprite* restart = MenuItemSprite::create(restart_norm, restart_press, CC_CALLBACK_1(GameLayer::gameRestart, this));
	restart->setPosition(170, 380);

	// 退出游戏菜单项
	Sprite* exit_norm = Sprite::createWithSpriteFrameName("exit_norm.png");
	Sprite* exit_press = Sprite::createWithSpriteFrameName("exit_press.png");
	MenuItemSprite* exit = MenuItemSprite::create(exit_norm, exit_press, CC_CALLBACK_1(GameLayer::gameExit, this));
	exit->setPosition(265, 380);

	Menu* menu = Menu::create(restart, exit, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	// 初始化游戏界面
	this->gameInit();

	return true;
}

// 重新开始
void GameLayer::gameRestart(Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameLayer::createScene()));
}

// 退出游戏
void GameLayer::gameExit(Ref* pSender)
{
	Director::getInstance()->end();
	exit(0);
}

// 初始化游戏界面
void GameLayer::gameInit()
{
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{

		}
	}
}