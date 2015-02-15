#include "BattleScene.h"
#include "cocostudio\CocoStudio.h"
#include "UIButton.h"
#include "cocostudio\CCArmature.h"
#include "Export.h"
#include "deprecated\CCDeprecated.h"
#include "2d\CCAnimationCache.h"
#include "GMap.h"
#include "Joystick\Joystick.h"
USING_NS_CC;
using namespace cocostudio;
using namespace ui;
BattleScene::BattleScene(){

}

BattleScene::~BattleScene(){}

bool BattleScene::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	mylayer = Layer::create();
	this->addChild(mylayer);
	//µØÍ¼
	mymap = GMap::create();
	mymap->InitMap("background/gp1.png", visibleSize);
	mylayer->addChild(mymap,0);
	//ÐéÄâÒ¡¸Ë
	joystick = Joystick::CreateJoystick("joy/joystick_1.png", "joy/joy_bt.png", ccp(60, 60));
	mylayer->addChild(joystick, 2);
	joystick->joy_isWork(true);
	//animation4
	ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("swordsman/Hero.ExportJson");
	Armature* armature = Armature::create("Hero");
	armature->setPosition(Vec2(origin.x + visibleSize.width / 2-100, origin.y + visibleSize.height/2-100));
	armature->getAnimation()->setSpeedScale(0.5f);
	armature->getAnimation()->playWithIndex(0);
	//¶¯»­Ö¡¼àÌý
	armature->getAnimation()->setFrameEventCallFunc([&](Bone* bone, const std::string &frameEventName, int originFrameIndex, int currentFrameIndex){
		CCLOG("animation frameEvent is done!");
		//¹Ç÷À»»·ô
		/*Skin* skin = Skin::create("Direction_bt.png");
		auto _bone = bone;
		bone->addDisplay(skin, 1);
		bone->changeDisplayWithIndex(n++%2,true);*/
	});
	mylayer->addChild(armature,1);

	Button* bt = Button::create();
	bt->loadTextures("joy/33-1.png", "joy/33-2.png", "");
	bt->setPosition(Vec2(origin.x + visibleSize.width - 50, origin.y + 50));
	bt->addTouchEventListener([=](CCObject* psender, Widget::TouchEventType type){
		static int num = 0;
		armature->getAnimation()->playWithIndex(num);
		if (num < 4)
			num++;
		else
			num = 0;
	});
	mylayer->addChild(bt,2);

	Button* closeBt = Button::create();
	closeBt->loadTextures("test/CloseNormal.png", "test/CloseSelected.png", "");
	closeBt->setPosition(Vec2(origin.x + visibleSize.width-50, origin.y + visibleSize.height-30));
	closeBt->addTouchEventListener([=](CCObject* psender, Widget::TouchEventType type){
		Director::getInstance()->popScene();
	});
	mylayer->addChild(closeBt,2);
	return true;
}

void BattleScene::update(float delta)
{
	
}

