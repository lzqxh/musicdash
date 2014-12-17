#include "ScoreLayer.h"
#include "consts/Message.h"
#include "consts/ResolutionConst.h"
#include "DataManager/DataVo.h"

int ScoreLayer::score = 0;

bool ScoreLayer::init() {
	if (!Layer::init()) return false;
//	label = LabelBMFont::create("!\"#$%&'()*+,", "numbers/score-export.fnt");
	//addChild(label);
	//label->setPosition(designWidth/2, designHeight/2);
	scoreBox = Sprite::create("mainscence/scoreBox.png");
	addChild(scoreBox);
	scoreBox->setPosition(designWidth - 110, designHeight - 50);
	scoreBox->setScale(0.5);

	return true;
}

void ScoreLayer::onEnter() {
	Layer::onEnter();
	auto l1 = EventListenerCustom::create(Message::score,
		CC_CALLBACK_1(ScoreLayer::displayScore, this));
	auto l2 = EventListenerCustom::create(Message::score,
		CC_CALLBACK_1(ScoreLayer::displayCombos, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(l1, this);
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
		l3->setPosition(0, -50);
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

void ScoreLayer::displayScore(EventCustom *event) {
	int ds = *static_cast<int *>(event->getUserData());
	int &score = DataVo::inst()->score;
	if (score < 0)
		score = 0;
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
