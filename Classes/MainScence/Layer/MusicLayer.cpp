#include "MusicLayer.h"

#include "DataManager/DataVo.h"
#include "consts/ResolutionConst.h"
#include "consts/Message.h"
#include "consts/MyConsts.h"

MusicLayer* MusicLayer::create(std::string skinPath) {
	MusicLayer* node = new MusicLayer();
	if (node) node->skinPath = skinPath;
	if (node && !node->init()) 
		delete node;
	return node;
}

bool MusicLayer::init() {
	if (!Layer::init()) return false; 

	auto bg = Sprite::create(skinPath + "bg.jpg");
	bg->setPosition(_center);
	addChild(bg, -2);

	auto roadbg = Sprite::create(skinPath + "road.png");
	roadbg->setPosition(designWidth/2, designHeight/2-50);
	addChild(roadbg, -1);
	roadTexture = CCTextureCache::sharedTextureCache()->addImage(skinPath + "road2.png");

	int numbers = 150;
	float d = 1.0105349152832204381907932438556f;
	float x = 0.258, y = 1064.0/1111, tolx = 0, toly = 1064.0/1111;
	for (int i = 0; i < numbers; i++) {
		tolx += x;
		x = x * d;
	}
	x = 0.258;
	objects.clear();
	for (int i = 0; i < numbers + 30; i++) {
		Node* node = Node::create();
		node->setContentSize(Size(1.28f * roadbg->getContentSize().width, 1));
		node->setNormalizedPosition(Vec2(0.5, y));
		node->setScale(x);
		addChild(node, 2);
		objects.push_back(node);
		y -= toly / tolx * x;
		x = x * d;
	}
	std::reverse(objects.begin(), objects.end());

	d = pow(1226 / 384, 1.0 / (numbers - 1));
	x = 383, y = 1050, tolx = 0, toly = 1050;
	for (int i = 0; i < numbers; i++) {
		tolx += x;
		x = x * d;
	}
	x = 383;
	roads.clear();
	for (int i = 0; i < numbers + 30; i++) {
		Node* node = Node::create();
		node->setContentSize(Size(1046, 1));
		node->setPosition(Vec2(_center.x, y));
		node->setScale(x / 667);
		addChild(node, 1);
		roads.push_back(node);
		y -= toly / tolx * x;
		x = x * d;
	}
	std::reverse(roads.begin(), roads.end());

	return true;
}

void MusicLayer::onEnter() {
	Layer::onEnter();
	_eventDispatcher->addCustomEventListener(Message::next_timeslice,
											 CC_CALLBACK_1(MusicLayer::run, this));
	_eventDispatcher->addCustomEventListener(Message::next_note,
											 CC_CALLBACK_1(MusicLayer::addNote, this));
	_eventDispatcher->addCustomEventListener(Message::beer_effect_start,
											 CC_CALLBACK_1(MusicLayer::explodeAll, this));
	_eventDispatcher->addCustomEventListener(Message::beer_effect_stop,
											 CC_CALLBACK_1(MusicLayer::explodeAll, this));
	_eventDispatcher->addCustomEventListener(Message::explode,
											 CC_CALLBACK_1(MusicLayer::explode, this));
}

void MusicLayer::onExit() {
	_eventDispatcher->removeCustomEventListeners(Message::explode);
	_eventDispatcher->removeCustomEventListeners(Message::next_note);
	_eventDispatcher->removeCustomEventListeners(Message::next_timeslice);
	Layer::onExit();
}

void MusicLayer::run(EventCustom* event) {
	auto cache = SpriteFrameCache::getInstance();
	int moveSpeed = 1;
//	if (DataVo::inst()->isBeerEffectStart) moveSpeed = 2;
	for (int i = 0; i < moveSpeed; i++) {
		objects[i]->removeAllChildren();
		roads[i]->removeAllChildren();
	}
	for (int i = moveSpeed; i < objects.size(); i++) {
		std::vector<Node*> v;
		for (auto node : objects[i]->getChildren()) v.push_back(node);
		for (auto node : v) {
			node->retain();
			objects[i]->removeChild(node);
			objects[i - moveSpeed]->addChild(node);

			if (node->getName() == "explode") {
				int runCount = node->getTag();
				node->setTag(node->getTag() + 1);
				if (runCount % 4 == 1) {
					int k = runCount / 4 + 1;
					if (k > 11)
						node->setVisible(false);
					else {
						char name[20];
						sprintf(name, "explode_%d.png", k);
						static_cast<Sprite *>(node)->setDisplayFrame(cache->getSpriteFrameByName(name));
					}
				}
				if (runCount % 3 == 0) {
					objects[i - moveSpeed]->removeChild(node);
					objects[i]->addChild(node);
				}
			}
			node->release();
		}

		v.clear();
		for (auto node : roads[i]->getChildren()) v.push_back(node);
		for (auto node : v) {
			node->retain();
			roads[i]->removeChild(node);
			roads[i - moveSpeed]->addChild(node);
			node->release();
		}
	}
	addBuilding();
	roadMove();
}


void MusicLayer::addNote(EventCustom* event) {
	int index = *(int*)event->getUserData();
	//int index = curTime + 135;
	if (index >= DataVo::inst()->musicLength || index < 0) return;
	addTrafficCone(index);
	addBeer(index); 
	addBarrier(index); 
	addRailing(index);
	addManholeCover(index);
}

void MusicLayer::addBuilding() {
	static int lastL = 0, lastR = 0;
	int interval = 40, k;
	if (++lastL >= interval) {
		k = rand() % 4 + 1;
		lastL = 0;
		std::string url = skinPath + "building/building_" + std::to_string(k) + ".png";
		Sprite* sprite = Sprite::create(url);
		sprite->setScale(1.4f);
      	sprite->setNormalizedPosition(Vec2(-0.60, 0));
		sprite->setTag(0);
		objects.back()->addChild(sprite);
	}
	if (++lastR >= interval) {
		lastR = 0;
		k = rand() % 4 + 1;
		lastL = 0;
		std::string url = skinPath + "building/building_" + std::to_string(k) + ".png";
		Sprite* sprite = Sprite::create(url);
		sprite->setScale(1.4f);
		sprite->setFlipX(true);
      	sprite->setNormalizedPosition(Vec2(0.60, 0));
		sprite->setTag(0);
		objects.back()->addChild(sprite);
	}
}
void MusicLayer::addTrafficCone(int index) {
	if (DataVo::inst()->data[index][0]) {
		Sprite* sprite = Sprite::create(skinPath + "obstacle.png");
		sprite->setScale(1.5f);
      	sprite->setNormalizedPosition(Vec2(-0.13, 0));
		sprite->setTag(index);
		objects.back()->addChild(sprite);
	}
	if (DataVo::inst()->data[index][1]) {
		Sprite* sprite = Sprite::create(skinPath + "obstacle.png");
		sprite->setScale(1.5f);
      	sprite->setNormalizedPosition(Vec2(0.13, 0));
		sprite->setTag(index);
		objects.back()->addChild(sprite);
	}
}

void MusicLayer::addBeer(int index) {
	if (DataVo::inst()->data[index][2] == 1) {
		Sprite* sprite = Sprite::create(skinPath + "beer.png");
		sprite->setScale(1.5f);
      	sprite->setNormalizedPosition(Vec2(-0.13, 0));
		sprite->setTag(index);
		objects.back()->addChild(sprite);
	}
	if (DataVo::inst()->data[index][3] == 1) {
		Sprite* sprite = Sprite::create(skinPath + "beer.png");
		sprite->setScale(1.5f);
      	sprite->setNormalizedPosition(Vec2(0.13, 0));
		sprite->setTag(index);
		objects.back()->addChild(sprite);
	}
}

void MusicLayer::addBarrier(int index) {
	int interval = 10;
	if (DataVo::inst()->data[index][4] % interval == 1) {
      	Sprite* sprite = Sprite::create(skinPath + "barrier.png");
		sprite->setScale(1.5f);
      	sprite->setNormalizedPosition(Vec2(-0.19, 0));
		sprite->setTag(index);
		objects.back()->addChild(sprite);
	}
	if (DataVo::inst()->data[index][5] % interval == 1) {
      	Sprite* sprite = Sprite::create(skinPath + "barrier.png");
		sprite->setScale(1.5f);
      	sprite->setNormalizedPosition(Vec2(0.19, 0));
		sprite->setFlipX(true);
		sprite->setTag(index);
		objects.back()->addChild(sprite);
	}
}

void MusicLayer::addRailing(int index) {
	int interval = 19;
	if (DataVo::inst()->data[index][6] % interval == 1) {
      	Sprite* sprite = Sprite::create(skinPath + "railing.png");
      	sprite->setNormalizedPosition(Vec2(-0.14, 0));
		sprite->setRotation(-6.5f);
		sprite->setFlipX(true);
		sprite->setTag(index);
		objects.back()->addChild(sprite);
	}
	if (DataVo::inst()->data[index][7] % interval == 1) {
      	Sprite* sprite = Sprite::create(skinPath + "railing.png");
		sprite->setRotation(6.5f);
      	sprite->setNormalizedPosition(Vec2(0.14, 0));
		sprite->setTag(index);
		objects.back()->addChild(sprite);
	}
}
void MusicLayer::addManholeCover(int index) {
	if (DataVo::inst()->data[index][8]) {
      	Sprite* sprite = Sprite::create(skinPath + "manholecover.png");
      	sprite->setNormalizedPosition(Vec2(0, 0));
		sprite->setScale(1.5f);
		sprite->setTag(index);
		objects.back()->addChild(sprite);
	}
}


void MusicLayer::explode(EventCustom *event) {
	int tag = *static_cast<int *>(event->getUserData());
	for (auto node : objects) {
		auto beer = node->getChildByTag(tag);
		if (beer) {
			beer->setName("explode");
			beer->setTag(1);
			beer->setScale(5.2f);
		}
	}
}

void MusicLayer::explodeAll(EventCustom *event) {
	for (auto node : objects) {
		auto notes = node->getChildren();
		for (auto note : notes) {
			if (note->getTag() > 100) {
				note->setName("explode");
				note->setTag(1);
				note->setScale(5.2f);
			}
		}
	}
}

void MusicLayer::roadMove() {
	Sprite* sprite = Sprite::create();
	static float y = 0;
	float H = 7;
	Rect rect = Rect(0, y, 667, H);
	y = (y + H);
	if (y + H >= 960) y -= 960;
	sprite->setTexture(roadTexture);
	sprite->setTextureRect(rect);
	sprite->setNormalizedPosition(Vec2(0, 0));
	sprite->setTag(0);
	roads.back()->addChild(sprite);
}
