#include "InputCenter.h"
#include "consts/Message.h"
#include "DataManager/DataVo.h"

bool InputCenter::init() {
	if (!Layer::init()) return false;
	this->setAccelerometerEnabled(true);

	ms = nullptr;
	touchStatus = 0;

	return true;
}

void InputCenter::onEnter() {
	Layer::onEnter();

	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(InputCenter::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(InputCenter::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(InputCenter::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void InputCenter::onExit() {
	_eventDispatcher->removeEventListener(listener);
	Layer::onExit();
}

bool InputCenter::onTouchBegan(Touch *touch, Event *event) {
	if (ms)
		return false;

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

	if ( d > 10000 ) {
		auto slope = y / x;

		if ( touchStatus == 0 ) {
			if ( slope < -2 || slope > 2 && y > 0 ) {
				_eventDispatcher->dispatchCustomEvent(Message::input_slide_up);
				lastInput = Message::input_slide_up;
			}
			else if ( slope >= -2 && slope <= -0.5 && y > 0 ) {
				_eventDispatcher->dispatchCustomEvent(Message::input_slide_up_left);
				lastInput = Message::input_slide_up_left;
			}
			else if ( slope > -0.5 && slope < 0.5 && x < 0 ) {
				_eventDispatcher->dispatchCustomEvent(Message::input_slide_left);
				lastInput = Message::input_slide_left;
			}
			else if ( slope > -0.5 && slope < 0.5 && x > 0 ) {
				_eventDispatcher->dispatchCustomEvent(Message::input_slide_right);
				lastInput = Message::input_slide_right;
			}
			else if ( slope >= 0.5 && slope <= 2 && y > 0) {
				_eventDispatcher->dispatchCustomEvent(Message::input_slide_up_right);
				lastInput = Message::input_slide_up_right;
			}

			touchStatus = 1;
		}
		else {
			if ( slope < -2 || slope > 2 && y > 0 && lastInput != Message::input_slide_up) {
				_eventDispatcher->dispatchCustomEvent(Message::input_touch_release);
				_eventDispatcher->dispatchCustomEvent(Message::input_slide_up);
				lastInput = Message::input_slide_up;
			}
			else if ( slope >= -2 && slope <= -0.5 && y > 0 && 
				lastInput != Message::input_slide_up_left && lastInput != Message::input_slide_left) {
				_eventDispatcher->dispatchCustomEvent(Message::input_touch_release);
				_eventDispatcher->dispatchCustomEvent(Message::input_slide_up_left);
				lastInput = Message::input_slide_up_left;
			}
			else if ( slope > -0.5 && slope < 0.5 && x < 0 && 
				lastInput != Message::input_slide_up_left && lastInput != Message::input_slide_left) {
				_eventDispatcher->dispatchCustomEvent(Message::input_touch_release);
				_eventDispatcher->dispatchCustomEvent(Message::input_slide_left);
				lastInput = Message::input_slide_left;
			}
			else if ( slope > -0.5 && slope < 0.5 && x > 0 &&
				lastInput != Message::input_slide_up_right && lastInput != Message::input_slide_right) {
				_eventDispatcher->dispatchCustomEvent(Message::input_touch_release);
				_eventDispatcher->dispatchCustomEvent(Message::input_slide_right);
				lastInput = Message::input_slide_right;
			}
			else if ( slope >= 0.5 && slope <= 2 && y > 0 &&
				lastInput != Message::input_slide_up_right && lastInput != Message::input_slide_right) {
				_eventDispatcher->dispatchCustomEvent(Message::input_touch_release);
				_eventDispatcher->dispatchCustomEvent(Message::input_slide_up_right);
				lastInput = Message::input_slide_up_right;
			}
		}
		start = center;
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
		lastInput = "";
		touchStatus = 0;
	}
	else 
		_eventDispatcher->dispatchCustomEvent(Message::input_click);
}