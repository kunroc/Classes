#pragma once
#include "cocos2d.h"
#include "Joystick\Joystick.h"
#include <map>
USING_NS_CC;
enum HeroState{
	HeroStand,
	HeroRun,
	HeroAttack,
	HeroDied
};
class Hero : public Node
{
public:
	Hero(char *heroPath);
	~Hero();
	bool init();
	static Hero* create(char *heroPath);
	//启动监听
	bool loadAnimation(const char *name_plist,char *name_pre,int count,int tag);
	void playAnimation(int tag, int loop/* =1 */);
	void stopAnimation(int tag);
	
	void StartListen();
	void update(float delta);
	HeroState getState();
	void setState(HeroState state);
	bool judgePositon(Size visibleSize);

	std::map<int,Animation*> HeroAnis;
	Sprite* heroSprite;
	bool HeroDirecton;
private:
	//Texture2D* heroTex;
	bool aniIsDoing;
	char* heroPath;
	HeroState enCurState;
};

