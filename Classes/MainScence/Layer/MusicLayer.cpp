#include "MusicLayer.h"

#include "DataManager/DataVo.h"
#include "consts/ResolutionConst.h"
#include "consts/Message.h"
#include "consts/MyConsts.h"

bool MusicLayer::init() {
	if (!Layer::init()) return false; 
	_explosionTexture = TextureCache::getInstance()->addImage("mainscence/explosion.png");

	auto roadbg = Sprite::create("mainscence/road.png");
	roadbg->setScale(PNG_SCALE);
	roadbg->setPosition(_center);
	addChild(roadbg);

	int numbers = 150;
	float d = pow(1.28 / 0.258, 1.0 / (numbers - 1));
	float x = 0.258, y = 1.0, tolx = 0, toly = 1.0;
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
		roadbg->addChild(node);
		objects.push_back(node);
		y -= toly / tolx * x;
		x = x * d;
	}
	std::reverse(objects.begin(), objects.end());

	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("mainscence/explode.plist");
	
	return true;
}

void MusicLayer::onEnter() {
	Layer::onEnter();
	_eventDispatcher->addCustomEventListener(Message::next_timeslice,
											 CC_CALLBACK_1(MusicLayer::run, this));
	_eventDispatcher->addCustomEventListener(Message::disp_effect,
											 CC_CALLBACK_1(MusicLayer::dispEffect, this));
	_eventDispatcher->addCustomEventListener(Message::explode,
											 CC_CALLBACK_1(MusicLayer::explode, this));
}

void MusicLayer::onExit() {
	_eventDispatcher->removeCustomEventListeners(Message::explode);
	_eventDispatcher->removeCustomEventListeners(Message::disp_effect);
	_eventDispatcher->removeCustomEventListeners(Message::next_timeslice);
	Layer::onExit();
}

void MusicLayer::run(EventCustom* event) {
	static int runCount = 0;
	runCount++;
	std::vector<std::vector<int> > &data = DataVo::inst()->data;
	if (DataVo::inst()->data[curTime][8] == 1) {
		CCLog("%d\n", curTime);
	}
	objects[0]->removeAllChildren();
	auto cache = SpriteFrameCache::getInstance();
	for (int i = 1; i < objects.size(); i++) {
		std::vector<Node*> v;
		for (auto node : objects[i]->getChildren()) v.push_back(node);
		for (auto node : v) {
			node->retain();
			objects[i]->removeChild(node);
			objects[i - 1]->addChild(node);
			node->release();

			if (node->getName() == "explode" && runCount >= 4) {
				runCount = 0;
				if (node->getTag() > 11)
					node->setVisible(false);
				else {
					char name[20];
					sprintf(name, "explode_%d.png", node->getTag());
					node->setTag(node->getTag() + 1);
					static_cast<Sprite *>(node)->setDisplayFrame(cache->getSpriteFrameByName(name));
				}
			}
		}
	}
	int index = curTime + 135;
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
		sprite->setScale(1.5f);
      	sprite->setNormalizedPosition(Vec2(-0.13, 0));
		objects.back()->addChild(sprite);
	}
	if (DataVo::inst()->data[index][1]) {
		Sprite* sprite = Sprite::create("mainscence/obstacle1.png");
		sprite->setScale(1.5f);
      	sprite->setNormalizedPosition(Vec2(0.13, 0));
		objects.back()->addChild(sprite);
	}
}

void MusicLayer::addBeer(int index) {
	if (DataVo::inst()->data[index][2] == 1) {
		Sprite* sprite = Sprite::create("mainscence/beer.png");
		sprite->setScale(1.5f);
      	sprite->setNormalizedPosition(Vec2(-0.13, 0));
		sprite->setTag(index);
		objects.back()->addChild(sprite);
	}
	if (DataVo::inst()->data[index][3] == 1) {
		Sprite* sprite = Sprite::create("mainscence/beer.png");
		sprite->setScale(1.5f);
      	sprite->setNormalizedPosition(Vec2(0.13, 0));
		sprite->setTag(index);
		objects.back()->addChild(sprite);
	}
}

void MusicLayer::addBarrier(int index) {
	int interval = 10;
	if (DataVo::inst()->data[index][4] % interval == 1) {
      	Sprite* sprite = Sprite::create("mainscence/barrier.png");
		sprite->setScale(1.5f);
      	sprite->setNormalizedPosition(Vec2(-0.19, 0));
		objects.back()->addChild(sprite);
	}
	if (DataVo::inst()->data[index][5] % interval == 1) {
      	Sprite* sprite = Sprite::create("mainscence/barrier.png");
		sprite->setScale(1.5f);
      	sprite->setNormalizedPosition(Vec2(0.19, 0));
		sprite->setFlipX(true);
		objects.back()->addChild(sprite);
	}
}

void MusicLayer::addRailing(int index) {
	int interval = 19;
	if (DataVo::inst()->data[index][6] % interval == 1) {
      	Sprite* sprite = Sprite::create("mainscence/railing.png");
      	sprite->setNormalizedPosition(Vec2(-0.14, 0));
		sprite->setRotation(-6.5f);
		sprite->setFlipX(true);
		objects.back()->addChild(sprite);
	}
	if (DataVo::inst()->data[index][7] % interval == 1) {
      	Sprite* sprite = Sprite::create("mainscence/railing.png");
		sprite->setRotation(6.5f);
      	sprite->setNormalizedPosition(Vec2(0.14, 0));
		objects.back()->addChild(sprite);
	}
}
void MusicLayer::addManholeCover(int index) {
	if (DataVo::inst()->data[index][8]) {
		CCLog("%d %d\n", curTime, index);
      	Sprite* sprite = Sprite::create("mainscence/manholecover.png");
      	sprite->setNormalizedPosition(Vec2(0, 0));
		sprite->setScale(1.5f);
		objects.back()->addChild(sprite);
	}
}

void MusicLayer::dispEffect(EventCustom *e) {
	int index = objects.size() - 135;
	objects[index]->removeAllChildren();
	Vec2 pos;
	pos.x = 0.2;
	pos.y = 0.2;
	if (DataVo::inst()->data[curTime][3] == 1) pos.x = 1.0f - pos.x;
	pos.x = designWidth * pos.x;
	pos.y = designHeight * pos.y;
	CCParticleSystem* m_emitter;
	m_emitter = CCParticleExplosion::create();
	m_emitter->setTotalParticles(50);
	m_emitter->setPosVar(ccp(100,100));
	m_emitter->setLife(0.9);
	m_emitter->setLifeVar(0.3);
	m_emitter->setGravity(ccp(20.0, 0));
	m_emitter->setStartSize(60.0f);
	m_emitter->setStartSizeVar(5.0f);
	m_emitter->setTexture( _explosionTexture );
	m_emitter->setPosition(pos);
	this->addChild(m_emitter, 10);
	m_emitter->setAutoRemoveOnFinish(true);
}

void MusicLayer::explode(EventCustom *event) {
	int tag = *static_cast<int *>(event->getUserData());
	for (auto node : objects) {
		auto beer = node->getChildByTag(tag);
		if (beer) {
			beer->setName("explode");
			beer->setTag(1);
		}
	}
}
