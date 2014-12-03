#include "MusicLayer.h"

#include "DataManager/DataVo.h"
#include "consts/ResolutionConst.h"
#include "consts/Message.h"

bool MusicLayer::init() {
	if (!Layer::init()) return false; 
	Rect rect;
	rect.setRect(10, 10, 25, designHeight / 100);
	for (int i = 0; i < 100; i++) {
		Sprite* sprite;
		sprite = Sprite::create("test.png", rect);
		sprite->setPosition(designWidth/6 * 1, designHeight / 101 * (i + 1));
		barrier[0].push_back(sprite);

		sprite = Sprite::create("test.png", rect);
		sprite->setPosition(designWidth/6 * 2, designHeight / 101 * (i + 1));
		barrier[1].push_back(sprite);

		sprite = Sprite::create("test.png", rect);
		sprite->setPosition(designWidth/6 * 3, designHeight / 101 * (i + 1));
		barrier[2].push_back(sprite);

		sprite = Sprite::create("test.png", rect);
		sprite->setPosition(designWidth/6 * 4, designHeight / 101 * (i + 1));
		barrier[3].push_back(sprite);

		sprite = Sprite::create("test.png", rect);
		sprite->setPosition(designWidth/6 * 5, designHeight / 101 * (i + 1));
		barrier[4].push_back(sprite);
	}
	for (int i = 0; i < 100; i++) 
		for (int k = 0; k < 5; k++) {
			addChild(barrier[k][i]);
			barrier[k][i]->setVisible(false);
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
		for (int j = 0; j < 5; j++) {
			if (i < data.size() && data[i][j] == 1) {
				barrier[j][pos]->setVisible(true);
			} else 
				barrier[j][pos]->setVisible(false);
		}
	}
}

