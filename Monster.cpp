#include "Monster.h"
USING_NS_CC; 
Monster::Monster(void)
{
	MonsterDirecton = true;
	aniIsDoing = false;
	Monster_name=NULL;
	Monster_xue=NULL;
	enCurState = enStateStand;
}

Monster::~ Monster(void)
{

}

void  Monster::InitMonsterSprite(char *name)
{
	Monster_name=name;
	m_MonsterSprite=CCSprite::create(name);
	m_MonsterSprite->setFlipX(MonsterDirecton);
	this->addChild(m_MonsterSprite);
}
void Monster::InitMonsterSprite(char *name,char *xue_back,char* xue_fore)
{
	InitMonsterSprite(name);
	//���ù����Ѫ�� 
	Monster_xue = new ProgressView();  
	Monster_xue->setPosition(ccp(m_MonsterSprite->getPositionX()+25, m_MonsterSprite->getPositionY()+60));//�����ڹ�����ͷ  
	//Monster_xue->setScale(2.2f);  
	Monster_xue->setBackgroundTexture(xue_back);  
	Monster_xue->setForegroundTexture(xue_fore);  
	Monster_xue->setTotalProgress(300.0f);  
	Monster_xue->setCurrentProgress(300.0f); 
	this->addChild(Monster_xue);
}

bool Monster::loadAnimation(const char *name_plist, char *name_pre, int count, int tag)
{
	if (MonsterAnis.find(tag) != MonsterAnis.end())
		return true;
	SpriteFrameCache *m_frameCache = SpriteFrameCache::getInstance();
	m_frameCache->addSpriteFramesWithFile(name_plist);
	Vector<SpriteFrame*> sframes;
	for (auto i = 1; i <= count; i++)
	{
		SpriteFrame* frame = m_frameCache->getSpriteFrameByName(String::createWithFormat("%s%d.png", name_pre, i)->getCString());
		sframes.pushBack(frame);
	}
	Animation* ani = Animation::createWithSpriteFrames(sframes);
	ani->retain();
	//ani->setLoops(-1);//-1��ʾ����ѭ������
	ani->setDelayPerUnit(0.1f);

	MonsterAnis.insert(std::map<int, Animation*>::value_type(tag, ani));
	return true;
}

void Monster::playAnimation(int tag, int loop /* =1 */)
{
	if (this->aniIsDoing == true)
		return;
	auto currentAni = this->MonsterAnis.find(tag);
	if (currentAni == this->MonsterAnis.end()) {
		return;
	}
	else {
		auto ani = currentAni->second;
		ani->setLoops(loop);
		Animate* action = Animate::create(ani);
		action->setTag(tag);
		this->m_MonsterSprite->runAction(Sequence::create(action,
		CallFunc::create([&]()
		{
			this->aniIsDoing = false;
			enCurState = enStateStand;
		}), NULL));
		this->aniIsDoing = true;
	}
}

void Monster::stopAnimation(int tag)
{
	if (tag == -1)
		m_MonsterSprite->stopAllActions();
	else
	{
		m_MonsterSprite->stopActionByTag(tag);
		this->aniIsDoing = false;
	}
}

void Monster::StartListen(Node* m_hero, Node* m_map)
{
	my_hero = m_hero;
	my_map = m_map;	
	getDistance();
	this->schedule(schedule_selector(Monster::updateMonster), 1.0f);//ÿ��3��������  
	this->scheduleUpdate();//Ӣ��һ��������ӷ�Χ������׷��Ӣ�۴�  
}

void Monster::updateMonster(float delta)
{
	getDistance();
}

void Monster::update(float delta)
{
	if (distance > 100)
		MonsterSeeRun();
	else if (distance <= 100 )//��Ӣ�������Ŀ��ӷ�Χ�ڣ�����׷��Ӣ��  
		FollowRun();

	if (this->Monster_xue->getCurrentProgress() <= 0)
		this->setState(enStateDied);

	switch (enCurState) {

	case enStateStand:
		//this->removeChild(m_MonsterSprite, TRUE);
		//m_MonsterSprite = Sprite::create(Monster_name);//�ָ�����ԭ������ͼ����
		//m_MonsterSprite->setFlipX(MonsterDirecton);
		//this->addChild(m_MonsterSprite);
		break;
	case enStateRun:
		this->playAnimation(1, 1);
		break;
	case enStateAttack:
		this->playAnimation(2, 1);
		break;
	case enStateDied:
		this->doDied();
		break;
	}
}

void Monster::MonsterSeeRun()
{
	Size visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	//����ľ�������
	auto  absX = this->getPositionX() + my_map->getPositionX();
	auto absY = this->getPositionY() + my_map->getPositionY();
	if (absX <= 8){
		MonsterDirecton  =  false;
		m_MonsterSprite->setFlipX(MonsterDirecton);
	}
	else if (absX >= visibleSize.width - 8){
		MonsterDirecton = true;
		m_MonsterSprite->setFlipX(MonsterDirecton);
	}
	//���Ź������߶���
	this->setState(enStateRun);
	MoveBy *moveAction = nullptr;
	if (MonsterDirecton == true)
		moveAction = CCMoveBy::create(1, ccp(-2, 0));
	else
		moveAction = CCMoveBy::create(1, ccp(2, 0));
	//�����ص�������Ѳ��·�����  
	this->runAction(moveAction);
}

void Monster::FollowRun()
{
	getDistance();
	if (distance <= 50 && y == 0)
	{
		JudegeAttack();
		return;
	}
	this->setState(enStateRun);

	if (x >= 0)
	{
		MonsterDirecton = false;
		m_MonsterSprite->setFlipX(MonsterDirecton); 
		this->setPositionX(this->getPositionX() + 1);
	}
	else
	{
		MonsterDirecton = true;
		m_MonsterSprite->setFlipX(MonsterDirecton); 
		this->setPositionX(this->getPositionX() - 1);
	}

	if (y >= 0)
		this->setPositionY(this->getPositionY() + 1);
	else
		this->setPositionY(this->getPositionY() - 1);	
}

void Monster::JudegeAttack()
{
	this->setState(enStateAttack);
}

void Monster::doDied()
{
	this->unscheduleAllSelectors();
	//this->removeChild(m_MonsterSprite, TRUE);
	//m_MonsterSprite = Sprite::create(Monster_name);//�ָ�����ԭ������ͼ����
	//m_MonsterSprite->setRotation(90);
	//this->addChild(m_MonsterSprite);
}

EnumState Monster::getState()
{
	return this->enCurState;
}

void Monster::setState(EnumState state)
{
	this->enCurState = state;
}

float Monster::getDistance()
{
	//�õ�����x�ľ���,�ǵù��������Ҫ���ϵ�ͼ��  
	x = my_hero->getPositionX() - (this->getPositionX() + my_map->getPositionX());
	//�õ�����y�ľ��룬�ǵù��������Ҫ���ϵ�ͼ��  
	y = my_hero->getPositionY() - (this->getPositionY() + my_map->getPositionY());
	//�ȼ�������Ӣ�۵ľ���  
	distance = sqrt(pow(x, 2) + pow(y, 2));
	return distance;
}

float Monster::getDistanceX()
{
	getDistance();
	return sqrt(pow(x, 2));
}

float Monster::getDistanceY()
{
	getDistance();
	return sqrt(pow(y, 2));
}


