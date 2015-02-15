#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Joystick\Joystick.h"
#include "Hero.h"
#include "GMap.h"
#include "ProgressView.h"
#include "Monster.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    CREATE_FUNC(HelloWorld);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
	virtual void update(float delta);
	void loadAttackBt();
	void doRunning();
	bool isRectCollision(Hero* sp1, Monster* sp2);

	Size visibleSize;
	Vec2 origin;
	GMap* mymap;
	ProgressView *m_pProgressView;
	Joystick* joystick;
	Hero* hero;
	Monster* monster;
	std::vector<Monster*> monsterList;
};

#endif // __HELLOWORLD_SCENE_H__
