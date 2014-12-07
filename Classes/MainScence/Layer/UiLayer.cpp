#include "UiLayer.h"
#include <cmath>


bool UiLayer::init() {
	if (!Layer::init()) return false;

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(UiLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(UiLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(UiLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	scheduleUpdate();
	ms = nullptr;

	return true;
}

bool UiLayer::onTouchBegan(Touch *touch, Event *event) {
	auto center = touch->getLocation();
	float actionTime = 0.2;
	/*
	float scale = 0.8;
	GLubyte opacity = 110;
	auto t1 = Sprite::create("touch_1.png");
	auto t2 = Sprite::create("touch_2.png");
	auto t3 = Sprite::create("touch_3.png");
	ts.push_back(t1);
	ts.push_back(t2);
	ts.push_back(t3);
	for( auto t : ts ) {
		addChild(t);
		t->setPosition(center);
		t->setScale(scale);
		t->runAction(RepeatForever::create(RotateBy::create(actionTime, 100)));
	}
	*/

	auto ms = MotionStreak::create(0.7, 2, 10, Color3B::WHITE, "bg.png");
	addChild(ms);
	ms->setPosition(center);
	this->ms = ms;
	return true;
}
void UiLayer::onTouchMoved(Touch *touch, Event *event) {
	auto center = touch->getLocation();
	for (auto t : ts)
		t->setPosition(center);
	ms->setPosition(center);
}
void UiLayer::onTouchEnded(Touch *touch, Event *event) {
	for (auto t : ts)
		removeChild(t);
	ts.clear();
	ms = nullptr;
}

void UiLayer::update(float dt) {
}
