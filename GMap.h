#pragma once
#include "cocos2d.h"
USING_NS_CC; 
class GMap : public CCNode
{
public:
	GMap();
	~GMap();
	//��ʼ����ͼ,window_sizeoΪ����̨��С
	void InitMap(const char *map_name,const Size &window_size);
	//���ݾ����λ���ƶ���ͼ��visibleSizeΪ��ǰ���ڵĴ�С
	void MoveMap(bool direction);
	//�жϵ�ͼ�Ƿ񵽴��Ե
	bool JudgeMap();
	//virtual void update(float delta);
	CREATE_FUNC(GMap);
	
	Sprite *m_map;//��ͼ����
private:
	Size visibleSize;
};
