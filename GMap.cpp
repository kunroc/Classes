#include "GMap.h"

GMap::GMap()
{

}
GMap::~GMap()
{

}
void GMap::InitMap(const char *map_name, const Size &window_size)
{
	this->m_map=Sprite::create(map_name);
	m_map->setAnchorPoint(ccp(0,0));//设置锚点
	m_map->setScale(480/m_map->getContentSize().height);
	this->setAnchorPoint(ccp(0,0));//设置锚点
	this->addChild(m_map);
}
void GMap::MoveMap(bool direction)//
{
	int length;
	if (!direction)
		length = 3;
	else
		length = -3;
	this->setPositionX(this->getPosition().x + length);
}
bool GMap::JudgeMap()
{
	visibleSize = Director::sharedDirector()->getVisibleSize();
	if (this->getPositionX() > 0)
	{
		this->setPositionX(0);
		return false;
	}
	else if (this->getPositionX() < visibleSize.width - m_map->getContentSize().width)
	{
		this->setPositionX(visibleSize.width - m_map->getContentSize().width);
		return false;
	}
	return true;
}

