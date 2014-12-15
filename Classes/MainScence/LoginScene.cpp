#include "LoginScene.h"
#include "consts\ResolutionConst.h"
#include "SongSelectionScene.h"
using namespace cocos2d;
using namespace ui;

bool LoginScene::init() {
	if (!Scene::init()) return false;

	layer = Layer::create();
	addChild(layer);
	auto startButton = MenuItemImage::create("loginscence/startButtonNormal.png", "loginscence/startButtonSelected.png",
		CC_CALLBACK_1(LoginScene::onStartButtonClicked, this));
	mainMenu = Menu::create(startButton, nullptr);
	mainMenu->setContentSize(startButton->getContentSize());
	layer->addChild(mainMenu, 1000);

	//¿ª³¡¶¯»­
	auto loading = Sprite::create("loginscence/loading.png");
	auto backGround = Sprite::create("loginscence/login1.png");
	auto road = Sprite::create("loginscence/login2.png");
	auto boss = Sprite::create("loginscence/boss.png");
	auto xiaoming = Sprite::create("loginscence/xiaoming.png");
	auto title = LabelTTF::create("MusicDash", "fonts/Marker Felt.ttf", 70);
	title->setColor(Color3B::BLACK);
	auto building1 = Sprite::create("building/building_1.png");
	auto building2 = Sprite::create("building/building_2.png");
	auto building3 = Sprite::create("building/building_3.png");

	layer->addChild(loading, 1000);
	layer->addChild(backGround, -1000);
	layer->addChild(road, 0);
	layer->addChild(boss, 1);
	layer->addChild(xiaoming, 1);
	layer->addChild(title, 1);
	layer->addChild(building1, 0);
	layer->addChild(building2, 0);
	layer->addChild(building3, 0);

	loading->setScale(0.5f);
	loading->setPosition(designWidth/2, designHeight/2);
	loading->setOpacity(0);
	backGround->setPosition(designWidth/2, designHeight/2);
	backGround->setVisible(false);
	road->setScaleY(0.5);
	road->setAnchorPoint(Point(0.25, 0));
	road->setPosition(0, 0);
	road->setVisible(false);
	boss->setScale(0.1f);
	boss->setAnchorPoint(Point(1, 0));
	boss->setPosition(0, 50);
	xiaoming->setScale(0.1f);
	xiaoming->setAnchorPoint(Point(0, 0));
	xiaoming->setPosition(designWidth + 20, 30);
	title->setAnchorPoint(Point(0, 1));
	title->setPosition(50, designHeight-50);
	title->setOpacity(0);
	building1->setScale(0.5);
	building2->setScale(0.5);
	building3->setScale(0.5);
	building1->setAnchorPoint(Point(0.3, 0));
	building2->setAnchorPoint(Point(0.3, 0));
	building3->setAnchorPoint(Point(0.3, 0));
	building1->setPosition(designWidth * 0.15, designHeight);
	building2->setPosition(designWidth * 0.45, designHeight);
	building3->setPosition(designWidth * 0.75, designHeight);
	mainMenu->ignoreAnchorPointForPosition(false);
	mainMenu->setAnchorPoint(Point(0, 0.7));
	mainMenu->setPosition(Point(designWidth/2, 0));

	float delayTime = 2;
	float actionTime = 0.4;
	auto delay = DelayTime::create(delayTime);
	
	auto loading1 = FadeIn::create(delayTime/3);
	auto loading2 = DelayTime::create(delayTime/3);
	auto loading3 = FadeOut::create(delayTime/3);
	auto loading4 = CallFunc::create([this, loading](){this->layer->removeChild(loading);});
	loading->runAction(Sequence::create(loading1, loading2, loading3, loading4, nullptr));
	auto backGround1 = Show::create();
	backGround->runAction(Sequence::create(delay, backGround1, nullptr));
	auto road1 = Show::create();
	road->runAction(Sequence::create(delay, road1, nullptr));
	auto boss1 = MoveBy::create(actionTime, Vec2(designWidth/2 - 100, 0));
	boss->runAction(Sequence::create(delay, boss1, nullptr));
	auto xiaoming1 = MoveBy::create(actionTime, Vec2(-designWidth/2 + 100, 0));
	xiaoming->runAction(Sequence::create(delay, xiaoming1, nullptr));
	auto title1 = FadeIn::create(actionTime);
	title->runAction(Sequence::create(delay, title1, nullptr));
	auto building11 = MoveBy::create(actionTime, Vec2(0, -designHeight/2 - 120));
	building1->runAction(Sequence::create(delay, building11, nullptr));
	auto building21 = MoveBy::create(actionTime, Vec2(0, -designHeight/2 - 120));
	building2->runAction(Sequence::create(delay, building21, nullptr));
	auto building31 = MoveBy::create(actionTime, Vec2(0, -designHeight/2 - 120));
	building3->runAction(Sequence::create(delay, building31, nullptr));
	auto mainMenu1 = MoveBy::create(actionTime, Vec2(0, designHeight/2 + 50));
	mainMenu->runAction(Sequence::create(delay, mainMenu1, nullptr));

	return true;
}

void LoginScene::onEnter() {
	Scene::onEnter();
}

void LoginScene::onStartButtonClicked(Ref *pSender) {
	auto songSelection = SongSelectionScene::create();
	Director::getInstance()->pushScene(songSelection);
}