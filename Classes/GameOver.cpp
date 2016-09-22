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

	// ������ǩ
	auto over = Label::create("GAME OVER", "Arial", 50);
	over->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 30));
	this->addChild(over);

	// ���¿�ʼ��Ϸ�˵���
	auto restart = Label::create("Restart", "Arial", 30);
	auto restartMenuItem = MenuItemLabel::create(restart, CC_CALLBACK_1(GameOver::restartMenu, this));
	restartMenuItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 10));

	auto menu = Menu::create(restartMenuItem, NULL);
	addChild(menu);
	return true;
}

// ���¿�ʼ��Ϸ�ص�
void GameOver::restartMenu(Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameLayer::createScene()));
}