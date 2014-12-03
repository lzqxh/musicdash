#include "ScoreLayer.h"

bool ScoreLayer::init() {
	if (!Layer::init()) return false;
	return true;
}

void ScoreLayer::onEnter() {
	Layer::onEnter();
//	auto l1 = EventListenerCustom::create(Message::score,
//		CC_CALLBACK_1(ScoreLayer::displayScore, this));
//	_eventDispatcher->addEventListenerWithSceneGraphPriority(l1, this);
}

void ScoreLayer::onExit() {
	Layer::onExit();
}

//void ScoreLayer::displayScore(EventCustom *event) {
//	int score = * static_cast<int*>(event->getUserData());
//	std::string text1;
//	std::string text2;
//	if (score == 0) text1 = "MISS";
//	else {
//		text1 = score == 2 ? "PREFECT" : "GOOD";
//		text2 = "Combos " + std::to_string(DataVO::inst()->combos);
//	}
//	if (label1) { removeChild(label1); label1 = nullptr; }
//	if (label2) { removeChild(label2); label2 = nullptr; }
//	label1 = LabelTTF::create(text1, "fonts/Marker Felt.ttf", 35);
//	label1->setPosition(_center.x, _center.y + 150);
//	this->addChild(label1);
//	auto scale = CCScaleTo::create(0.2f, 1.3f);
//	auto delay = DelayTime::create(0.2f);
//	auto remove = CallFunc::create([this](){
//		if (label1) { removeChild(label1); label1 = nullptr; }
//		if (label2) { removeChild(label2); label2 = nullptr; }
//	});
//	if (text2 != "") {
//		label2 = LabelTTF::create(text2, "fonts/Marker Felt.ttf", 27);
//		label2->setPosition(_center.x, _center.y + 100);
//		this->addChild(label2);
//		label2->runAction(scale->clone());
//	}
//	label1->runAction(Sequence::create(scale, delay, remove, nullptr));
//}