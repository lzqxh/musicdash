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
	Rect rect;
	rect.setRect(10, 10, 25, designHeight / 100);
	for (int i = 0; i < 100; i++) {
		Sprite* sprite;
		sprite = Sprite::create("test.png", rect);
		sprite->setPosition(designWidth/6 * 1, designHeight / 101 * (i + 1));
		addChild(sprite);
		barrier[0].push_back(sprite);

		sprite = Sprite::create("test.png", rect);
		sprite->setPosition(designWidth/6 * 5, designHeight / 101 * (i + 1));
		addChild(sprite);
		barrier[1].push_back(sprite);
	}

	//—©ª®Õ∞’œ∞≠ŒÔ
	float d = 1.0163099;
	float x = 0.258, y = 1.0, tolx = 0, toly = 1.0;
	for (int i = 0; i < 100; i++) {
		tolx += x;
		x = x * d;
	}
	x = 0.258;
	for (int i = 0; i < 100; i++) {
		Sprite* sprite;
		sprite = Sprite::create("mainscence/obstacle1.png");
		sprite->setScale(PNG_SCALE * x);
		sprite->setPosition(designWidth * (0.5 - 0.25 * x), y * designHeight);
		addChild(sprite);
		obstacleLR[0].push_back(sprite);

		sprite = Sprite::create("mainscence/obstacle1.png");
		sprite->setScale(PNG_SCALE * x);
		sprite->setPosition(designWidth * (0.5 + 0.25 * x), y * designHeight);
		addChild(sprite);
		obstacleLR[1].push_back(sprite);

		y -= toly / tolx * x;
		x = x * d;
	}
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
	for (int i = curTime, pos = 0; i < curTime + 100; i++, pos++) {
		barrier[0][pos]->setVisible(i < data.size() && data[i][0] == 1);
		barrier[1][pos]->setVisible(i < data.size() && data[i][4] == 1);
	}
	for (int i = curTime, pos = 0; i < curTime + 100; i++, pos++) {
		obstacleLR[0][pos]->setVisible(true);
		obstacleLR[1][pos]->setVisible(true);
	//	obstacleLR[0][pos]->setVisible(i < data.size() && data[i][1] == 1);
	//	obstacleLR[1][pos]->setVisible(i < data.size() && data[i][3] == 1);
	}
}

