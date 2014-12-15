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
	pauseButton->setPosition(50, designHeight-50);
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
	_eventDispatcher->addCustomEventListener(Message::input_click,
											 CC_CALLBACK_1(ControlCenter::receiveInput, this));

	CCLog("on Enter\n");
	//直接开始～
	gameStart();
}

void ControlCenter::onExit() {
	CCLog("on Exit\n");
	_eventDispatcher->removeCustomEventListeners(Message::input_click);
	_eventDispatcher->removeCustomEventListeners(Message::input_touch_release);
	_eventDispatcher->removeCustomEventListeners(Message::input_slide_left);
	_eventDispatcher->removeCustomEventListeners(Message::input_slide_right);
	_eventDispatcher->removeCustomEventListeners(Message::input_slide_up);
	this->unschedule(schedule_selector(ControlCenter::fixedUpdate));
	CCNode::onExit();
}


void ControlCenter::staticScore(int score) {
	if (score != 0) {
		if (score == -1)
			DataVo::inst()->combos = 0;
		else 
			DataVo::inst()->combos++;
		_eventDispatcher->dispatchCustomEvent(Message::score, &score);
	}
}

bool ControlCenter::atLeft() {
	return (roleStatus == Sliding_L || roleStatus == Action_M2L || roleStatus == Action_L2M);
}
bool ControlCenter::atRight() {
	return (roleStatus == Sliding_R || roleStatus == Action_M2R || roleStatus == Action_L2M);
}
bool ControlCenter::atRail() {
	return (roleStatus == Sliding_UL || roleStatus == Action_M2UL || roleStatus == Action_UL2M ||
		    roleStatus == Sliding_UR || roleStatus == Action_M2UR || roleStatus == Action_UR2M);
}
bool ControlCenter::atJumping() {
	return roleStatus == Action_JUMP;
}

void ControlCenter::checkTrafficCone(int index) {
	auto &data = DataVo::inst()->data[index];
	int score = 0;
	if (data[0] == 1) score = atRight() ? 1 : -1;
	if (data[1] == 1) score = atLeft() ? 1 : -1;
	staticScore(score);
}

void ControlCenter::checkBeer(int index) {
	auto &data = DataVo::inst()->data[index];
	int score = 0;
	if (data[2] == 1) score = atLeft() ? 1 : -1;
	if (data[3] == 1) score = atRight() ? 1 : -1;
	if (score == 1) {
//		_eventDispatcher->dispatchCustomEvent(Message::disp_effect, nullptr);
		_eventDispatcher->dispatchCustomEvent(Message::explode, &index);
	}
	staticScore(score);
}
void ControlCenter::checkBarrier(int index) {
	auto &data = DataVo::inst()->data[index];
	int numbers = 200 / TIMESLICE_SIZE, score = 0;
	if (data[4] % numbers == 1) score = atRight() ? 1 : -1;
	if (data[5] % numbers == 1) score = atLeft() ? 1 : -1;
	staticScore(score);
}

void ControlCenter::checkRailing(int index) {
	auto &data = DataVo::inst()->data[index];
	int numbers = 200 / TIMESLICE_SIZE, score = 0;
	if (data[6] % numbers == 1) score = atRail() ? 1 : -1;
	if (data[7] % numbers == 1) score = atRail() ? 1 : -1;
	staticScore(score);
}

void ControlCenter::checkManholeCover(int index) {
	auto &data = DataVo::inst()->data[index];
	int score = 0;
	if (data[8]  == 1) score = atJumping() ? 1 : -1;
	staticScore(score);
}

void ControlCenter::evalution() {
	int score = 0;
	int index = curTime;
	if (index >= DataVo::inst()->musicLength) return;

	checkTrafficCone(curTime);
	checkBeer(curTime);
	checkBarrier(curTime);
	checkRailing(curTime);
	checkManholeCover(curTime);
}

void ControlCenter::gameStart() {
	curTime = 0;
	_accTime = 0;
	DataVo::inst()->combos = 0;
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
	auto &data = DataVo::inst()->data;
	switch (roleStatus) {
	case Action_L2M:
	case Action_R2M:
	case Action_UL2M:
	case Action_UR2M:
		if (--DataVo::inst()->actionCount == 0) roleStatus = RoleStatus::Sliding_M;
		break;
	case Action_M2L:
		if (--DataVo::inst()->actionCount == 0) roleStatus = RoleStatus::Sliding_L;
		break;
	case Action_M2R:
		if (--DataVo::inst()->actionCount == 0) roleStatus = RoleStatus::Sliding_R;
		break;
	case Action_M2UL:
		if (--DataVo::inst()->actionCount == 0) roleStatus = RoleStatus::Sliding_UL;
		break;
	case Action_M2UR:
		if (--DataVo::inst()->actionCount == 0) roleStatus = RoleStatus::Sliding_UR;
		break;
	case Action_JUMP:
		if (--DataVo::inst()->actionCount == 0) roleStatus = RoleStatus::Sliding_M;
		break;
	case Sliding_L:
		if (lastInput == Message::input_touch_release) {
			roleStatus = RoleStatus::Action_L2M;
			DataVo::inst()->actionCount = 18;
			lastInput = "";
		}
		break;
	case Sliding_R:
		if (lastInput == Message::input_touch_release) {
			roleStatus = RoleStatus::Action_R2M;
			DataVo::inst()->actionCount = 18;
			lastInput = "";
		}
		break;
	case Sliding_UL:
		if (data[curTime][6] == 0) {
			roleStatus = RoleStatus::Action_UL2M;
			DataVo::inst()->actionCount = 14;
			lastInput = "";
		}
		break;
	case Sliding_UR:
		if (data[curTime][7] == 0) {
			roleStatus = RoleStatus::Action_UR2M;
			DataVo::inst()->actionCount = 14;
			lastInput = "";
		}
		break;
	case Sliding_M:
		if (lastInput == Message::input_slide_left) {
			roleStatus = RoleStatus::Action_M2L;
			DataVo::inst()->actionCount = 18;
			lastInput = "";
		}
		else if (lastInput == Message::input_slide_right) {
			roleStatus = RoleStatus::Action_M2R;
			DataVo::inst()->actionCount = 18;
			lastInput = "";
		}
		else if (lastInput == Message::input_slide_up) {
			roleStatus = RoleStatus::Action_JUMP;
			DataVo::inst()->actionCount = 35;
			lastInput = "";
		}
        else if (lastInput == Message::input_click) {
        	bool upL = false, upR = false;
        	for (int i = curTime; i < curTime + 10 && i < data.size(); i++) {
        		if (data[i][6]) upL = true;
        		if (data[i][7]) upR = true;
        	}
			CCLog("%d %d\n", upL, upR);
        	if (upL || upR) {
        		roleStatus = upL ? RoleStatus::Action_M2UL : RoleStatus::Action_M2UR;
        		DataVo::inst()->actionCount = 14;
        	}
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
		_eventDispatcher->dispatchCustomEvent(Message::next_timeslice, nullptr);
	}
}


void ControlCenter::receiveInput(EventCustom *event) {
	lastInput = event->getEventName();
}

void ControlCenter::showControlMenu(Ref *pSender) {
	this->gameStatus = gs_pause;
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	auto restart = MenuItemImage::create("restart.png", "restart.png", [this](Ref *) {
		this->removeChild(controlMenu);
		this->gameStart();
	});
	auto stop = MenuItemImage::create("stop.png", "stop.png", [this](Ref *) {
		this->gameOver();
	});
	auto cancel = MenuItemImage::create("cancel.png", "cancel.png", [this](Ref *) {
		this->removeChild(controlMenu);
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		this->gameStatus = gs_playing;
	});
	controlMenu = Menu::create(restart, stop, cancel, nullptr);
	controlMenu->setAnchorPoint(Point(0.5, 0.5));
	controlMenu->setPosition(_center);
	this->addChild(controlMenu, 1000);
	restart->setPosition(0, 50);
	stop->setPosition(0, 0);
	cancel->setPosition(0, -50);
}
