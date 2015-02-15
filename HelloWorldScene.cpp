#include "HelloWorldScene.h"
#include "UIButton.h"
#include "UIWidget.h"
#include "Hero.h"
#include "BattleScene.h"
USING_NS_CC;
using namespace ui;
Scene* HelloWorld::createScene()
{
	auto scene = Scene::create();
	auto layer = HelloWorld::create();
	scene->addChild(layer);
	return scene;
}

bool HelloWorld::init()
{
	if (!Layer::init())
		return false;
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	//地图
	mymap = GMap::create();
	//mymap->setPosition(ccp(-1,0));
	mymap->InitMap("background/gp4.png", visibleSize);
	this->addChild(mymap, 0);
	//设置英雄的血条 
	m_pProgressView = new ProgressView();
	m_pProgressView->setPosition(ccp(150, 450));
	m_pProgressView->setScale(2.2f);
	m_pProgressView->setBackgroundTexture("xue_back.png");
	m_pProgressView->setForegroundTexture("xue_fore.png");
	m_pProgressView->setTotalProgress(100.0f);
	m_pProgressView->setCurrentProgress(100.0f);
	Sprite *xuekuang = Sprite::create("kuang.png");//添加血条的框架
	xuekuang->setPosition(ccp(m_pProgressView->getPositionX(), m_pProgressView->getPositionY()));
	Sprite *touxiang = Sprite::create("touxiang.png");//添加英雄的左上角的小头像
	touxiang->setPosition(ccp(m_pProgressView->getPositionX() - 120, m_pProgressView->getPositionY()));
	this->addChild(touxiang, 2);
	this->addChild(xuekuang, 2);
	this->addChild(m_pProgressView, 2);
	//退出按钮
	Button* closeBt = Button::create();
	closeBt->loadTextures("test/skip.png", "test/skip_down.png", "");
	closeBt->setPosition(Vec2(visibleSize.width - 30, visibleSize.height - 30));
	closeBt->addTouchEventListener([=](CCObject* psender, Widget::TouchEventType type){
		Director::getInstance()->pushScene(TransitionFade::create(2, BattleScene::create()));
		//Director::getInstance()->resume();
	});
	this->addChild(closeBt, 2);
	//虚拟摇杆
	this->joystick = Joystick::CreateJoystick("joy/joystick_1.png", "joy/joy_bt.png", ccp(60, 60));
	this->addChild(joystick, 2);
	joystick->joy_isWork(true);
	//攻击按钮
	loadAttackBt();
	//加载英雄
	this->hero = Hero::create("hero/hero.png");
	hero->setPosition(ccp(100, 200));
	hero->loadAnimation("hero/hero_run.plist", "hero_run", 8, 1);
	hero->loadAnimation("hero/hero_attack.plist", "hero_attack", 20, 2);
	hero->StartListen();
	this->addChild(hero, 1);
	//加载批量怪物
	for (int i = 0; i < 3; i++)
	{
		auto _monster = Monster::create();
		_monster->InitMonsterSprite("monster/monster.png", "xue_back.png", "xue_fore.png");
		_monster->setPosition(ccp(visibleSize.width - 50, visibleSize.height / 2 - 50 * i));
		_monster->loadAnimation("monster/monster_run.plist", "monster_run", 6, 1);
		_monster->loadAnimation("monster/monster_attack.plist", "monster_attack", 5, 2);
		_monster->StartListen(hero, mymap);
		monsterList.push_back(_monster);
		mymap->addChild(_monster);
	}
	//定时器
	this->scheduleUpdate();
	return true;
}


void HelloWorld::loadAttackBt()
{
	Button* attackBt1 = Button::create();
	attackBt1->loadTextures("joy/jinengban4.png", "joy/jinengban4.png", "");
	attackBt1->setPosition(ccp(visibleSize.width - 40, 40));
	attackBt1->setScale(80 / attackBt1->getContentSize().height);
	attackBt1->addTouchEventListener([=](CCObject* psender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED)
			this->hero->setState(HeroState::HeroAttack);
	});
	this->addChild(attackBt1, 2);

	Button* attackBt2 = Button::create();
	attackBt2->loadTextures("joy/Direction_bt.png", "joy/Direction_bt.png", "");
	attackBt2->setPosition(ccp(visibleSize.width - 40 - 50, 40 + 50));
	attackBt2->setScale(40 / attackBt2->getContentSize().height);
	attackBt2->addTouchEventListener([=](CCObject* psender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED)
			this->hero->setState(HeroState::HeroAttack);
	});
	this->addChild(attackBt2, 2);

}


void HelloWorld::update(float delta)
{
	if (this->m_pProgressView->getCurrentProgress() <= 0){
		this->hero->setState(HeroState::HeroDied);
		//Director::getInstance()->replaceScene(TransitionFade::create(2, BattleScene::create()));
	}
	this->doRunning();
	for (auto _monster : monsterList){
		this->isRectCollision(hero, _monster);
	}
}
//英雄行走控制
void HelloWorld::doRunning()
{
	Size visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	switch (this->joystick->joyDirection)
	{
	case 1:
		if (this->hero->HeroDirecton != this->joystick->runDirection)
			hero->HeroDirecton = joystick->runDirection;
		hero->heroSprite->setFlipX(hero->HeroDirecton);
		hero->setState(HeroState::HeroRun);
		if (hero->getPositionX() > visibleSize.width)//不让精灵超出右边 
			break;
		if (hero->judgePositon(visibleSize) && mymap->JudgeMap())
			mymap->MoveMap(true);
		else
			hero->setPosition(ccp(hero->getPosition().x + 3, hero->getPosition().y)); //向右走  
		break;
	case  2:
		if (hero->getPositionY() <= visibleSize.height - 100)
			hero->setPosition(ccp(hero->getPosition().x, hero->getPosition().y + 1));   //向上走
		this->hero->setState(HeroState::HeroRun);
		break;
	case 3:
		if (this->hero->HeroDirecton != this->joystick->runDirection)
			hero->HeroDirecton = joystick->runDirection;
		hero->heroSprite->setFlipX(hero->HeroDirecton);
		hero->setState(HeroState::HeroRun);
		if (hero->getPositionX() < 0)//不让精灵超出左边 
			break;
		if (hero->judgePositon(visibleSize) && mymap->JudgeMap())//精灵在中间，地图不在边缘
			mymap->MoveMap(false);
		else
			hero->setPosition(ccp(hero->getPosition().x - 3, hero->getPosition().y));
		break;
	case 4:
		if (hero->getPositionY() >= 100)
			hero->setPosition(ccp(hero->getPosition().x, hero->getPosition().y - 1));   //向下走
		this->hero->setState(HeroState::HeroRun);
		break;
	default:
		//hero->stopAnimation(1);
		break;
	}
}

bool HelloWorld::isRectCollision(Hero* sp1, Monster* sp2)
{
	if (sp2->getDistanceY() <= 20 && sp1->heroSprite->getBoundingBox().intersectsRect(sp2->m_MonsterSprite->getBoundingBox()))
	{
		auto heroState = sp1->getState();
		auto monsterState = sp2->getState();
		if (heroState == HeroState::HeroDied || monsterState == EnumState::enStateDied)
			return true;
		if (heroState == HeroState::HeroAttack)
			sp2->Monster_xue->setCurrentProgress(sp2->Monster_xue->getCurrentProgress() - 2);
		if (monsterState == EnumState::enStateAttack)
			m_pProgressView->setCurrentProgress(m_pProgressView->getCurrentProgress() - 0.1);
	}
	return true;
}
