#include "Joystick/Joystick.h"
#include "2d/CCSprite.h"
#include "base/CCEventListenerTouch.h"
const double PI = 3.1415;
Joystick* Joystick::CreateJoystick(const char* bgImage, const char* Image, Point position)
{
	auto *layer = Joystick::create();
	if (layer)
	{
		layer->initView(bgImage, Image, position);
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return nullptr;
}

Joystick::Joystick(){
	this->runDirection = false;
	this->joyDirection = joy_stay;
}
Joystick::~Joystick(){

}

bool Joystick::initView(const char* bgImage, const char* Image, Point position)
{
	auto joyBg = Sprite::create(bgImage);
	joyBg->setPosition(position);
	joyBg->setScale(120 / joyBg->getContentSize().height);
	joyBg->setVisible(false);
	this->addChild(joyBg, 0, tag_joyBg);

	auto joyBt = Sprite::create(Image);
	joyBt->setPosition(position);
	joyBt->setScale(38/joyBt->getContentSize().height);
	joyBt->setVisible(false);
	this->addChild(joyBt, 0, tag_joyBt);

	joyDirection = joy_stay;
	return true;
}

bool Joystick::joy_isWork(bool iswork)
{
	auto joyBg = (Sprite*)this->getChildByTag(tag_joyBg);
	joyBg->setVisible(iswork);

	auto joyBt = (Sprite *)this->getChildByTag(tag_joyBt);
	joyBt->setVisible(iswork);

	if (iswork)
	{
		auto joyListen = EventListenerTouchOneByOne::create();
		joyListen->onTouchBegan = CC_CALLBACK_2(Joystick::onTouchBegan, this);
		joyListen->onTouchMoved = CC_CALLBACK_2(Joystick::onTouchMoved, this);
		joyListen->onTouchEnded = CC_CALLBACK_2(Joystick::onTouchEnded, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(joyListen, this);//添加触摸委托
	} 
	else
	{
		CCDirector::sharedDirector()->getEventDispatcher()->removeEventListenersForTarget(this);
	}
	return true;
}

bool Joystick::onTouchBegan(Touch* t, Event* ev)
{
	auto point = t->getLocation();
	auto joyBg = (Sprite *)this->getChildByTag(tag_joyBg);
	auto joyBt = (CCSprite*)this->getChildByTag(tag_joyBt);
	float angle = getAngle(joyBg->getPosition(), point);
	if (joyBg->boundingBox().containsPoint(point)){
		isCanMove = true;
		joyBt->setPosition(point);
		getDirection(angle);
		CCLOG("touch began");
	}	
	return true;
}

void Joystick::onTouchMoved(Touch* t, Event* ev)
{
	if (!isCanMove)
	{
		return;
	}
	Point point = t->getLocation();
	auto joyBg = (CCSprite*)this->getChildByTag(tag_joyBg);
	auto joyBt = (CCSprite*)this->getChildByTag(tag_joyBt);
	float joyBgR = joyBg->getContentSize().width*0.5;
	float angle = getAngle(joyBg->getPosition(),point);
	//判断两个圆的圆心距是否大于摇杆背景的半径
	if (sqrt(pow((joyBg->getPosition().x - point.x), 2) + pow((joyBg->getPosition().y - point.y), 2)) >= joyBgR)
		joyBt->setPosition(ccpAdd(getAngelePosition(joyBgR, angle), ccp(joyBg->getPosition().x, joyBg->getPosition().y)));
	else
		joyBt->setPosition(point);
	getDirection(angle);
	CCLOG("touch moved");
}

void Joystick::onTouchEnded(Touch* t, Event* ev)
{
	if (!isCanMove)
	{
		return;
	}
	auto joyBg = (CCSprite*)this->getChildByTag(tag_joyBg);
	auto joyBt = (CCSprite*)this->getChildByTag(tag_joyBt);
	joyBt->stopAllActions();
	joyBt->runAction(CCMoveTo::create(0.08f, joyBg->getPosition()));
	isCanMove = false;
	joyDirection = joy_stay;
	CCLOG("touch end");
}

float Joystick::getAngle(CCPoint pos1, CCPoint pos2)
{
	float px1 = pos1.x;
	float py1 = pos1.y;
	float px2 = pos2.x;
	float py2 = pos2.y;

	float a = px2 - px1;
	float b = py1 - py2;
	//算出斜边长度
	float c = sqrt(pow(a, 2) + pow(b, 2));
	float cosAngle = a / c;
	float angle = acos(cosAngle);
	//注意：当触屏的位置Y坐标<摇杆的Y坐标，我们要去反值-0~-180
	if (py2 < py1)
	{
		angle = -angle;
	}
	return angle;
}

Point Joystick::getAngelePosition(float r, float angle)
{
	return ccp(r*cos(angle), r*sin(angle));
}

void Joystick::getDirection(float angle)
{
	if (angle >= -PI / 4 && angle < PI / 4)
	{
		this->joyDirection = joy_right;
		this->runDirection = false;
	}
	else if (angle >= PI / 4 && angle < 3 * PI / 4)
	{
		this->joyDirection = joy_up;
	}
	else if ((angle >= 3 * PI / 4 && angle <= PI) || (angle >= -PI&&angle < -3 * PI / 4))
	{
		this->joyDirection = joy_left;
		this->runDirection = true;
	}
	else if (angle >= -3 * PI / 4 && angle < -PI / 4)
	{
		this->joyDirection = joy_down;
	}
}



