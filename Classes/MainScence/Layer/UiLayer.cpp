#include "UiLayer.h"
#include "consts\ResolutionConst.h"
#include "consts\Message.h"
#include "DataManager/DataVo.h"

bool UiLayer::init() {
	if (!Layer::init()) return false;

	auto pauseButtonItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
		CC_CALLBACK_1(UiLayer::showControlMenu, this));
	pauseButton = Menu::create(pauseButtonItem, nullptr);
	pauseButton->setPosition(50, designHeight-50);
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

	scheduleUpdate();
	
	return true;
}

void UiLayer::onEnter() {
	Layer::onEnter();
	_eventDispatcher->addCustomEventListener(Message::get_beer, CC_CALLBACK_1(UiLayer::energyUp, this));

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
	auto restart = MenuItemImage::create("restart.png", "restart.png", [this](Ref *) {
		this->removeChild(controlMenu);
		_eventDispatcher->dispatchCustomEvent(Message::game_restart, nullptr);
	});
	auto stop = MenuItemImage::create("stop.png", "stop.png", [this](Ref *) {
		this->removeChild(controlMenu);
		_eventDispatcher->dispatchCustomEvent(Message::game_stop, nullptr);
	});
	auto cancel = MenuItemImage::create("cancel.png", "cancel.png", [this](Ref *) {
		this->removeChild(controlMenu);
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		_eventDispatcher->dispatchCustomEvent(Message::game_resume, nullptr);
	});
	controlMenu = Menu::create(restart, stop, cancel, nullptr);
	controlMenu->setAnchorPoint(Point(0.5, 0.5));
	controlMenu->setPosition(_center);
	this->addChild(controlMenu, 1000);
	restart->setPosition(0, 50);
	stop->setPosition(0, 0);
	cancel->setPosition(0, -50);
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