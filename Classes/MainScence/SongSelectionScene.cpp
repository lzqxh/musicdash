#include "SongSelectionScene.h"
#include "consts\ResolutionConst.h"
#include "GameWorld.h"

using namespace cocos2d;

bool SongSelectionScene::init() {
	if (!Scene::init()) return false;

	bg = Layer::create();
	addChild(bg);

	auto pic = Sprite::create("mainscence/bg.jpg");
	pic->setAnchorPoint(Point(0,0));
	bg->addChild(pic, -1000);

	auto backButton = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
		[this](Ref *){Director::getInstance()->popScene();});
	menu = Menu::create(backButton, nullptr);
	menu->setPosition(designWidth - 50, designHeight - 50);
	bg->addChild(menu, 1000);

	//just for test;
	auto song1 = Sprite::create("songs/test.png");
	auto song2 = Sprite::create("songs/song2.png");
	auto song3 = Sprite::create("songs/song3.png");

	songs.push_back(song1);
	songs.push_back(song2);
	songs.push_back(song3);

	focus = 0;

	for (int i = 0; i < songs.size(); i++) {
		bg->addChild(songs[i]);
		songs[i]->setPosition(designWidth/2 + 460*i, designHeight/2);
	}

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(SongSelectionScene::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(SongSelectionScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	return true;
}

void SongSelectionScene::onEnter() {
	Scene::onEnter();
}

bool SongSelectionScene::onTouchBegan( Touch * touch, Event *event) {
	beginPoint = touch->getLocation();
	return true;
}
void SongSelectionScene::onTouchEnded( Touch * touch, Event *event) {
	auto endPoint = touch->getLocation();
	int distance = 10;
	float actionTime = 0.5;
	if(endPoint.x - beginPoint.x > distance) { //向右划
		if(focus > 0) {
			for (auto song : songs) {
				auto move = MoveBy::create(actionTime, Vec2(460, 0));
				song->runAction(move);
			}
			focus--;
		}
	}
	else if(endPoint.x - beginPoint.x < -distance) { //向左划
		if(focus < songs.size() - 1) {
			for (auto song : songs) {
				auto move = MoveBy::create(actionTime, Vec2(-460, 0));
				song->runAction(move);
			}
			focus++;
		}
	}
	else { //点击
		auto game = GameWorld::create();
		Director::getInstance()->pushScene(game);
	}
}