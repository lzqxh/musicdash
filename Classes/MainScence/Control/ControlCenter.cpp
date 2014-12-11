#include "ControlCenter.h"

#include "SimpleAudioEngine.h"

#include "InputCenter.h"
#include "MainScence\SongSelectionScene.h"
#include "DataManager\DataVo.h"

#include "consts\ResolutionConst.h"
#include "consts\MyConsts.h"
#include "consts\Message.h"

bool ControlCenter::init() {
	if (!CCNode::init()) return false;

	//test 即加载的歌词名字
	DataVo::inst()->load("songs/test");
	gameStatus = gs_pause;

	auto inputCenter= InputCenter::create();
	addChild(inputCenter);

	roleStatus = Sliding_M;

	auto pauseButtonItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
		CC_CALLBACK_1(ControlCenter::showControlMenu, this));
	pauseButton = Menu::create(pauseButtonItem, nullptr);
	pauseButton->setPosition(designWidth-50, designHeight-50);
	addChild(pauseButton, 1000);

	return true;
}

void ControlCenter::onEnter() {
	CCNode::onEnter();
	this->schedule(schedule_selector(ControlCenter::fixedUpdate), 0.016);
	_eventDispatcher->addCustomEventListener(Message::input_slide_up,
											 CC_CALLBACK_1(ControlCenter::receiveInput, this));
	_eventDispatcher->addCustomEventListener(Message::input_slide_left,
											 CC_CALLBACK_1(ControlCenter::receiveInput, this));
	_eventDispatcher->addCustomEventListener(Message::input_slide_right,
											 CC_CALLBACK_1(ControlCenter::receiveInput, this));
	_eventDispatcher->addCustomEventListener(Message::input_touch_release,
											 CC_CALLBACK_1(ControlCenter::receiveInput, this));

	CCLog("on Enter\n");
	//直接开始～
	gameStart();
}

void ControlCenter::onExit() {
	CCLog("on Exit\n");
	_eventDispatcher->removeCustomEventListeners(Message::input_touch_release);
	_eventDispatcher->removeCustomEventListeners(Message::input_slide_left);
	_eventDispatcher->removeCustomEventListeners(Message::input_slide_right);
	_eventDispatcher->removeCustomEventListeners(Message::input_slide_up);
	this->unschedule(schedule_selector(ControlCenter::fixedUpdate));
	CCNode::onExit();
}


void ControlCenter::evalution() {
	int score = 0;
	_eventDispatcher->dispatchCustomEvent(Message::score, &score);
}

void ControlCenter::gameStart() {
	curTime = 0;
	_accTime = 0;
	CocosDenshion::SimpleAudioEngine::getInstance()->
		playBackgroundMusic(DataVo::inst()->musicFile.c_str());
	gameStatus = gs_playing;
}

void ControlCenter::gameOver() {
	gameStatus = gs_over;
	CocosDenshion::SimpleAudioEngine::getInstance()->
		stopBackgroundMusic();
	auto scene = SongSelectionScene::create();
	Director::getInstance()->replaceScene(scene);
	return;
}

void ControlCenter::roleMove() {
	switch (roleStatus) {
	case Action_L2M:
	case Action_R2M:
		if (--DataVo::inst()->actionCount == 0) roleStatus = RoleStatus::Sliding_M;
		break;
	case Action_M2L:
		if (--DataVo::inst()->actionCount == 0) roleStatus = RoleStatus::Sliding_L;
		break;
	case Action_M2R:
		if (--DataVo::inst()->actionCount == 0) roleStatus = RoleStatus::Sliding_R;
		break;
	case Action_JUMP:
		if (--DataVo::inst()->actionCount == 0) roleStatus = RoleStatus::Sliding_M;
		break;
	case Sliding_U:
		if (DataVo::inst()->data[curTime][2] == 0) {
			roleStatus = RoleStatus::Action_U2M;
			DataVo::inst()->actionCount = 20;
		}
		break;
	case Sliding_L:
		if (lastInput == Message::input_touch_release) {
			roleStatus = RoleStatus::Action_L2M;
			DataVo::inst()->actionCount = 20;
			lastInput = "";
		}
		break;
	case Sliding_R:
		if (lastInput == Message::input_touch_release) {
			roleStatus = RoleStatus::Action_R2M;
			DataVo::inst()->actionCount = 20;
			lastInput = "";
		}
		break;
	case Sliding_M:
		if (lastInput == Message::input_slide_left) {
			roleStatus = RoleStatus::Action_M2L;
			DataVo::inst()->actionCount = 20;
			lastInput = "";
		}
		else if (lastInput == Message::input_slide_right) {
			roleStatus = RoleStatus::Action_M2R;
			DataVo::inst()->actionCount = 20;
			lastInput = "";
		}
		else if (lastInput == Message::input_slide_up) {
			roleStatus = RoleStatus::Action_JUMP;
			DataVo::inst()->actionCount = 40;
			lastInput = "";
		}
		break;
	};
}
void ControlCenter::fixedUpdate(float dt) {
	if (gameStatus != gs_playing) return;
	_accTime += dt * 1000;
	while ((curTime + 1) * TIMESLICE_SIZE < _accTime) {
		curTime ++;
		if (curTime >= DataVo::inst()->musicLength) {
			gameOver();
			break;
		}
		roleMove();
		evalution();
		if (DataVo::inst()->data[curTime][8] == 1) gameStatus = gs_pause;
		_eventDispatcher->dispatchCustomEvent(Message::next_timeslice, nullptr);
	}
}


void ControlCenter::receiveInput(EventCustom *event) {
	lastInput = event->getEventName();
}

void ControlCenter::showControlMenu(Ref *pSender) {
	this->gameStatus = gs_pause;
	Director::getInstance()->pause();
	auto restart = MenuItemImage::create("restart.png", "restart.png", [this](Ref *) {
		this->removeChild(controlMenu);
		Director::getInstance()->resume();
		this->gameOver();
		this->gameStart();
	});
	auto stop = MenuItemImage::create("stop.png", "stop.png", [this](Ref *) {
		this->gameOver();
		Director::getInstance()->resume();
		Director::getInstance()->popScene();
	});
	auto cancel = MenuItemImage::create("cancel.png", "cancel.png", [this](Ref *) {
		this->removeChild(controlMenu);
		Director::getInstance()->resume();
		this->gameStatus = gs_playing;
	});
	controlMenu = Menu::create(restart, stop, cancel, nullptr);
//	controlMenu->ignoreAnchorPointForPosition(false);
//	controlMenu->setAnchorPoint(Point(0.5, 0.5));
	this->addChild(controlMenu, 1000);
	restart->setPosition(0, 50);
	stop->setPosition(0, 0);
	cancel->setPosition(0, -50);
}
