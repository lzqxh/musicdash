#include "InputCenter.h"
#include "consts/Message.h"
#include "DataManager/DataVo.h"

bool InputCenter::init() {
	if (!Layer::init()) return false;
	this->setAccelerometerEnabled(true);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(InputCenter::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(InputCenter::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(InputCenter::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	ms = nullptr;
	touchStatus = 0;

	return true;
}

void InputCenter::onEnter() {
	Layer::onEnter();
}

void InputCenter::onExit() {
	Layer::onExit();
}

bool InputCenter::onTouchBegan(Touch *touch, Event *event) {
	auto center = touch->getLocation();
	this->start = center;

	auto ms = MotionStreak::create(0.7, 2, 10, Color3B::WHITE, "touch.png");
	addChild(ms);
	ms->setPosition(center);
	this->ms = ms;

	return true;
}
void InputCenter::onTouchMoved(Touch *touch, Event *event) {
	auto center = touch->getLocation();
	ms->setPosition(center);

	auto x = center.x - start.x;
	auto y = center.y - start.y;
	auto d = x*x + y*y;

	if ( touchStatus == 0 ) {
		if ( d > 10000 ) {
			if ( abs(x) >= abs(y) && x < 0 )
				_eventDispatcher->dispatchCustomEvent(Message::input_slide_left);
			else if ( abs(x) >= abs(y) && x >=0 )
				_eventDispatcher->dispatchCustomEvent(Message::input_slide_right);
			else if ( abs(x) < abs(y) && y >= 0 )
				_eventDispatcher->dispatchCustomEvent(Message::input_slide_up);

			touchStatus = 1;
		}
	}
}
void InputCenter::onTouchEnded(Touch *touch, Event *event) {
	ms->runAction(Sequence::create(
		DelayTime::create(1),
		RemoveSelf::create(),
		nullptr));
	ms = nullptr;
	if ( touchStatus == 1 ) {
		_eventDispatcher->dispatchCustomEvent(Message::input_touch_release);
		touchStatus = 0;
	}
}