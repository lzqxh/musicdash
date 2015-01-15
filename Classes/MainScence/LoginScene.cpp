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

	//鍔犺浇椤甸潰
	auto loadingLayer = Layer::create();
	this->addChild(loadingLayer);
	auto loading = Sprite::create("loginscence/loading.png");
    loading->setPosition(designWidth/2, designHeight/2);
    loading->setOpacity(0);
	loadingLayer->addChild(loading);

    auto loading1 = FadeIn::create(delayTime/3);
    auto loading2 = DelayTime::create(delayTime/3);
    auto loading3 = FadeOut::create(delayTime/3);
    auto loading4 = CallFunc::create([this, loadingLayer](){this->removeChild(loadingLayer);});
    loading->runAction(Sequence::create(loading1, loading2, loading3, loading4, nullptr));


	layer = Layer::create();
	addChild(layer);
	layer->setVisible(false);
	layer->setPosition(_center);
	layer->runAction(Sequence::create(delay, Show::create(), nullptr));

	//鑳屾櫙
	auto backGround1 = Sprite::create("loginscence/bg.png");
	layer->addChild(backGround1, 1);
	auto backGround2 = Sprite::create("loginscence/xiaoming.png");
	layer->addChild(backGround2, 6);


	//鍔ㄧ敾
	auto light = Sprite::create("loginscence/light.png");
	layer->addChild(light, 2);
	auto lightAction = CCBlink::create(3.0, 20);
	light->runAction(Sequence::create(delay, lightAction, nullptr));

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
		DelayTime::create(delayTime + 0.4),
		FadeIn::create(0.2f),
		ScaleTo::create(0.3f, 2.0),
		Spawn::create(
			MoveTo::create(0.2f, ccp(0, 0)),
			ScaleTo::create(0.2f, 1.0),
			nullptr
		),
		nullptr);
	title2->runAction(title2Action);
	layer->addChild(title2, 7);

	auto title3 = Sprite::create("loginscence/title3.png");
	title3->setPosition(ccp(-350, 400));
	auto title3Action = Sequence::create(
		DelayTime::create(delayTime + 1.6),
		MoveTo::create(0.2f, ccp(-350, 15)),
		JumpTo::create(1.2f, ccp(0, 0), 40, 3),
		nullptr);
	title3->runAction(title3Action);
	layer->addChild(title3, 3);



	//浼楁寜閽�
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

	btnStart->runAction(RepeatForever::create(Sequence::create(
//		ShakyTiles3D::create(0.5, Size(1, 1), 15, false),
		Repeat::create( Sequence::create(
			RotateBy::create(0.07, 9),
			RotateBy::create(0.14, -18),
			RotateBy::create(0.07, 9),
			nullptr),
			5
		),
		DelayTime::create(2), nullptr
	)));
//	auto node = NodeGrid::create();
//	node->setPosition(Vec2::ZERO);
//	auto sprite = Sprite::create("loginscence/button_start_n.png");
//	node->addChild(sprite);
//	node->setPosition(183, -160);
//	layer->addChild(node, 1001);
//	node->runAction(RepeatForever::create(Sequence::create(
//		Show::create(),
////		ShakyTiles3D::create(0.5, Size(1, 1), 15, false),
//		Ripple3D::create(5, Size(1, 1), ccp(50, 50), 200, 50, 100),
//		Hide::create(),
//		DelayTime::create(2), nullptr
//	)));

	return true;
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
