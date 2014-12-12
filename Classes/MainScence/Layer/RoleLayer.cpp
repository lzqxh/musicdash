#include "RoleLayer.h"
#include "DataManager/DataVo.h"
#include "consts/Message.h"
#include "consts/ResolutionConst.h"

bool RoleLayer::init() {
	if (!Layer::init()) return false;

	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("rolemove/rolemove.plist");

	role = Sprite::create();
	role->setSpriteFrame(cache->getSpriteFrameByName("slide_0.png"));
	role->setPosition(designWidth/2, 200);
	role->setScale(0.4);
	addChild(role);

	frameName = "slide_0.png";
	eventCount = 0;
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

	auto cache = SpriteFrameCache::getInstance();
	static RoleStatus lastRoleStatus;
	if (roleStatus != lastRoleStatus) eventCount = 0;
	lastRoleStatus = roleStatus;
	switch(roleStatus) {
	case Sliding_M:
	case Sliding_L:
	case Sliding_R:
		if (++eventCount < 10)
			break;
		eventCount = 0;
		if (frameName == "slide_1.png" && rand() % 10 < 7) //有一定概率不蹬地
			frameName = "slide_0.png";
		else
			frameName = nextFrameName("slide");
		role->setDisplayFrame(cache->getSpriteFrameByName(frameName));
		break;
	case Action_M2L:
	case Action_R2M:
		if (++eventCount >= 10) {
			eventCount = 0;
			frameName = nextFrameName("left");
			role->setDisplayFrame(cache->getSpriteFrameByName(frameName));
		}
		role->setPosition(role->getPosition().x - 5, role->getPosition().y);
		break;
	case Action_M2R:
	case Action_L2M:
		if (++eventCount >= 10) {
			eventCount = 0;
			frameName = nextFrameName("right");
			role->setDisplayFrame(cache->getSpriteFrameByName(frameName));
		}
		role->setPosition(role->getPosition().x + 5, role->getPosition().y);
		break;
	case Action_JUMP:
		++eventCount;
		if (eventCount == 1)
			role->setDisplayFrame(cache->getSpriteFrameByName("up_0.png"));
		if (eventCount <= 10)
			role->setPosition(role->getPosition().x, role->getPosition().y + 15);
		else if (eventCount > 40 - 10)
			role->setPosition(role->getPosition().x, role->getPosition().y - 15);
		break;

	default:
		break;
	}
}

std::string RoleLayer::nextFrameName(std::string prefix) {
	char newFrameName[20];
	int frameSuf = atoi(frameName.substr(frameName.find('_')+1, 1).c_str());

	if (this->frameName.find(prefix) != std::string::npos) {
		if (prefix == "slide")
			++frameSuf %= 4;
		else if (prefix == "left" || prefix == "right")
			++frameSuf %= 3;
		sprintf(newFrameName, "%s_%d.png", prefix.c_str(), frameSuf);
	}
	else
		sprintf(newFrameName, "%s_1.png", prefix.c_str());
	return newFrameName;
}