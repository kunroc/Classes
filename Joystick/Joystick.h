#pragma once;
#include "cocos2d.h"
#include "base\CCPlatformMacros.h"
USING_NS_CC;
class Joystick : public cocos2d::Layer
{
	typedef enum{
		tag_joyBg,
		tag_joyBt,
	}tagForJoystick;

	typedef enum{
		joy_stay,
		joy_right,
		joy_up,
		joy_left,
		joy_down,
	}joyDirecton;
public:
	static Joystick* CreateJoystick(const char* bgImage, const char* Image, Point position);
	Joystick();
	~Joystick();
	CREATE_FUNC(Joystick);
	bool joy_isWork(bool iswork);
	int joyDirection;
	bool runDirection;
private:
	bool initView(const char* bgImage, const char* Image, Point position);
	bool onTouchBegan(Touch* t, Event* ev);
	void onTouchMoved(Touch* t, Event* ev);
	void onTouchEnded(Touch* t, Event* ev);
	float getAngle(CCPoint pos1, CCPoint pos2);
	Point getAngelePosition(float r,float angle);
	void getDirection(float angle);
	bool isCanMove;
};
