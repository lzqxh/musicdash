#include "UiLayer.h"
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
	energy->setPosition(designWidth - 100, designHeight - 300);
	energy->setScale(0.4);
	energy->setDisplayFrame(cache->getSpriteFrameByName("energy_0.png"));
	energyValue = 0;

	scoreBox = Sprite::create("mainscence/scoreBox.png");
	addChild(scoreBox);
	scoreBox->setAnchorPoint(Point(1.0f,1.0f));
	scoreBox->setPosition(designWidth - 10, designHeight - 10);

	scheduleUpdate();
	
	return true;
}

void UiLayer::onEnter() {
	Layer::onEnter();
	_eventDispatcher->addCustomEventListener(Message::get_beer, CC_CALLBACK_1(UiLayer::energyUp, this));
	_eventDispatcher->addCustomEventListener(Message::score, CC_CALLBACK_1(UiLayer::displayScore, this));
	displayScore(nullptr);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch *touch, Event *event)->bool{
		auto location = touch->getLocation();
		auto origin = energy->getPosition();
		auto rect = Rect(origin.x, origin.y,
			energy->getDisplayFrame()->getOriginalSize().width * energy->getScale(),
			energy->getDisplayFrame()->getOriginalSize().height * energy->getScale());

		if ( this->energyValue == 10 
			&& rect.containsPoint(location)) {
			energyValue = 0;
			energy->setDisplayFrame(
				SpriteFrameCache::getInstance()->getSpriteFrameByName("energy_0.png"));
		}
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void UiLayer::onExit() {
	_eventDispatcher->removeEventListenersForTarget(this, true);
	Layer::onExit();
}

void UiLayer::showControlMenu(Ref *pSender) {
	_eventDispatcher->dispatchCustomEvent(Message::game_pause, nullptr);
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

	auto restart = MenuItemImage::create("mainscence/restart.png", "mainscence/restart.png", [this](Ref *) {
		this->removeChild(controlMenu);
		_eventDispatcher->dispatchCustomEvent(Message::game_restart, nullptr);
		pauseButton->setVisible(true);
		pauseButton->runAction(MoveTo::create(0.5f, Point(50, 50)));
		pauseButton->runAction(ScaleTo::create(0.5f, 0.6));
	});
	auto stop = MenuItemImage::create("mainscence/home.png", "mainscence/home.png", [this](Ref *) {
		this->removeChild(controlMenu);
		_eventDispatcher->dispatchCustomEvent(Message::game_stop, nullptr);
		pauseButton->setVisible(true);
		pauseButton->setPosition(50, 50);
		pauseButton->setScale(0.6);
	});
	auto cancel = MenuItemImage::create("mainscence/start.png", "mainscence/start.png", [this](Ref *) {
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
	energyValue = energyValue >= 10 ? 10 : energyValue + 1;
	char name[20];
	sprintf(name, "energy_%d.png", energyValue);
	energy->setDisplayFrame(cache->getSpriteFrameByName(name));
}

void UiLayer::displayScore(EventCustom *event) {
	if (score)
		scoreBox->removeChild(score, true);

	score = LabelBMFont::create(std::to_string(DataVo::inst()->score),
		"fonts/number_2.fnt");
	scoreBox->addChild(score);
	score->setAnchorPoint(Point(1, 0.5f));
	auto x = scoreBox->getTextureRect().getMaxX() * scoreBox->getScale() - 10;
	auto y = scoreBox->getTextureRect().getMaxY() * scoreBox->getScale() / 2;
	score->setPosition(x, y);
}
