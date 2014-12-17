#include "RoleLayer.h"
#include "DataManager/DataVo.h"
#include "consts/Message.h"
#include "consts/ResolutionConst.h"

bool RoleLayer::init() {
	if (!Layer::init()) return false;

	auto cache = SpriteFrameCache::getInstance();

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
	_eventDispatcher->addCustomEventListener(Message::score,
		CC_CALLBACK_1(RoleLayer::roleEffect, this));
}

void RoleLayer::onExit() {
	_eventDispatcher->removeCustomEventListeners(Message::next_timeslice);
	_eventDispatcher->removeCustomEventListeners(Message::score);
	Layer::onExit();
}

void RoleLayer::run(EventCustom *event) {

	auto cache = SpriteFrameCache::getInstance();
	static RoleStatus lastRoleStatus;
	Vec2 off;
	if (roleStatus != lastRoleStatus) eventCount = 0;
	lastRoleStatus = roleStatus;
	switch(roleStatus) {
	case Sliding_M:
		role->setPosition(designWidth/2, 200);
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
	case Sliding_UL:
	case Sliding_UR:
		role->setDisplayFrame(cache->getSpriteFrameByName("up_1.png"));
		break;
	case Action_M2UL:
	case Action_M2UR:
		role->setDisplayFrame(cache->getSpriteFrameByName("up_0.png"));
		off = roleStatus == Action_M2UL ? nextM2ULPosition(++eventCount) : 
									      nextM2URPosition(++eventCount);
		role->setPosition(role->getPosition() + off);
		break;
	case Action_UL2M:
	case Action_UR2M:
		role->setDisplayFrame(cache->getSpriteFrameByName("up_0.png"));
		off = roleStatus == Action_UL2M ? nextM2ULPosition(20 - eventCount++) : 
									      nextM2URPosition(20 - eventCount++);
		off *= -1;
		CCLog("off %.3f %.3f\n", off.x, off.y);
		role->setPosition(role->getPosition() + off);
		break;
	case Action_M2L:
	case Action_R2M:
		++eventCount;
		if (eventCount == 1 || eventCount == 4 || eventCount == 16 - 4) {
			frameName = nextFrameName("left");
			role->setDisplayFrame(cache->getSpriteFrameByName(frameName));
		}
		role->setPosition(role->getPosition().x - 8, role->getPosition().y);
		break;
	case Action_M2R:
	case Action_L2M:
		++eventCount;
		if (eventCount == 1 || eventCount == 4 || eventCount == 16 - 4) {
			frameName = nextFrameName("right");
			role->setDisplayFrame(cache->getSpriteFrameByName(frameName));
		}
		role->setPosition(role->getPosition().x + 8, role->getPosition().y);
		break;
	case Action_JUMP:
		++eventCount;
		if (eventCount == 1)
			role->setDisplayFrame(cache->getSpriteFrameByName("up_0.png"));
		if (eventCount <= 10)
			role->setPosition(role->getPosition().x, role->getPosition().y + 20);
		else if (eventCount > 35 - 10)
			role->setPosition(role->getPosition().x, role->getPosition().y - 20);
		break;

	default:
		break;
	}
}

Vec2 RoleLayer::nextM2ULPosition(int index) {
	return Vec2(-8.61f, 3.1f);
}
Vec2 RoleLayer::nextM2URPosition(int index) {
	return Vec2(11.61f, 3.1f);
}

void RoleLayer::roleEffect(EventCustom *event) {
	int score = * static_cast<int *>(event->getUserData());
	if (score == -1) {
		role->stopAllActions();
		role->setVisible(true);
		role->runAction(Blink::create(0.5, 2));

		auto shake = MoveBy::create(0.1, Vec2(-20, 0));
		auto scene = Director::getInstance()->getRunningScene();
		scene->stopAllActions();
		scene->setPosition(0, 0);
		scene->runAction(
			Sequence::create(
			shake, shake->reverse(), shake, shake->reverse(), nullptr));

		auto hit = Sprite::create("mainscence/hit.png");
		role->addChild(hit);
		hit->setScale(2);
		hit->setPosition(0, 800);
		hit->runAction(Sequence::create(
			DelayTime::create(0.5),
			RemoveSelf::create(),
			nullptr));
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