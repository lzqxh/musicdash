#include "ScoreLayer.h"
#include "consts/Message.h"
#include "consts/ResolutionConst.h"
#include "DataManager/DataVo.h"


bool ScoreLayer::init() {
	if (!Layer::init()) return false;
	return true;
}

void ScoreLayer::onEnter() {
	Layer::onEnter();
	auto l1 = EventListenerCustom::create(Message::score,
		[this](EventCustom *event){
			int ds = *static_cast<int *>(event->getUserData());
			float c;
			if (ds == 1)
				c = 0.9;
			else if (ds == 2)
				c = 1;
			else 
				c = 0;
			DataVo::inst()->score += (1000 + 10 * DataVo::inst()->combos) * c;
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(l1, this);
	auto l2 = EventListenerCustom::create(Message::score,
		CC_CALLBACK_1(ScoreLayer::displayCombos, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(l2, this);
}

void ScoreLayer::onExit() {
	Layer::onExit();
}

void ScoreLayer::displayCombos(EventCustom *event) {
	int score = * static_cast<int*>(event->getUserData());
	if (label) { removeChild(label); label = nullptr; }
	label = CCNode::create();
	LabelBMFont *l1 = nullptr, *l2 = nullptr, *l3 = nullptr;
//	auto l1 = LabelBMFont::create("C", "fonts/number_1.fnt");
//	l1->setScale(0.5f);
//	l1->setPosition(-100, 200);
//	lable->addChild(l1);

	std::string text1;
	if (score == -1) text1 = "M";
	else {
		text1 = score == 2 ? "P" : "G";
	}
	l2 = LabelBMFont::create(text1, "fonts/number_1.fnt");
	l2->setPosition(0, 100);
	label->addChild(l2);

	if (score != -1) {
		auto l3 = LabelBMFont::create(std::to_string(DataVo::inst()->combos), "fonts/number_1.fnt", 35);
		l3->setScale(0.7f);
		l3->setPosition(0, -10);
		label->addChild(l3);
	}

	label->setPosition(_center.x, _center.y + 250);
	this->addChild(label);
	auto scale = CCScaleTo::create(0.2f, 1.3f);
	auto delay = DelayTime::create(0.2f);
	auto remove = CallFunc::create([this](){
		if (label) { removeChild(label); label = nullptr; }
	});
	label->runAction(Sequence::create(scale, delay, remove, nullptr));
}
