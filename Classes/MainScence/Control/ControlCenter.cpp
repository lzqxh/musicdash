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

	_eventDispatcher->addCustomEventListener(Message::game_pause,
											 CC_CALLBACK_1(ControlCenter::gamePause, this));
	_eventDispatcher->addCustomEventListener(Message::game_resume,
											 CC_CALLBACK_1(ControlCenter::gameResume, this));
	_eventDispatcher->addCustomEventListener(Message::game_restart,
											 CC_CALLBACK_1(ControlCenter::gameStart, this));
	_eventDispatcher->addCustomEventListener(Message::game_stop,
											 CC_CALLBACK_1(ControlCenter::gameOver, this));
	CCLog("on Enter\n");
	//直接开始～
	this->_eventDispatcher->dispatchCustomEvent(Message::game_restart, nullptr);
}

void ControlCenter::onExit() {
	CCLog("on Exit\n");
	_eventDispatcher->removeCustomEventListeners(Message::game_stop);
	_eventDispatcher->removeCustomEventListeners(Message::game_restart);
	_eventDispatcher->removeCustomEventListeners(Message::game_resume);
	_eventDispatcher->removeCustomEventListeners(Message::game_pause);

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
		_eventDispatcher->dispatchCustomEvent(Message::get_beer, &index);
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

void ControlCenter::gameStart(EventCustom* e = nullptr) {
	for (curTime = -300; curTime < 0; curTime++)
		_eventDispatcher->dispatchCustomEvent(Message::next_timeslice, nullptr);
	auto cache = SpriteFrameCache::getInstance();
	auto sprite = Sprite::create();
	sprite->setPosition(ccp(_center.x, _center.y + 150));
	this->addChild(sprite);

	sprite->setDisplayFrame(cache->getSpriteFrameByName("count_3.png"));
	auto action = CCSequence::create(
		DelayTime::create(1.0f),
		CCCallFunc::create([sprite, cache](){
			sprite->setDisplayFrame(cache->getSpriteFrameByName("count_2.png"));
		}),
		DelayTime::create(1.0f),
		CCCallFunc::create([sprite, cache](){
			sprite->setDisplayFrame(cache->getSpriteFrameByName("count_1.png"));
		}),
		DelayTime::create(1.0f),
		CCCallFunc::create([sprite, cache](){
			sprite->setDisplayFrame(cache->getSpriteFrameByName("count_go.png"));
		}),
		DelayTime::create(0.3f),
		CCCallFunc::create([sprite, this](){
			this->removeChild(sprite);;
			_accTime = 0;
			DataVo::inst()->combos = 0;
			DataVo::inst()->score = 0;
			while (!inputQue.empty()) inputQue.pop();
			CocosDenshion::SimpleAudioEngine::getInstance()->
				playBackgroundMusic(DataVo::inst()->musicFile.c_str());
			gameStatus = gs_playing;
		}),
		nullptr
		);
	sprite->runAction(action);
}

void ControlCenter::gameOver(EventCustom* event = nullptr) {
	gameStatus = gs_over;
	CocosDenshion::SimpleAudioEngine::getInstance()->
		stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->
		playBackgroundMusic("loginscence/bg.mp3", true);
	auto cache = SpriteFrameCache::getInstance();
	auto bg = Sprite::create();
	bg->setDisplayFrame(cache->getSpriteFrameByName("gameover_bg.png"));
	bg->setPosition(ccp(_center.x, _center.y + 100));
	bg->setAnchorPoint(ccp(0.5, 0.5));
	bg->setScale(2.7f);

	int n = DataVo::inst()->score;
	auto scoreLabel = LabelTTF::create(std::to_string(n), "fonts/Marker Felt.ttf", 27);
	scoreLabel->setAnchorPoint(ccp(1.0, 0.5));
	scoreLabel->setPosition(215, 223);
	bg->addChild(scoreLabel, 10);

	auto button1 = MenuItemImage::create();
	button1->setNormalSpriteFrame(cache->getSpriteFrameByName("gameover_restart_1.png"));
	button1->setSelectedSpriteFrame(cache->getSpriteFrameByName("gameover_restart_2.png"));
	button1->setScale(2.7f);
	button1->setPosition(ccp(-170, 0));

	auto button2 = MenuItemImage::create();
	button2->setNormalSpriteFrame(cache->getSpriteFrameByName("gameover_back_1.png"));
	button2->setSelectedSpriteFrame(cache->getSpriteFrameByName("gameover_back_2.png"));
	button2->setScale(2.7f);
	button2->setPosition(ccp(0, 0));

	auto menu = Menu::create(button1, button2, nullptr);
	menu->setPosition(_center.x, 135);
	this->addChild(bg);
	this->addChild(menu);

	button1->setCallback([this, bg, menu](Ref *){
		this->removeChild(bg);
		this->removeChild(menu);
		this->_eventDispatcher->dispatchCustomEvent(Message::game_restart, nullptr);
	});
	button2->setCallback([&](Ref *){
		auto scene = SongSelectionScene::create();
		Director::getInstance()->replaceScene(scene);
	});
	return;
}

void ControlCenter::gamePause(EventCustom* event = nullptr) {
	gameStatus = gs_pause;
	CocosDenshion::SimpleAudioEngine::getInstance()->
		pauseBackgroundMusic();
	return;
}

void ControlCenter::gameResume(EventCustom* event = nullptr) {
	gameStatus = gs_playing;
	CocosDenshion::SimpleAudioEngine::getInstance()->
		resumeBackgroundMusic();
	return;
}

void ControlCenter::roleMove() {
	auto &data = DataVo::inst()->data;
	std::string lastInput = "";
	while (!inputQue.empty()) {
		if (inputQue.front().second == Message::input_touch_release) {
			if (roleStatus == Action_M2L || roleStatus == Action_M2R || 
				roleStatus == Sliding_L || roleStatus == Sliding_R) {
				lastInput = inputQue.front().second;
				break;
			}
		}
		if (curTime - inputQue.front().first > 25) inputQue.pop();
		else {
			lastInput = inputQue.front().second;
			break;
		}
	}
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
			DataVo::inst()->actionCount = 16;
			lastInput = "";
		}
		else if (lastInput == Message::input_slide_right) {
			roleStatus = RoleStatus::Action_M2R;
			DataVo::inst()->actionCount = 16;
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
        	if (upL || upR) {
        		roleStatus = upL ? RoleStatus::Action_M2UL : RoleStatus::Action_M2UR;
        		DataVo::inst()->actionCount = 14;
        	}
        	lastInput = "";
        }
        break;
	};

	if (lastInput == "" && !inputQue.empty()) {
		if (inputQue.front().second != Message::input_touch_release)
			CocosDenshion::SimpleAudioEngine::getInstance()->
				playEffect("input.wav");
		inputQue.pop();
	}
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
	inputQue.push(std::make_pair(curTime, event->getEventName()));
}

