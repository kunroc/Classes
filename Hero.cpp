#include "Hero.h"
#include "2d\CCSpriteFrame.h"
#include "deprecated\CCDeprecated.h"
#include "renderer\CCTextureCache.h"

Hero* Hero::create(char *heroPath)
{
	Hero *pRet = new Hero(heroPath);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}
Hero::Hero(char *heroPath)
{
	this->heroPath = heroPath;
	this->heroSprite = Sprite::create(heroPath);
	this->addChild(heroSprite);
}

Hero::~Hero()
{
}

bool Hero::init()
{
	this->HeroDirecton = false;
	this->aniIsDoing = false;
	this->enCurState = HeroStand;
	return true;
}

bool Hero::loadAnimation(const char *name_plist, char *name_pre, int count, int tag)
{
	if (HeroAnis.find(tag) != HeroAnis.end())
		return true;
	SpriteFrameCache *m_frameCache = SpriteFrameCache::getInstance();
	m_frameCache->addSpriteFramesWithFile(name_plist);
	Vector<SpriteFrame*> sframes;
	for (auto i = 1; i <= count; i++)
	{
		SpriteFrame* frame = m_frameCache->getSpriteFrameByName(String::createWithFormat("%s%d.png",name_pre, i)->getCString());
		sframes.pushBack(frame);
	}
	Animation* ani = Animation::createWithSpriteFrames(sframes);
	ani->retain();
	//ani->setLoops(-1);//-1表示无限循环播放
	ani->setDelayPerUnit(0.1f);

	HeroAnis.insert(std::map<int, Animation*>::value_type(tag, ani));
	return true;
}

void Hero::playAnimation(int tag, int loop /* =1 */)
{
	if (this->aniIsDoing == true)
		return;
	//StopAnimation(-1);
	CCLOG("aniTag:%d",this->numberOfRunningActions());
	auto currentAni = this->HeroAnis.find(tag);
	if (currentAni == this->HeroAnis.end()) {
		return;
	}
	else {
		auto ani = currentAni->second;
		ani->setLoops(loop);
		Animate* action = Animate::create(ani);
		action->setTag(tag);
		//this->heroSprite->runAction(action);
		this->heroSprite->runAction(Sequence::create(action,
			CallFunc::create([&]()
			{
				this->aniIsDoing = false;
				this->enCurState = HeroStand;
			}), NULL));
		this->aniIsDoing = true;
	}
}

void Hero::stopAnimation(int tag)
{
	if (tag == -1)
	{
		heroSprite->stopAllActions();
	} 
	else
	{
		heroSprite->stopActionByTag(tag);
	}
	this->aniIsDoing = false;
	this->enCurState = HeroStand;
}


void Hero::StartListen()
{
	this->scheduleUpdate();
}

void Hero::update(float delta)
{
	switch (enCurState) {

	case HeroStand:
		//this->removeChild(heroSprite, TRUE);
		//heroSprite = Sprite::create(heroPath);//恢复精灵原来的贴图样子
		//heroSprite->setFlipX(HeroDirecton);
		//this->addChild(heroSprite);
		break;
	case HeroRun:
		this->playAnimation(1, 1);
		break;
	case HeroAttack:
		this->playAnimation(2, 1);
		break;
	case HeroDied:
		
		break;
	}
}


HeroState Hero::getState()
{
	return this->enCurState;
}

void Hero::setState(HeroState state)
{
	this->enCurState = state;
}

bool Hero::judgePositon(Size visibleSize)
{
	if (this->getPositionX() > visibleSize.width / 2 - 10 && this->getPositionX() < visibleSize.width / 2 + 10)
		return true;
	else
		return false;//到达中间位置  
}


