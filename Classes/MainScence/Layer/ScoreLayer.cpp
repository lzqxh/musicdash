#include "ScoreLayer.h"
#include "consts/Message.h"
#include "consts/ResolutionConst.h"
#include "DataManager/DataVo.h"

bool ScoreLayer::init() {
	if (!Layer::init()) return false;
	score = 0;
	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("numbers/score.plist");

	return true;
}

void ScoreLayer::onEnter() {
	Layer::onEnter();
	auto l1 = EventListenerCustom::create(Message::score,
		CC_CALLBACK_1(ScoreLayer::displayScore, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(l1, this);
}

void ScoreLayer::onExit() {
	Layer::onExit();
}

/*
void ScoreLayer::displayScore(EventCustom *event) {
	int score = * static_cast<int*>(event->getUserData());
	std::string text1;
	std::string text2;
	if (score == 0) text1 = "MISS";
	else {
		text1 = score == 2 ? "PREFECT" : "GOOD";
		text2 = "Combos " + std::to_string(DataVo::inst()->combos);
	}
	if (label) { removeChild(label); label = nullptr; }
	if (label2) { removeChild(label2); label2 = nullptr; }
	label = LabelTTF::create(text1, "fonts/Marker Felt.ttf", 35);
	label->setPosition(_center.x, _center.y + 150);
	this->addChild(label);
	auto scale = CCScaleTo::create(0.2f, 1.3f);
	auto delay = DelayTime::create(0.2f);
	auto remove = CallFunc::create([this](){
		if (label) { removeChild(label); label = nullptr; }
		if (label2) { removeChild(label2); label2 = nullptr; }
	});
	if (text2 != "") {
		label2 = LabelTTF::create(text2, "fonts/Marker Felt.ttf", 27);
		label2->setPosition(_center.x, _center.y + 100);
		this->addChild(label2);
		label2->runAction(scale->clone());
	}
	label->runAction(Sequence::create(scale, delay, remove, nullptr));
}
*/

void ScoreLayer::displayScore(EventCustom *event) {
	int ds = *static_cast<int *>(event->getUserData());
	score += ds;
	int n = score;
	for (auto sprite : scoreSprite)
		removeChild(sprite);
	scoreSprite.clear();

	auto cache = SpriteFrameCache::getInstance();
	if (n <= 0) {
		auto sprite = Sprite::create();
		sprite->setDisplayFrame(cache->getSpriteFrameByName("score_0.png"));
		scoreSprite.push_back(sprite);
	}
	else {
		while(n > 0) {
			int m = n % 10;
			n /= 10;
			auto sprite = Sprite::create();
			char name[20];
			sprintf(name, "score_%d.png", m);
			sprite->setDisplayFrame(cache->getSpriteFrameByName(name));
			scoreSprite.push_back(sprite);
		}
	}
	float offsetX = 0;
	float offsetY = 50;
	for (auto sprite : scoreSprite) {
		sprite->setScale(0.3);
		auto width =  sprite->getSpriteFrame()->getOriginalSize().width;
		offsetX += sprite->getScale() * width;
		sprite->setPosition(designWidth-offsetX, designHeight-offsetY);
		addChild(sprite);
	}
}
