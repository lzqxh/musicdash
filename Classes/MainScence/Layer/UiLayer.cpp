#include "UiLayer.h"
#include "mainscence/SongSelectionScene.h"
#include "mainscence/LoginScene.h"
#include "consts\ResolutionConst.h"
#include "consts\Message.h"
#include "DataManager/DataVo.h"

bool UiLayer::init() {
	if (!Layer::init()) return false;

	auto pauseButtonItem = MenuItemImage::create("mainscence/home.png", "mainscence/home.png",
		CC_CALLBACK_1(UiLayer::showControlMenu, this));
	pauseButton = Menu::create(pauseButtonItem, nullptr);
	pauseButton->setAnchorPoint(Point(0.0f, 0.0f));
	pauseButton->setScale(0.6);
	pauseButton->setPosition(50, 50);
	addChild(pauseButton, 1000);

	progress = Sprite::create("mainscence/progress.png");
	addChild(progress, 1000);
	progress->setAnchorPoint(Point(0, 0.5));
	progress->setPosition(10, designHeight/2);
	progress->setScale(0.5);

	head = Sprite::create("mainscence/head.png");
	progress->addChild(head);
	head->setScale(0.6);
	head->setAnchorPoint(Point(0, 0));

	auto cache = SpriteFrameCache::getInstance();

	energy = Sprite::create();
	addChild(energy);
	energy->setAnchorPoint(Point(0, 0));
	energy->setPosition(designWidth - 100, designHeight - 370);
	energy->setScale(0.4);
	energy->setDisplayFrame(cache->getSpriteFrameByName("energy_0.png"));

	scoreBox = Sprite::create("mainscence/scoreBox2.png");
	addChild(scoreBox);
	scoreBox->setAnchorPoint(Point(1.0f,1.0f));
	scoreBox->setPosition(designWidth - 10, designHeight - 10);

	scheduleUpdate();
	
	return true;
}

void UiLayer::onEnter() {
	Layer::onEnter();
	score = distance = nullptr;
	_eventDispatcher->addCustomEventListener(Message::beer_get, CC_CALLBACK_1(UiLayer::energyUp, this));
	_eventDispatcher->addCustomEventListener(Message::beer_effect_start, CC_CALLBACK_1(UiLayer::energyExp, this));
	_eventDispatcher->addCustomEventListener(Message::disp_score, CC_CALLBACK_1(UiLayer::displayScore, this));
	_eventDispatcher->addCustomEventListener(Message::game_stop, CC_CALLBACK_1(UiLayer::showGameoverBox, this));
	displayScore(nullptr);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch *touch, Event *event)->bool{
		auto location = touch->getLocation();
		auto origin = energy->getPosition();
		auto rect = Rect(origin.x, origin.y,
			energy->getDisplayFrame()->getOriginalSize().width * energy->getScale(),
			energy->getDisplayFrame()->getOriginalSize().height * energy->getScale());

		if (rect.containsPoint(location)) {
			this->_eventDispatcher->dispatchCustomEvent(Message::beer_click, this);
		}
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void UiLayer::onExit() {
	_eventDispatcher->removeEventListenersForTarget(this, true);
	_eventDispatcher->removeCustomEventListeners(Message::beer_get);
	_eventDispatcher->removeCustomEventListeners(Message::beer_effect_start);
	_eventDispatcher->removeCustomEventListeners(Message::disp_score);
	_eventDispatcher->removeCustomEventListeners(Message::game_stop);

	Layer::onExit();
}

void UiLayer::showControlMenu(Ref *pSender) {
	_eventDispatcher->dispatchCustomEvent(Message::game_pause, nullptr);
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

	auto restart = MenuItemImage::create("mainscence/restart.png", "mainscence/restart.png", [this](Ref *) {
		CocosDenshion::SimpleAudioEngine::getInstance()->
			playEffect("soundeffect/button.wav");
		this->removeChild(controlMenu);
		_eventDispatcher->dispatchCustomEvent(Message::game_restart, nullptr);
		pauseButton->setVisible(true);
		pauseButton->runAction(MoveTo::create(0.5f, Point(50, 50)));
		pauseButton->runAction(ScaleTo::create(0.5f, 0.6));
	});
	auto stop = MenuItemImage::create("mainscence/home.png", "mainscence/home.png", [this](Ref *) {
		CocosDenshion::SimpleAudioEngine::getInstance()->
			playEffect("soundeffect/button.wav");
		this->removeChild(controlMenu);
//		_eventDispatcher->dispatchCustomEvent(Message::game_stop, nullptr);
//		pauseButton->setVisible(true);
//		pauseButton->setPosition(50, 50);
//		pauseButton->setScale(0.6);
		auto scene = LoginScene::create();
		Director::getInstance()->replaceScene(scene);
	});
	auto cancel = MenuItemImage::create("mainscence/start.png", "mainscence/start.png", [this](Ref *) {
		CocosDenshion::SimpleAudioEngine::getInstance()->
			playEffect("soundeffect/button.wav");
		this->removeChild(controlMenu);
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		_eventDispatcher->dispatchCustomEvent(Message::game_resume, nullptr);
		pauseButton->setVisible(true);
		pauseButton->runAction(MoveTo::create(0.5f, Point(50, 50)));
		pauseButton->runAction(ScaleTo::create(0.5f, 0.6));
	});
	controlMenu = Menu::create(restart, stop, cancel, nullptr);
	controlMenu->setAnchorPoint(Point(0.5, 0.5));
	controlMenu->setPosition(_center);
	this->addChild(controlMenu, 1000);
	stop->setPosition(-100, 100);
	restart->setPosition(100, 100);
	cancel->setPosition(0, -100);
	stop->setVisible(false);
	restart->setOpacity(0);
	cancel->setOpacity(0);
	
	float actionTime = 0.5;
	auto move = MoveTo::create(actionTime, controlMenu->convertToWorldSpace(stop->getPosition()));
	auto scale = ScaleTo::create(actionTime, 1);
	pauseButton->runAction(move);
	pauseButton->runAction(scale);
	pauseButton->runAction(Sequence::create(
		DelayTime::create(actionTime),
		Hide::create(), nullptr));

	stop->runAction(Sequence::create(
		DelayTime::create(actionTime),
		Show::create(), nullptr));
	restart->runAction(Sequence::create(
		DelayTime::create(actionTime),
		FadeIn::create(actionTime), nullptr));
	cancel->runAction(Sequence::create(
		DelayTime::create(actionTime),
		FadeIn::create(actionTime), nullptr));
}

void UiLayer::update(float dt) {
	auto spriteLength = progress->getDisplayFrame()->getOriginalSize().height - 100;
	float musicLength = DataVo::inst()->musicLength;
	float time = curTime;
	auto y = time / musicLength * spriteLength;
	head->setPosition(45, y < 0 ? 0 : y);
}

void UiLayer::energyUp(EventCustom *event) {
	auto cache = SpriteFrameCache::getInstance();
	int &energyValue = DataVo::inst()->energyValue;
	energyValue = energyValue >= 10 ? 10 : energyValue + 1;
	char name[20];
	sprintf(name, "energy_%d.png", energyValue);
	energy->setDisplayFrame(cache->getSpriteFrameByName(name));
}

void UiLayer::energyExp(EventCustom *event) {
	//Maybe need a action ?!
	auto cache = SpriteFrameCache::getInstance();
	int &energyValue = DataVo::inst()->energyValue;
	char name[20];
	sprintf(name, "energy_%d.png", energyValue);
	energy->setDisplayFrame(cache->getSpriteFrameByName(name));
}

void UiLayer::displayScore(EventCustom *event) {
	if (score) scoreBox->removeChild(score, true);

	score = LabelBMFont::create(std::to_string(DataVo::inst()->score),
		"fonts/number_2.fnt");
	scoreBox->addChild(score);
	score->setAnchorPoint(Point(1, 0.5f));
	float x = scoreBox->getTextureRect().getMaxX() * scoreBox->getScale() - 45;
	float y = scoreBox->getTextureRect().getMaxY() * scoreBox->getScale() / 4  - 2;
	score->setPosition(x, y);

	if (distance) scoreBox->removeChild(distance, true);

	distance = LabelBMFont::create(std::to_string(DataVo::inst()->distance),
		"fonts/number_2.fnt");
	scoreBox->addChild(distance);
	distance->setAnchorPoint(Point(1, 0.5f));
	x = scoreBox->getTextureRect().getMaxX() * scoreBox->getScale() - 45;
	y = scoreBox->getTextureRect().getMaxY() * scoreBox->getScale() * 3 / 4 - 3;
	distance->setPosition(x, y);
}

void UiLayer::showGameoverBox(EventCustom *event) {
	CocosDenshion::SimpleAudioEngine::getInstance()->
		playBackgroundMusic("soundeffect/gameover.mp3");
	auto bg = Sprite::create("mainscence/gameoverbox.png");
	bg->setPosition(ccp(_center.x, _center.y + 100));
	bg->setAnchorPoint(ccp(0.5, 0.5));
	bg->setScale(PNG_SCALE);

	int n = DataVo::inst()->score;
	auto scoreLabel = LabelBMFont::create(std::to_string(n), "fonts/number_2.fnt");
	scoreLabel->setAnchorPoint(ccp(1.0, 0.5));
	scoreLabel->setPosition(1200, 1500);
	scoreLabel->setScale(1.0/PNG_SCALE);
	bg->addChild(scoreLabel, 10);

	n = DataVo::inst()->distance;
	auto distanceLabel = LabelBMFont::create(std::to_string(n), "fonts/number_2.fnt");
	distanceLabel->setAnchorPoint(ccp(1.0, 0.5));
	distanceLabel->setPosition(1200, 1650);
	distanceLabel->setScale(1.0/PNG_SCALE);
	bg->addChild(distanceLabel, 10);

	auto button1 = MenuItemImage::create("mainscence/home.png", "mainscence/home.png");
	button1->setPosition(ccp(_center.x - 270, _center.y + 150));

	auto button2 = MenuItemImage::create("mainscence/restart.png", "mainscence/restart.png");
	button2->setPosition(ccp(_center.x, _center.y + 150));

	auto button3 = MenuItemImage::create("mainscence/start.png", "mainscence/start.png");
	button3->setPosition(ccp(_center.x + 270, _center.y + 150));

	auto menu = Menu::create(button1, button2, button3, nullptr);
	menu->setAnchorPoint(ccp(0, 0));
	menu->setPosition(ccp(550, 630));
	bg->addChild(menu);

	this->addChild(bg, 1000);
	button1->setCallback([this, bg, menu](Ref *){
		CocosDenshion::SimpleAudioEngine::getInstance()->
			playEffect("soundeffect/button.wav");
		auto scene = LoginScene::create();
		Director::getInstance()->replaceScene(scene);
	});

	button2->setCallback([this, bg](Ref *){
		this->removeChild(bg);
		this->_eventDispatcher->dispatchCustomEvent(Message::game_restart, nullptr);
		CocosDenshion::SimpleAudioEngine::getInstance()->
			playEffect("soundeffect/button.wav");
	});

	button3->setCallback([&](Ref *){
		CocosDenshion::SimpleAudioEngine::getInstance()->
			playEffect("soundeffect/button.wav");
		auto scene = SongSelectionScene::create();
		Director::getInstance()->replaceScene(scene);
	});

	return;
}

