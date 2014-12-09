#include "RoleLayer.h"
#include "DataManager/DataVo.h"
#include "consts/Message.h"
#include <cstring>

bool RoleLayer::init() {
	if (!Layer::init()) return false;

	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("rolemove/rolemove.plist");
	return true;
}


void RoleLayer::onEnter() {
	Layer::onEnter();
	_eventDispatcher->addCustomEventListener(Message::next_timeslice,
		CC_CALLBACK_1(RoleLayer::run, this));
}

void RoleLayer::onExit() {
	_eventDispatcher->removeCustomEventListeners(Message::next_timeslice);
	Layer::onExit();
}

void RoleLayer::run(EventCustom *event) {
}
