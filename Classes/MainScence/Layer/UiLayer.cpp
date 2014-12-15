#include "UiLayer.h"
#include "consts\ResolutionConst.h"
#include "consts\Message.h"

bool UiLayer::init() {
	if (!Layer::init()) return false;

	auto pauseButtonItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
		CC_CALLBACK_1(UiLayer::showControlMenu, this));
	pauseButton = Menu::create(pauseButtonItem, nullptr);
	pauseButton->setPosition(50, designHeight-50);
	addChild(pauseButton, 1000);
	return true;
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