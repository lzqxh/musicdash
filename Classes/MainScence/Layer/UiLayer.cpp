#include "UiLayer.h"
#include "mainscence/SongSelectionScene.h"
#include "mainscence/LoginScene.h"
#include "consts\ResolutionConst.h"
#include "consts\Message.h"
#include "DataManager/DataVo.h"
#include "DataManager/LocalRecord.h"
#include "../OptionsScene.h"
#include "ScoreLayer.h"

bool UiLayer::init() {
	if (!Layer::init()) return false;

	auto pauseButtonItem = MenuItemImage::create("buttons/button_home_n.png", "buttons/button_home_s.png",
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

	auto restart = MenuItemImage::create("buttons/button_restart_n.png", "buttons/button_restart_s.png", [this](Ref *) {
		if (OptionsScene::ifSound)
			CocosDenshion::SimpleAudioEngine::getInstance()->
				playEffect("soundeffect/button.wav");
		this->removeChild(controlMenu);
		_eventDispatcher->dispatchCustomEvent(Message::game_restart, nullptr);
		pauseButton->setVisible(true);
		pauseButton->runAction(MoveTo::create(0.5f, Point(50, 50)));
		pauseButton->runAction(ScaleTo::create(0.5f, 0.6));
	});
	auto stop = MenuItemImage::create("buttons/button_home_n.png", "buttons/button_home_s.png", [this](Ref *) {
		if (OptionsScene::ifSound)
			CocosDenshion::SimpleAudioEngine::getInstance()->
				playEffect("soundeffect/button.wav");
		this->removeChild(controlMenu);
		Director::getInstance()->popScene();
	});
	auto cancel = MenuItemImage::create("buttons/button_continue_n.png", "buttons/button_continue_s.png", [this](Ref *) {
		if (OptionsScene::ifSound)
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
	if (score)
		scoreBox->removeChild(score, true);
	score = LabelBMFont::create(std::to_string(ScoreLayer::score),
		"fonts/number_2.fnt");
	scoreBox->addChild(score);
	score->setAnchorPoint(Point(1, 0.5f));
	float x = scoreBox->getTextureRect().getMaxX() * scoreBox->getScale() - 50;
	float y = scoreBox->getTextureRect().getMaxY() * scoreBox->getScale() / 4  + 3;
	score->setPosition(x, y);

	if (distance) scoreBox->removeChild(distance, true);

	distance = LabelBMFont::create(std::to_string(DataVo::inst()->distance),
		"fonts/number_2.fnt");
	scoreBox->addChild(distance);
	distance->setAnchorPoint(Point(1, 0.5f));
	x = scoreBox->getTextureRect().getMaxX() * scoreBox->getScale() - 50;
	y = scoreBox->getTextureRect().getMaxY() * scoreBox->getScale() * 3 / 4 - 3;
	distance->setPosition(x, y);
}

void UiLayer::showGameoverBox(EventCustom *event) {
	auto name = DataVo::inst()->musicname;
	CocosDenshion::SimpleAudioEngine::getInstance()->
		playBackgroundMusic("soundeffect/gameover.mp3");
	auto bg = Sprite::create("mainscence/gameoverbox.png");
	bg->setPosition(ccp(_center.x, _center.y));
	bg->setAnchorPoint(ccp(0.5, 0.5));

	int n = DataVo::inst()->coins;
	auto scoreLabel = LabelBMFont::create(std::to_string(ScoreLayer::score), "fonts/number_2.fnt");
	scoreLabel->setAnchorPoint(ccp(1.0, 0.5));
	scoreLabel->setPosition(designWidth * 8.7 / 11.29, designHeight * (1 - 14.26 / 18.81));
	scoreLabel->setScale(1.3f);
	bg->addChild(scoreLabel, 10);

	int dis = DataVo::inst()->distance;
	auto distanceLabel = LabelBMFont::create(std::to_string(dis), "fonts/number_2.fnt");
	distanceLabel->setAnchorPoint(ccp(1.0, 0.5));
	distanceLabel->setPosition(designWidth * 8.7 / 11.29, designHeight * (1 - 12.44 / 18.81));
	distanceLabel->setScale(1.3f);
	bg->addChild(distanceLabel, 10);

	n = LocalRecord::inst()->getHighestRecord(name);
	auto highestLabel = LabelBMFont::create(std::to_string(n), "fonts/number_2.fnt");
	highestLabel->setAnchorPoint(ccp(1.0, 0.5));
	highestLabel->setPosition(designWidth * 8.7 / 11.29, designHeight * (1 - 16.08 / 18.81));
	highestLabel->setScale(1.3f);
	bg->addChild(highestLabel, 10);

	auto pic = Sprite::create(name + "/pic.png");
	pic->setPosition(designWidth * 4.2 / 11.29, designHeight * (1 - 5.05 / 18.81));
	pic->setRotation(-10);
	bg->addChild(pic);

	auto songname = Sprite::create(name + "/name.png");
	songname->setAnchorPoint(ccp(0, 0.5f));
	songname->setPosition(designWidth * 1.72 / 11.29, designHeight * (1 - 10.96 / 18.81));
	bg->addChild(songname);

	auto passGate = Sprite::create("mainscence/passgate.png");
	passGate->setPosition(_center);
	passGate->setScale(3.2f);
	passGate->runAction(Sequence::create(DelayTime::create(4.8f), ScaleTo::create(0.3, 1), nullptr));
	bg->addChild(passGate);

	auto button1 = MenuItemImage::create("buttons/button_home_n.png", "buttons/button_home_s.png");
	button1->setPosition(ccp(designWidth * (0.72 + 1.97 * 0.5) / 11.29 , 50));
	button1->setScale(0.8f);

	auto button2 = MenuItemImage::create("buttons/button_restart_n.png", "buttons/button_restart_s.png");
	button2->setPosition(ccp(designWidth * (0.72 + 1.97 * 1.5) / 11.29 , 50));
	button2->setScale(0.8f);

	auto button3 = MenuItemImage::create("buttons/button_shop_n.png", "buttons/button_shop_s.png");
	button3->setPosition(ccp(designWidth * (0.72 + 1.97 * 2.5) / 11.29 , 50));
	button3->setScale(0.8f);

	auto button4 = MenuItemImage::create("buttons/button_rank_n.png", "buttons/button_rank_s.png");
	button4->setPosition(ccp(designWidth * (0.72 + 1.97 * 3.5) / 11.29 , 50));
	button4->setScale(0.8f);

	auto button5 = MenuItemImage::create("buttons/button_achievement_n.png", "buttons/button_achievement_s.png");
	button5->setPosition(ccp(designWidth * (0.72 + 1.97 * 4.5) / 11.29 , 50));
	button5->setScale(0.8f);

	auto menu = Menu::create(button1, button2, button3, button4, button5, nullptr);
	menu->setPosition(0, 0);
	bg->addChild(menu);

	this->addChild(bg, 1000);
	button1->setCallback([this, bg, menu](Ref *){
		if (OptionsScene::ifSound)
			CocosDenshion::SimpleAudioEngine::getInstance()->
				playEffect("soundeffect/button.wav");
		Director::getInstance()->popScene();
	});

	button2->setCallback([this, bg](Ref *){
		this->removeChild(bg);
		this->_eventDispatcher->dispatchCustomEvent(Message::game_restart, nullptr);
		if (OptionsScene::ifSound)
			CocosDenshion::SimpleAudioEngine::getInstance()->
				playEffect("soundeffect/button.wav");
	});
	return;
}

