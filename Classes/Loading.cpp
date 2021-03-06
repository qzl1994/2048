#include "Loading.h"

Scene* Loading::createScene()
{
	auto scene = Scene::create();

	auto layer = Loading::create();

	scene->addChild(layer);

	return scene;
}

bool Loading::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	// 等待标签
	auto loading = Label::createWithSystemFont("Loading...", "Arial", 36);
	loading->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(loading);

	// 异步加载图片资源
	TextureCache::getInstance()->addImageAsync("res.png", CC_CALLBACK_1(Loading::loadComplete, this));
}

// 回调
void Loading::loadComplete(Texture2D* texture)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res.plist", texture);

	auto scene = GameLayer::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene));
}