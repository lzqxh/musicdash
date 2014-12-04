#include "MusicLayer.h"

#include "DataManager/DataVo.h"
#include "consts/ResolutionConst.h"
#include "consts/Message.h"
#include "consts/MyConsts.h"

bool MusicLayer::init() {
	if (!Layer::init()) return false; 
	auto roadbg = Sprite::create("mainscence/road.png");
	roadbg->setScale(PNG_SCALE);
	roadbg->setPosition(_center);
	addChild(roadbg);

	int numbers = 250;
	float d = pow(1.28 / 0.258, 1.0 / (numbers - 1));
	float x = 0.258, y = 1.0, tolx = 0, toly = 1.0;
	for (int i = 0; i < numbers; i++) {
		tolx += x;
		x = x * d;
	}
	x = 0.258;
	for (int i = 0; i < numbers + 10; i++) {
		Node* node = Node::create();
		node->setContentSize(Size(1.28f * roadbg->getContentSize().width, 1));
		node->setNormalizedPosition(Vec2(0.5, y));
		node->setScale(x);
		roadbg->addChild(node);
		objects.push_back(node);
		y -= toly / tolx * x;
		x = x * d;
	}
	std::reverse(objects.begin(), objects.end());
	return true;
}

void MusicLayer::onEnter() {
	Layer::onEnter();
	_eventDispatcher->addCustomEventListener(Message::next_timeslice,
											 CC_CALLBACK_1(MusicLayer::run, this));
}

void MusicLayer::onExit() {
	_eventDispatcher->removeCustomEventListeners(Message::next_timeslice);
	Layer::onExit();
}

void MusicLayer::run(EventCustom* event) {
	std::vector<std::vector<int> > &data = DataVo::inst()->data;
	objects[0]->removeAllChildren();
	for (int i = 1; i < objects.size(); i++) {
		std::vector<Node*> v;
		for (auto node : objects[i]->getChildren()) v.push_back(node);
		for (auto node : v) {
			node->retain();
			objects[i]->removeChild(node);
			objects[i - 1]->addChild(node);
			node->release();
		}
	}
	int index = curTime + objects.size() - 1;
	if (index >= DataVo::inst()->musicLength) return;
	addTrafficCone(index);
	addBeer(index);
	addBarrier(index);
	addRailing(index);
	addManholeCover(index);
}

void MusicLayer::addTrafficCone(int index) {
	if (DataVo::inst()->data[index][0]) {
      	Sprite* sprite = Sprite::create("mainscence/obstacle1.png");
      	sprite->setNormalizedPosition(Vec2(-0.25, 0));
		objects.back()->addChild(sprite);
	}
	if (DataVo::inst()->data[index][1]) {
      	Sprite* sprite = Sprite::create("mainscence/obstacle1.png");
      	sprite->setNormalizedPosition(Vec2(0.25, 0));
		objects.back()->addChild(sprite);
	}
}

void MusicLayer::addBeer(int index) {
	if (DataVo::inst()->data[index][2]) {
      	Sprite* sprite = Sprite::create("mainscence/beer.png");
      	sprite->setNormalizedPosition(Vec2(-0.25, 0));
		objects.back()->addChild(sprite);
	}
	if (DataVo::inst()->data[index][3]) {
      	Sprite* sprite = Sprite::create("mainscence/beer.png");
      	sprite->setNormalizedPosition(Vec2(0.25, 0));
		objects.back()->addChild(sprite);
	}
}

void MusicLayer::addBarrier(int index) {
	int interval = 7;
	if (DataVo::inst()->data[index][4] % interval == 1) {
      	Sprite* sprite = Sprite::create("mainscence/barrier.png");
      	sprite->setNormalizedPosition(Vec2(-0.19, 0));
		objects.back()->addChild(sprite);
	}
	if (DataVo::inst()->data[index][5] % interval == 1) {
      	Sprite* sprite = Sprite::create("mainscence/barrier.png");
      	sprite->setNormalizedPosition(Vec2(0.19, 0));
		sprite->setFlipX(true);
		objects.back()->addChild(sprite);
	}
}

void MusicLayer::addRailing(int index) {
	int interval = 17;
	if (DataVo::inst()->data[index][6] % interval == 1) {
      	Sprite* sprite = Sprite::create("mainscence/railing.png");
      	sprite->setNormalizedPosition(Vec2(-0.25, 0));
		sprite->setFlipX(true);
		objects.back()->addChild(sprite);
	}
	if (DataVo::inst()->data[index][7] % interval == 1) {
      	Sprite* sprite = Sprite::create("mainscence/railing.png");
      	sprite->setNormalizedPosition(Vec2(0.25, 0));
		objects.back()->addChild(sprite);
	}
}
void MusicLayer::addManholeCover(int index) {
	if (DataVo::inst()->data[index][8]) {
      	Sprite* sprite = Sprite::create("mainscence/manholecover.png");
      	sprite->setNormalizedPosition(Vec2(0, 0));
		objects.back()->addChild(sprite);
	}
}
