#include "ControlCenter.h"

#include "SimpleAudioEngine.h"

#include "InputCenter.h"
#include "MainScence\SongSelectionScene.h"
#include "DataManager\DataVo.h"
#include "MainScence\GameWorld.h"

#include "consts\ResolutionConst.h"
#include "consts\MyConsts.h"
#include "consts\Message.h"
#include "..\OptionsScene.h"
#include "..\Layer\ScoreLayer.h"

bool ControlCenter::init() {
	if (!CCNode::init()) return false;
	gameStatus = gs_over;
	auto inputCenter= InputCenter::create();
	addChild(inputCenter);
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
//	_eventDispatcher->addCustomEventListener(Message::input_click,
//											 CC_CALLBACK_1(ControlCenter::receiveInput, this));
	_eventDispatcher->addCustomEventListener(Message::input_slide_up_left,
											 CC_CALLBACK_1(ControlCenter::receiveInput, this));
	_eventDispatcher->addCustomEventListener(Message::input_slide_up_right,
											 CC_CALLBACK_1(ControlCenter::receiveInput, this));

	_eventDispatcher->addCustomEventListener(Message::game_pause,
											 CC_CALLBACK_1(ControlCenter::gamePause, this));
	_eventDispatcher->addCustomEventListener(Message::game_resume,
											 CC_CALLBACK_1(ControlCenter::gameResume, this));
	_eventDispatcher->addCustomEventListener(Message::game_restart,
											 CC_CALLBACK_1(ControlCenter::gameRestart, this));
	_eventDispatcher->addCustomEventListener(Message::game_stop,
											 CC_CALLBACK_1(ControlCenter::gameOver, this));

	_eventDispatcher->addCustomEventListener(Message::beer_click,
											 CC_CALLBACK_1(ControlCenter::clickBeer, this));
	CCLog("on Enter\n");
	//直接开始～
//	this->_eventDispatcher->dispatchCustomEvent(Message::game_restart, nullptr);
	gameStart(nullptr);
}

void ControlCenter::onExit() {
	CCLog("on Exit\n");
	_eventDispatcher->removeCustomEventListeners(Message::game_stop);
	_eventDispatcher->removeCustomEventListeners(Message::game_restart);
	_eventDispatcher->removeCustomEventListeners(Message::game_resume);
	_eventDispatcher->removeCustomEventListeners(Message::game_pause);

//	_eventDispatcher->removeCustomEventListeners(Message::input_click);
	_eventDispatcher->removeCustomEventListeners(Message::input_touch_release);
	_eventDispatcher->removeCustomEventListeners(Message::input_slide_left);
	_eventDispatcher->removeCustomEventListeners(Message::input_slide_right);
	_eventDispatcher->removeCustomEventListeners(Message::input_slide_up);
	_eventDispatcher->removeCustomEventListeners(Message::input_slide_up_left);
	_eventDispatcher->removeCustomEventListeners(Message::input_slide_up_right);

	_eventDispatcher->removeCustomEventListeners(Message::beer_click);
	this->unschedule(schedule_selector(ControlCenter::fixedUpdate));
	CCNode::onExit();
}


void ControlCenter::staticScore(int score) {
	if (score != 0) {
		if (score == -1) {
			DataVo::inst()->combos = 0;
			DataVo::inst()->speedX = 0;
		}
		else {
			DataVo::inst()->combos++;
			DataVo::inst()->speedX = 10;
//			DataVo::inst()->coins += 1;
		}
		_eventDispatcher->dispatchCustomEvent(Message::score, &score);
		_eventDispatcher->dispatchCustomEvent(Message::disp_score, nullptr);
	}
}

void ControlCenter::staticDis() {
	static float accdis = 0;
	accdis += (DataVo::inst()->speed + DataVo::inst()->speedX) * 1.0 * TIMESLICE_SIZE / 1000 * 15;
	DataVo::inst()->distance += (int)accdis;
	accdis -= (int)accdis;
	_eventDispatcher->dispatchCustomEvent(Message::disp_score, nullptr);
}

int ControlCenter::atLeft() {
	switch (roleStatus)
	{
	case Action_M2L: 
		return 2;
	case Action_L2M:
	case Sliding_L: 
		return 1;
	default:
		return -1;
	}
}
int ControlCenter::atRight() {
	switch (roleStatus)
	{
	case Action_M2R: 
		return 2;
	case Action_R2M:
	case Sliding_R: 
		return 1;
	default:
		return -1;
	}
}
int ControlCenter::atRailL() {
	switch (roleStatus) {
	case Sliding_UL:
	case Action_M2UL:
		return 2;
	case Action_UL2M:
		return 1;
	default:
		return -1;
	}
}
int ControlCenter::atRailR() {
	switch (roleStatus) {
	case Sliding_UR:
	case Action_M2UR:
		return 2;
	case Action_UR2M:
		return 1;
	default:
		return -1;
	}
}

int ControlCenter::atJumping() {
	if (roleStatus == Action_JUMP) {
		if (DataVo::inst()->actionCount < 15) return 1;
		else return 2;
	}
	return -1;
}

void ControlCenter::checkTrafficCone(int index) {
	auto &data = DataVo::inst()->data[index];
	int score = 0;
	if (data[0] == 1) score = atRight();
	if (data[1] == 1) score = atLeft();
	staticScore(score);
}

void ControlCenter::checkBeer(int index) {
	auto &data = DataVo::inst()->data[index];
	int score = 0;
	if (data[2] == 1) score = atLeft();
	if (data[3] == 1) score = atRight();
	if (score > 0) {
		_eventDispatcher->dispatchCustomEvent(Message::explode, &index);
		_eventDispatcher->dispatchCustomEvent(Message::beer_get, &index);
	}
	staticScore(score);
}
void ControlCenter::checkBarrier(int index) {
	auto &data = DataVo::inst()->data[index];
	int numbers = 200 / TIMESLICE_SIZE, score = 0;
	if (data[4] % numbers == 1) score = atRight();
	if (data[5] % numbers == 1) score = atLeft();
	if (score == 1) score = 2;
	staticScore(score);
}

void ControlCenter::checkRailing(int index) {
	auto &data = DataVo::inst()->data[index];
	int numbers = 200 / TIMESLICE_SIZE, score = 0;
	if (data[6] % numbers == 1) score = atRailL();
	if (data[7] % numbers == 1) score = atRailR();
	staticScore(score);
}

void ControlCenter::checkManholeCover(int index) {
	auto &data = DataVo::inst()->data[index];
	int score = 0;
	if (data[8]  == 1) score = atJumping();
	staticScore(score);
}

void ControlCenter::evalution() {
	int score = 0;
	int index = curTime;
	if (index >= DataVo::inst()->musicLength) return;
	if (index < evaluateLim) return;

	checkTrafficCone(curTime);
	checkBeer(curTime);
	checkBarrier(curTime);
	checkRailing(curTime);
	checkManholeCover(curTime);
}

void ControlCenter::gameStart(EventCustom* e = nullptr) {
	CocosDenshion::SimpleAudioEngine::getInstance()->
		stopBackgroundMusic();
	roleStatus = Sliding_M;
	DataVo::inst()->combos = 0;
	DataVo::inst()->distance = 0;
	DataVo::inst()->energyValue = 0;
	DataVo::inst()->speed = 50;
	DataVo::inst()->speedX = 0;
	DataVo::inst()->isBeerEffectStart = false;
	ScoreLayer::score = 0;
	for (curTime = -138; curTime < 0; curTime++) {
		_eventDispatcher->dispatchCustomEvent(Message::next_timeslice, nullptr);
		int index = curTime + 138;
		if (index >= 0) _eventDispatcher->dispatchCustomEvent(Message::next_note, &index);
	}
	_eventDispatcher->dispatchCustomEvent(Message::beer_clear, nullptr);
	_eventDispatcher->dispatchCustomEvent(Message::disp_score, nullptr);
	auto cache = SpriteFrameCache::getInstance();
	_countSprite = Sprite::create();
	_countSprite->setPosition(ccp(_center.x, _center.y + 150));
	this->addChild(_countSprite);

	_countSprite->setDisplayFrame(cache->getSpriteFrameByName("count_3.png"));
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("soundeffect/321.mp3");

	gameStatus = gs_counting;
	auto action = CCSequence::create(
		DelayTime::create(1.0f),
		CCCallFunc::create([cache, this](){
			_countSprite->setDisplayFrame(cache->getSpriteFrameByName("count_2.png"));
		}),
		DelayTime::create(1.0f),
		CCCallFunc::create([this, cache](){
			_countSprite->setDisplayFrame(cache->getSpriteFrameByName("count_1.png"));
		}),
		DelayTime::create(1.0f),
		CCCallFunc::create([this, cache](){
			_countSprite->setDisplayFrame(cache->getSpriteFrameByName("count_go.png"));
		}),
		DelayTime::create(0.3f),
		CCCallFunc::create([this](){
			this->removeChild(_countSprite);;
			_countSprite = nullptr;
			_accTime = 0;
			while (!inputQue.empty()) inputQue.pop();
			CocosDenshion::SimpleAudioEngine::getInstance()->
				playBackgroundMusic(DataVo::inst()->musicFile.c_str());
			gameStatus = gs_playing;
		}),
		nullptr
	);
	_countSprite->runAction(action);
}

void ControlCenter::gameOver(EventCustom* event = nullptr) {
	gameStatus = gs_over;
	DataVo::inst()->updateRecord();
}

void ControlCenter::gameRestart(EventCustom* event = nullptr) {
	auto scene = GameWorld::create();
	Director::getInstance()->replaceScene(scene);
}

void ControlCenter::gamePause(EventCustom* event = nullptr) {
	if (gameStatus	== gs_counting) {
		gameStatus = gs_over;
		removeChild(_countSprite);
		_countSprite = nullptr;
	}
	else gameStatus = gs_pause;
	CocosDenshion::SimpleAudioEngine::getInstance()->
		pauseBackgroundMusic();
	return;
}

void ControlCenter::gameResume(EventCustom* event = nullptr) {
	if (gameStatus == gs_pause) {
		gameStatus = gs_playing;
		CocosDenshion::SimpleAudioEngine::getInstance()->
			resumeBackgroundMusic();
	}
	else if (gameStatus == gs_over) gameStart();
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
		//if (data[curTime][6] == 0) {
		if (lastInput == Message::input_touch_release) {
			roleStatus = RoleStatus::Action_UL2M;
			DataVo::inst()->actionCount = 14;
			lastInput = "";
		}
		else if (data[curTime][6] == 0) {
			roleStatus = RoleStatus::Sliding_L;
		}
		break;
	case Sliding_UR:
		//if (data[curTime][7] == 0) {
		if (lastInput == Message::input_touch_release) {
			roleStatus = RoleStatus::Action_UR2M;
			DataVo::inst()->actionCount = 14;
			lastInput = "";
		}
		else if (data[curTime][7] == 0) {
			roleStatus = RoleStatus::Sliding_R;
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
        else if (lastInput == Message::input_slide_up_left) {
        	bool upL = false;
        	for (int i = curTime; i < curTime + 50 && i < data.size(); i++)
        		if (data[i][6]) upL = true;
        	if (upL) {
        		roleStatus = RoleStatus::Action_M2UL;
        		DataVo::inst()->actionCount = 25;
        	}
			lastInput = "";
        }
		else if (lastInput == Message::input_slide_up_right) {
			bool upR = false;
			for (int i = curTime; i < curTime+50 && i < data.size(); i++)
				if (data[i][7]) upR = true;
			if (upR) {
				roleStatus = RoleStatus::Action_M2UR;
				DataVo::inst()->actionCount = 25;
			}
			lastInput = "";
//			else {
//				roleStatus = RoleStatus::Action_JUMP;
//				DataVo::inst()->actionCount = 35;
//			}
//			lastInput = "";
		}
        break;
	};

	if (lastInput == "" && !inputQue.empty()) {
		if (inputQue.front().second != Message::input_touch_release)
			if (OptionsScene::ifSound)
				CocosDenshion::SimpleAudioEngine::getInstance()->
					playEffect("soundeffect/input.wav");
		inputQue.pop();
	}
}
void ControlCenter::fixedUpdate(float dt) {
	if (gameStatus != gs_playing) return;
	_accTime += dt * 1000;
	static float _accRun = 0;
	float rate = 1.0;
	if (DataVo::inst()->isBeerEffectStart) rate = 1/1.5;
	while ((curTime + 1) * TIMESLICE_SIZE < _accTime) {
		curTime ++;
		if (curTime >= DataVo::inst()->musicLength) {
			_eventDispatcher->dispatchCustomEvent(Message::game_stop, nullptr);
			break;
		}
		roleMove();
		evalution();
		_eventDispatcher->dispatchCustomEvent(Message::next_roleaction, nullptr);
		_accRun += TIMESLICE_SIZE;
		while (_accRun >= TIMESLICE_SIZE * rate) {
			_eventDispatcher->dispatchCustomEvent(Message::next_timeslice, nullptr);
			_accRun -= TIMESLICE_SIZE * rate;
		}
		int index = curTime + 138 * rate;
		_eventDispatcher->dispatchCustomEvent(Message::next_note, &index);
		if (curTime % 15 == 0) 
			staticDis();
	}
}


void ControlCenter::receiveInput(EventCustom *event) {
	inputQue.push(std::make_pair(curTime, event->getEventName()));
}

void ControlCenter::beerEffectStart() {
	DataVo::inst()->isBeerEffectStart = true;
	evaluateLim = curTime + 138 * 1/1.5;
	_eventDispatcher->dispatchCustomEvent(Message::beer_effect_start, this);
	this->runAction( Sequence::create(
		DelayTime::create(10),
		CCCallFunc::create([this](){
			beerEffectStop();
		}),
		nullptr
		));
}
void ControlCenter::beerEffectStop() {
	DataVo::inst()->isBeerEffectStart = false;
	evaluateLim = curTime + 138;
	_eventDispatcher->dispatchCustomEvent(Message::beer_effect_stop, this);
}

void ControlCenter::clickBeer(EventCustom *event) {
	if (DataVo::inst()->energyValue != 10) return;
	DataVo::inst()->energyValue = 0;
	beerEffectStart();
}