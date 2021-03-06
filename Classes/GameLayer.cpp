#include "GameLayer.h"

#define RC_CONVERT_TO_XY(rc) (rc*70+55)

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

	// 分数
	scoreLabel = Label::createWithSystemFont("", "arial", 20);
	scoreLabel->setString(String::createWithFormat("%d", 0)->getCString());
	scoreLabel->setPosition(170, 420);
	this->addChild(scoreLabel);

	// 最高分
	highscoreLabel = Label::createWithSystemFont("", "arial", 20);
	int high = UserDefault::getInstance()->getIntegerForKey("HighScore");
	highscoreLabel->setString(String::createWithFormat("%d", high)->getCString());
	highscoreLabel->setPosition(265, 420);
	this->addChild(highscoreLabel);

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

	// 加入监听器
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

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
	// 初始化砖块
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			auto brick = Brick::create();
			brick->setPosition(Vec2(RC_CONVERT_TO_XY(y), RC_CONVERT_TO_XY(x) + 45));
			brick->setVisible(false);
			this->addChild(brick, 1);
			tables[x][y] = brick;
		}
	}

	// 获取两个随机坐标
	// c++11的随机数产生方式
	default_random_engine e(time(NULL));
	// 设定产生的随机数的范围，这里是0到3
	uniform_int_distribution<unsigned> u(0, 3);
	int row1 = u(e);
	int col1 = u(e);
	int row2 = u(e);
	int col2 = u(e);
	// 这个循环是保证两个砖块的坐标不会重复
	do{
		row2 = u(e);
		col2 = u(e);
	} while (row1 == row2&&col1 == col2);

	// 添加第一个砖块
	auto brick1 = tables[row1][col1];
	int isFour = e() % 10;
	if (isFour == 0){
		brick1->setLevel(2);
		brick1->setVisible(true);
	}
	else{
		brick1->setLevel(1);
		brick1->setVisible(true);
	}

	// 添加第二个砖块
	auto brick2 = tables[row2][col2];
	isFour = e() % 10;
	if (isFour == 0){
		brick2->setLevel(2);
		brick2->setVisible(true);
	}
	else{
		brick2->setLevel(1);
		brick2->setVisible(true);
	}
}

bool GameLayer::onTouchBegan(Touch* touch, Event* event)
{
	this->touchDown = touch->getLocationInView();
	this->touchDown = Director::getInstance()->convertToGL(this->touchDown);
	return true;
}

void GameLayer::onTouchMoved(Touch* touch, Event* event)
{

}

void GameLayer::onTouchEnded(Touch* touch, Event* event)
{
	bool hasMoved = false;

	Point touchUp = touch->getLocationInView();
	touchUp = Director::getInstance()->convertToGL(touchUp);

	if (touchUp.getDistance(touchDown) > 50)
	{
		// 判断上下还是左右
		if (abs(touchUp.x - touchDown.x) > abs(touchUp.y - touchDown.y))
		{
			// 左右滑动
			if (touchUp.x - touchDown.x > 0)
			{
				// 向右
				hasMoved = moveToRight();
			}
			else
			{
				// 向左
				hasMoved = moveToLeft();
			}
		}
		else
		{
			// 上下滑动
			if (touchUp.y - touchDown.y > 0)
			{
				// 向上
				hasMoved = moveToTop();
			}
			else
			{
				// 向下
				hasMoved = moveToDown();
			}
		}
	}

	if (hasMoved)
	{
		this->addBricks();
	}

	if (isOver())
	{
		// 存放分数
		int high = UserDefault::getInstance()->getIntegerForKey("HighScore");
		if (GameLayer::score>high)
		{
			UserDefault::getInstance()->setIntegerForKey("HighScore", GameLayer::score);
			UserDefault::getInstance()->flush();
		}
		GameLayer::score = 0;
		// 游戏结束
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameOver::createScene()));
	}
}

// 向上
bool GameLayer::moveToTop()
{
	bool hasMoved = false;

	// 将数字相同的格子合一
	for (int col = 0; col<4; col++)
	{
		for (int row = 3; row >= 0; row--)
		{
			// 遍历的每一次获得的方块
			auto brick = tables[row][col];
			// 找到不为空的方块
			if (brick->getLevel() != 0)
			{
				int k = row - 1;
				// 看这一列有没有等级和这个方块等级相同的
				while (k >= 0)
				{
					auto nextBrick = tables[k][col];
					if (nextBrick->getLevel() != 0)
					{
						if (brick->getLevel() == nextBrick->getLevel())
						{
							// 找到等级和这个砖块等级相同的就把他们合并
							brick->setLevel(nextBrick->getLevel() + 1);
							nextBrick->setLevel(0);
							nextBrick->setVisible(false);
							
							GameLayer::score += Brick::nums[brick->getLevel()];
							this->scoreLabel->setString(String::createWithFormat("%d", GameLayer::score)->getCString());
							
							hasMoved = true;
						}
						k = -1;
					}
					k--;
				}
			}
		}
	}

	// 将有数的格子填入空格子
	for (int col = 0; col<4; col++)
	{
		for (int row = 3; row >= 0; row--)
		{
			auto brick = tables[row][col];
			if (brick->getLevel() == 0)
			{
				int k = row - 1;
				while (k >= 0)
				{
					auto nextBrick = tables[k][col];
					if (nextBrick->getLevel() != 0)
					{
						brick->setLevel(nextBrick->getLevel());
						nextBrick->setLevel(0);
						brick->setVisible(true);
						nextBrick->setVisible(false);
						hasMoved = true;
						k = -1;
					}
					k--;
				}
			}
		}
	}
	return hasMoved;
}

// 向下
bool GameLayer::moveToDown()
{
	bool hasMoved = false;

	for (int col = 0; col<4; col++)
	{
		for (int row = 0; row<4; row++)
		{		
			auto brick = tables[row][col];	
			if (brick->getLevel() != 0)
			{
				int k = row + 1;
				while (k<4)
				{
					auto nextBrick = tables[k][col];
					if (nextBrick->getLevel() != 0)
					{
						if (brick->getLevel() == nextBrick->getLevel())
						{
							brick->setLevel(nextBrick->getLevel() + 1);
							nextBrick->setLevel(0);
							nextBrick->setVisible(false);

							GameLayer::score += Brick::nums[brick->getLevel()];
							this->scoreLabel->setString(String::createWithFormat("%d", GameLayer::score)->getCString());

							hasMoved = true;
						}
						k = 4;
					}
					k++;
				}
			}
		}
	}

	// 将有数的格子填入空格子
	for (int col = 0; col<4; col++)
	{
		for (int row = 0; row<4; row++)
		{
			// 遍历每一次的砖块
			auto brick = tables[row][col];
			// 找到空格子
			if (brick->getLevel() == 0)
			{
				int k = row + 1;
				while (k<4)
				{
					auto nextBrick = tables[k][col];
					if (nextBrick->getLevel() != 0)
					{
						// 将不为空的格子移到这里
						brick->setLevel(nextBrick->getLevel());
						nextBrick->setLevel(0);
						brick->setVisible(true);
						nextBrick->setVisible(false);
						hasMoved = true;
						k = 4;
					}
					k++;
				}
			}
		}
	}
	return hasMoved;
}

// 向左
bool GameLayer::moveToLeft()
{
	bool hasMoved = false;
	for (int col = 0; col<4; col++)
	{
		for (int row = 0; row<4; row++)
		{
			auto brick = tables[row][col];
			if (brick->getLevel() != 0)
			{
				int k = col + 1;
				while (k<4)
				{
					auto nextBrick = tables[row][k];
					if (nextBrick->getLevel() != 0)
					{
						if (brick->getLevel() == nextBrick->getLevel())
						{
							brick->setLevel(nextBrick->getLevel() + 1);
							nextBrick->setLevel(0);
							nextBrick->setVisible(false);

							GameLayer::score += Brick::nums[brick->getLevel()];
							this->scoreLabel->setString(String::createWithFormat("%d", GameLayer::score)->getCString());

							hasMoved = true;
						}
						k = 4;
					}
					k++;
				}
			}
		}
	}

	for (int row = 0; row<4; row++)
	{
		for (int col = 0; col<4; col++)
		{
			auto brick = tables[row][col];
			if (brick->getLevel() == 0)
			{
				int k = col + 1;
				while (k<4)
				{
					auto nextBrick = tables[row][k];
					if (nextBrick->getLevel() != 0)
					{
						brick->setLevel(nextBrick->getLevel());
						nextBrick->setLevel(0);
						brick->setVisible(true);
						nextBrick->setVisible(false);
						hasMoved = true;
						k = 4;
					}
					k++;
				}
			}
		}
	}

	return hasMoved;
}

// 向右
bool GameLayer::moveToRight()
{
	bool hasMoved = false;
	for (int row = 0; row<4; row++)
	{
		for (int col = 3; col >= 0; col--)
		{
			auto brick = tables[row][col];
			if (brick->getLevel() != 0)
			{
				int k = col - 1;
				while (k >= 0)
				{
					auto nextBrick = tables[row][k];
					if (nextBrick->getLevel() != 0)
					{
						if (brick->getLevel() == nextBrick->getLevel())
						{
							brick->setLevel(nextBrick->getLevel() + 1);
							nextBrick->setLevel(0);
							nextBrick->setVisible(false);

							GameLayer::score += Brick::nums[brick->getLevel()];
							this->scoreLabel->setString(String::createWithFormat("%d", GameLayer::score)->getCString());

							hasMoved = true;
						}
						k = -1;
					}
					k--;
				}
			}
		}
	}

	for (int row = 0; row<4; row++)
	{
		for (int col = 3; col >= 0; col--)
		{
			auto brick = tables[row][col];
			if (brick->getLevel() == 0)
			{
				int k = col - 1;
				while (k >= 0)
				{
					auto nextBrick = tables[row][k];
					if (nextBrick->getLevel() != 0)
					{
						brick->setLevel(nextBrick->getLevel());
						nextBrick->setLevel(0);
						brick->setVisible(true);
						nextBrick->setVisible(false);
						hasMoved = true;
						k = -1;
					}
					k--;
				}
			}
		}
	}

	return hasMoved;
}

// 添加砖块
void GameLayer::addBricks()
{
	// 获取两个随机坐标
	default_random_engine e(time(NULL));
	uniform_int_distribution<unsigned> u(0, 3);
	int row = 0;
	int col = 0;
	do{
		row = u(e);
		col = u(e);
	} while (tables[row][col]->getLevel() != 0);

	// 添加砖块
	auto brick = tables[row][col];
	int isFour = e() % 10;
	if (isFour == 0)
	{
		brick->setLevel(2);
		brick->setVisible(true);
	}
	else
	{
		brick->setLevel(1);
		brick->setVisible(true);
	}

	brick->setScale(0.5, 0.5);
	brick->runAction(ScaleTo::create(0.1f, 1.0f));
}

// 判断是否死亡
bool GameLayer::isOver(){
	for (int row = 0; row<4; row++){
		for (int col = 0; col<4; col++){
			// 判断是否存在空格子
			if (tables[row][col]->getLevel() == 0){
				// 有空格子肯定不会OVER
				return false;
			}
			// 判断周围格子,如果存在相等的数字则不OVER
			// 上
			int c = col;
			int r = row + 1;
			if (r != -1 && r != 4){
				if (tables[row][col]->getLevel() == tables[r][c]->getLevel()){
					return false;
				}
			}
			// 左
			c = col - 1;
			r = row;
			if (c != -1 && c != 4){
				if (tables[row][col]->getLevel() == tables[r][c]->getLevel()){
					return false;
				}
			}
			// 右
			c = col + 1;
			r = row;
			if (c != -1 && c != 4){
				if (tables[row][col]->getLevel() == tables[r][c]->getLevel()){
					return false;
				}
			}
			// 下
			c = col;
			r = row - 1;
			if (r != -1 && r != 4){
				if (tables[row][col]->getLevel() == tables[r][c]->getLevel()){
					return false;
				}
			}
		}
	}
	return true;
}