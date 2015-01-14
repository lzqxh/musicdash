#include "LoginScene.h"
#include "consts\ResolutionConst.h"
#include "SongSelectionScene.h"
using namespace cocos2d;
using namespace ui;

bool LoginScene::init() {
	if (!Scene::init()) return false;


    float delayTime = 3;
    float actionTime = 0.4;
    auto delay = DelayTime::create(delayTime);

	//加载页面
	auto loadingLayer = Layer::create();
	this->addChild(loadingLayer);
	auto loading = Sprite::create("loginscence/loading.png");
    loading->setPosition(designWidth/2, designHeight/2);
    loading->setOpacity(0);
	loadingLayer->addChild(loading);
    
    auto loading1 = FadeIn::create(delayTime/3);
    auto loading2 = DelayTime::create(delayTime/3);
    auto loading3 = FadeOut::create(delayTime/3);
    auto loading4 = CallFunc::create([this, loadingLayer](){this->layer->removeChild(loadingLayer);});
    loading->runAction(Sequence::create(loading1, loading2, loading3, loading4, nullptr));

	layer = Layer::create();
	addChild(layer);
	layer->setVisible(false);
	layer->setPosition(_center);
	layer->runAction(Sequence::create(delay, Show::create(), nullptr));

	//背景
	auto backGround1 = Sprite::create("loginscence/bg.png");
	backGround1->setVisible(true);
	layer->addChild(backGround1, 1);
	auto backGround2 = Sprite::create("loginscence/xiaoming.png");
	backGround2->setVisible(true);
	layer->addChild(backGround2, 6);

	//动画
	auto light = Sprite::create("loginscence/light.png");
//	light->setPosition(_center);
	layer->addChild(light, 2);
	auto lightAction = CCBlink::create(1.0, 2);
	light->runAction(RepeatForever::create(lightAction));

	auto title1 = Sprite::create("loginscence/title1.png");
	title1->setPosition(ccp(0, 500));
	auto title1Action = Sequence::create(
		DelayTime::create(delayTime),
		MoveTo::create(0.5f, Vec2::ZERO),
		nullptr);
	title1->runAction(title1Action);
	layer->addChild(title1, 4);

	auto title2 = Sprite::create("loginscence/title2.png");
	title2->setPosition(ccp(0, -400));
	title2->setOpacity(0);
	auto title2Action = Sequence::create(
		DelayTime::create(delayTime + 1),
		FadeIn::create(0.7f),
		ScaleTo::create(0.7f, 2.0),
		Spawn::create(
			MoveTo::create(0.7f, ccp(0, 0)),
			ScaleTo::create(0.7f, 1.0),
			nullptr
		),
		nullptr);
	title2->runAction(title2Action);
	layer->addChild(title2, 7);

	auto title3 = Sprite::create("loginscence/title3.png");
	title3->setPosition(ccp(-350, 400));
	auto title3Action = Sequence::create(
		DelayTime::create(delayTime + 3.5),
		MoveTo::create(0.2f, ccp(-350, 15)),
		JumpTo::create(1.2f, ccp(0, 0), 40, 3),
		nullptr);
	title3->runAction(title3Action);
	layer->addChild(title3, 3);

	//众按钮
	auto btnStart = MenuItemImage::create("loginscence/button_start_n.png", "loginscence/button_start_s.png",
		CC_CALLBACK_1(LoginScene::onStartButtonClicked, this));
	btnStart->setPosition(183, -160);
	auto btnShop = MenuItemImage::create("loginscence/button_shop_n.png", "loginscence/button_shop_s.png");
	btnShop->setPosition(180, -285);
	auto btnHelp = MenuItemImage::create("loginscence/button_help_n.png", "loginscence/button_help_s.png");
	btnHelp->setPosition(170, -390);
	auto btnAchieve = MenuItemImage::create("loginscence/button_achievement_n.png", 
		"loginscence/button_achievement_s.png");
	btnAchieve->setPosition(-150, -440);
	auto btnOptions = MenuItemImage::create("loginscence/button_options_n.png", 
		"loginscence/button_options_s.png");
	btnOptions->setPosition(0, -440);

	mainMenu = Menu::create(btnStart, btnShop, btnHelp, btnAchieve, btnOptions, nullptr);
	mainMenu->setPosition(Vec2::ZERO);
	layer->addChild(mainMenu, 1000);

//	//开场动画
//	auto road = Sprite::create("loginscence/login2.png");
//	auto boss = Sprite::create("loginscence/boss.png");
//	auto xiaoming = Sprite::create("loginscence/xiaoming.png");
//	auto title = Sprite::create();
//	title->setSpriteFrame("paodiaoyingxiong.png");
////	title->setColor(Color3B::BLACK);
//	auto building1 = Sprite::create("loginscence/building1.png");
//	auto building2 = Sprite::create("loginscence/building2.png");
//	auto building3 = Sprite::create("loginscence/building3.png");
//	auto building4 = Sprite::create("loginscence/building4.png");
//	auto building5 = Sprite::create("loginscence/building5.png");
//	auto logo = Sprite::create("logo.png");
//	auto cloud1 = Sprite::create("loginscence/cloud.png");
//	auto cloud2 = Sprite::create("loginscence/cloud.png");
//	auto cloud3 = Sprite::create("loginscence/cloud.png");

//	layer->addChild(road, 0);
//	layer->addChild(boss, 1);
//	layer->addChild(xiaoming, 1);
//	layer->addChild(title, 1000);
//	layer->addChild(building4, 0);
//	layer->addChild(building5, 0);
//	layer->addChild(building1, 0);
//	layer->addChild(building2, 0);
//	layer->addChild(building3, 0);
//	layer->addChild(logo, 1000);
//	layer->addChild(cloud1, 1);
//	layer->addChild(cloud2, 1);
//	layer->addChild(cloud3, 1);

//	road->setScaleY(1);
//	road->setAnchorPoint(Point(0.25, 0));
//	road->setPosition(0, 0);
//	road->setVisible(false);
//	boss->setScale(0.15f);
//	boss->setAnchorPoint(Point(1, 0));
//	boss->setPosition(0, 50);
//	xiaoming->setScale(0.15f);
//	xiaoming->setAnchorPoint(Point(0, 0));
//	xiaoming->setPosition(designWidth + 20, 30);
//	title->setAnchorPoint(Point(0, 1));
//	title->setScale(0.7f);
//	title->setPosition(30, designHeight-50);
//	title->setOpacity(0);
//	building1->setScale(0.8);
//	building2->setScale(0.8);
//	building3->setScale(0.8);
//	building1->setAnchorPoint(Point(0.3, 0));
//	building2->setAnchorPoint(Point(0.3, 0));
//	building3->setAnchorPoint(Point(0.3, 0));
//	building4->setAnchorPoint(Point(0.3, 0));
//	building5->setAnchorPoint(Point(0.3, 0));
//	building1->setPosition(designWidth * 0.15, designHeight);
//	building2->setPosition(designWidth * 0.45, designHeight);
//	building3->setPosition(designWidth * 0.75, designHeight);
//	building4->setPosition(designWidth * 0.2, designHeight);
//	building5->setPosition(designWidth * 0.6, designHeight);
//	mainMenu->ignoreAnchorPointForPosition(false);
//	mainMenu->setAnchorPoint(Point(0, 0.7));
//	mainMenu->setPosition(Point(designWidth/2, 0));
//	logo->setScale(0.13);
//	logo->setPosition(designWidth - 150, designHeight - 100);
//	logo->setOpacity(0);
//	cloud1->setScale(0.5);
//	cloud2->setScale(0.3);
//	cloud3->setScale(0.7);
//	cloud1->setOpacity(0);
//	cloud2->setOpacity(0);
//	cloud3->setOpacity(0);
//	cloud1->setPosition( 100, designHeight - 100 );
//	cloud2->setPosition( designWidth - 100, designHeight - 50 );
//	cloud3->setPosition( designWidth/2, designHeight - 130 );

//	auto backGround1 = Show::create();
//	backGround->runAction(Sequence::create(delay, backGround1, nullptr));
//	auto road1 = Show::create();
//	road->runAction(Sequence::create(delay, road1, nullptr));
//	auto boss1 = MoveBy::create(actionTime, Vec2(designWidth/2 + 10, 0));
//	boss->runAction(Sequence::create(delay, boss1, nullptr));
//	auto xiaoming1 = MoveBy::create(actionTime, Vec2(-designWidth/2 + 20, 0));
//	xiaoming->runAction(Sequence::create(delay, xiaoming1, nullptr));
//	auto title1 = FadeIn::create(actionTime);
//	title->runAction(Sequence::create(delay, title1, nullptr));
//	auto building11 = MoveBy::create(actionTime, Vec2(0, -designHeight/2 - 140));
//	building1->runAction(Sequence::create(delay, building11, nullptr));
//	auto building21 = MoveBy::create(actionTime, Vec2(0, -designHeight/2 - 140));
//	building2->runAction(Sequence::create(delay, building21, nullptr));
//	auto building31 = MoveBy::create(actionTime, Vec2(0, -designHeight/2 - 140));
//	building3->runAction(Sequence::create(delay, building31, nullptr));
//	auto building41 = MoveBy::create(actionTime, Vec2(0, -designHeight/2 - 30));
//	building4->runAction(Sequence::create(delay, building41, nullptr));
//	auto building51 = MoveBy::create(actionTime, Vec2(0, -designHeight/2 - 30));
//	building5->runAction(Sequence::create(delay, building51, nullptr));
//	auto mainMenu1 = MoveBy::create(actionTime, Vec2(0, designHeight/2 - 20));
//	mainMenu->runAction(Sequence::create(delay, mainMenu1, nullptr));
//	auto logo1 = FadeIn::create(actionTime);
//	logo->runAction(Sequence::create(delay, logo1, nullptr));
//	auto cloud11 = MoveBy::create(10 * actionTime, Vec2(designWidth - 200, 0));
//	cloud1->runAction(RepeatForever::create(Sequence::create(
//		cloud11, cloud11->reverse(), nullptr)));
//	cloud1->runAction(Sequence::create(delay, FadeTo::create(actionTime, 200), nullptr));
//	auto cloud21 = MoveBy::create(16 * actionTime, Vec2(-designWidth + 200, 0));
//	cloud2->runAction(RepeatForever::create(Sequence::create(
//		cloud21, cloud21->reverse(), nullptr)));
//	cloud2->runAction(Sequence::create(delay, FadeTo::create(actionTime, 200), nullptr));
//	auto cloud31 = MoveBy::create(20 * actionTime, Vec2(designWidth/2 , 0));
//	cloud3->runAction(RepeatForever::create(Sequence::create(
//		cloud31, cloud31->reverse(), cloud31->reverse(), cloud31, nullptr)));
//	cloud3->runAction(Sequence::create(delay, FadeTo::create(actionTime, 200), nullptr));

//	return true;
}

void LoginScene::onEnter() {
	Scene::onEnter();	
	CocosDenshion::SimpleAudioEngine::getInstance()->
		playBackgroundMusic("soundeffect/bg.mp3", true);
}

void LoginScene::onStartButtonClicked(Ref *pSender) {
	auto songSelection = SongSelectionScene::create();
	Director::getInstance()->pushScene(TransitionFade::create(0.5,songSelection));
}