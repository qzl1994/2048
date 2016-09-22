#include "GameOver.h"

Scene* GameOver::createScene()
{
	auto scene = Scene::create();

	auto layer = GameOver::create();

	scene->addChild(layer);

	return scene;
}

bool GameOver::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	// 结束标签
	auto over = Label::create("GAME OVER", "Arial", 50);
	over->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 30));
	this->addChild(over);

	// 重新开始游戏菜单项
	auto restart = Label::create("Restart", "Arial", 30);
	auto restartMenuItem = MenuItemLabel::create(restart, CC_CALLBACK_1(GameOver::restartMenu, this));
	restartMenuItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 10));

	auto menu = Menu::create(restartMenuItem, NULL);
	addChild(menu);
	return true;
}

// 重新开始游戏回调
void GameOver::restartMenu(Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameLayer::createScene()));
}