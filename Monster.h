#ifndef __Monster_H__
#define __Monster_H__
#include "cocos2d.h"
#include "ProgressView.h"
USING_NS_CC;

enum EnumState{
	enStateStand,       
	enStateRun,    
	enStateAttack,   
	enStateDied
};
class Monster:public cocos2d::CCNode
{
public:
	Monster(void);
	~Monster(void);
	//根据图片名创建怪物，不带血条
	void InitMonsterSprite(char *name);
	//带血条的怪物 
	void InitMonsterSprite(char *name,char *xue_back,char* xue_fore);
	//更新函数，如果英雄在可视范围内，不断触发
	void update(float delta);
	//监听函数,每隔3秒检测下，计算英雄与怪物的距离
	void updateMonster(float delta);
	//怪物启动监听英雄
	void StartListen(CCNode* m_hero, CCNode* m_map);
	//获取距离
	float getDistance();
	//X距离
	float getDistanceX();
	//Y距离
	float getDistanceY();
	//怪物巡逻路线
	void MonsterSeeRun();
	//在可视范围内，怪物跟随英雄运动
	void FollowRun();
	//死亡状态处理
	void doDied();
	//判断是否攻击
	void JudegeAttack();
	//怪物状态
	EnumState getState();
	void setState(EnumState state);
	//动画加载、播放等
	bool loadAnimation(const char *name_plist, char *name_pre, int count, int tag);
	void playAnimation(int tag, int loop/* =1 */);
	void stopAnimation(int tag);
	std::map<int, Animation*> MonsterAnis;
	bool aniIsDoing;
	bool MonsterDirecton;
	CREATE_FUNC(Monster);
	Sprite* m_MonsterSprite;//怪物精灵
	ProgressView*  Monster_xue;//怪物血条
private:
	char *Monster_name;//用来保存初始状态的精灵图片名称
	Node* my_hero;//当前英雄
	Node* my_map;//当前地图
	//当前怪物和英雄的距离
	float x;
	float y;
	float distance;
	EnumState enCurState;
};
#endif // __HERO_H__

