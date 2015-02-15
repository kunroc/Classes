#pragma once
#include "cocos2d.h"
USING_NS_CC; 
class GMap : public CCNode
{
public:
	GMap();
	~GMap();
	//初始化地图,window_sizeo为控制台大小
	void InitMap(const char *map_name,const Size &window_size);
	//根据精灵的位置移动地图，visibleSize为当前窗口的大小
	void MoveMap(bool direction);
	//判断地图是否到达边缘
	bool JudgeMap();
	//virtual void update(float delta);
	CREATE_FUNC(GMap);
	
	Sprite *m_map;//地图精灵
private:
	Size visibleSize;
};
