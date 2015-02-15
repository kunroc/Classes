#pragma once
#include "cocos2d.h"
#include "GMap.h"
#include "Joystick\Joystick.h"
#include "Hero.h"
#include "Monster.h"
USING_NS_CC;
class BattleScene : public cocos2d::Scene
{
public:
	BattleScene();
	virtual ~BattleScene();
	CREATE_FUNC(BattleScene);
	virtual bool init();
	virtual void update(float delta);
	Layer* mylayer;
	GMap* mymap;
	Joystick* joystick;
	Hero* hero;
	Monster* monster;
};

