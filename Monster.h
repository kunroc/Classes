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
	//����ͼƬ�������������Ѫ��
	void InitMonsterSprite(char *name);
	//��Ѫ���Ĺ��� 
	void InitMonsterSprite(char *name,char *xue_back,char* xue_fore);
	//���º��������Ӣ���ڿ��ӷ�Χ�ڣ����ϴ���
	void update(float delta);
	//��������,ÿ��3�����£�����Ӣ�������ľ���
	void updateMonster(float delta);
	//������������Ӣ��
	void StartListen(CCNode* m_hero, CCNode* m_map);
	//��ȡ����
	float getDistance();
	//X����
	float getDistanceX();
	//Y����
	float getDistanceY();
	//����Ѳ��·��
	void MonsterSeeRun();
	//�ڿ��ӷ�Χ�ڣ��������Ӣ���˶�
	void FollowRun();
	//����״̬����
	void doDied();
	//�ж��Ƿ񹥻�
	void JudegeAttack();
	//����״̬
	EnumState getState();
	void setState(EnumState state);
	//�������ء����ŵ�
	bool loadAnimation(const char *name_plist, char *name_pre, int count, int tag);
	void playAnimation(int tag, int loop/* =1 */);
	void stopAnimation(int tag);
	std::map<int, Animation*> MonsterAnis;
	bool aniIsDoing;
	bool MonsterDirecton;
	CREATE_FUNC(Monster);
	Sprite* m_MonsterSprite;//���ﾫ��
	ProgressView*  Monster_xue;//����Ѫ��
private:
	char *Monster_name;//���������ʼ״̬�ľ���ͼƬ����
	Node* my_hero;//��ǰӢ��
	Node* my_map;//��ǰ��ͼ
	//��ǰ�����Ӣ�۵ľ���
	float x;
	float y;
	float distance;
	EnumState enCurState;
};
#endif // __HERO_H__

