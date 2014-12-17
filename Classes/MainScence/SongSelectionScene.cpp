#include "SongSelectionScene.h"
#include "consts\ResolutionConst.h"
#include "GameWorld.h"

using namespace cocos2d;

bool SongSelectionScene::init() {
	if (!Scene::init()) return false;

	bg = Layer::create();
	addChild(bg);

	auto bgPic = Sprite::create("loginscence/songSelect.png");
	bgPic->setAnchorPoint(Point(0,0));
	bgPic->setScale(0.8);
	bg->addChild(bgPic, -1000);

	auto cloud1 = Sprite::create("loginscence/cloud.png");
	auto cloud2 = Sprite::create("loginscence/cloud.png");
	auto cloud3 = Sprite::create("loginscence/cloud.png");
	bg->addChild(cloud1, 1);
	bg->addChild(cloud2, 1);
	bg->addChild(cloud3, 1);
	cloud1->setScale(0.5);
	cloud2->setScale(0.3);
	cloud3->setScale(0.7);
	cloud1->setOpacity(0);
	cloud2->setOpacity(0);
	cloud3->setOpacity(0);
	cloud1->setPosition( 100, designHeight - 100 );
	cloud2->setPosition( designWidth - 100, designHeight - 50 );
	cloud3->setPosition( designWidth/2, designHeight - 130 );
	float actionTime = 0.4;
	auto cloud11 = MoveBy::create(10 * actionTime, Vec2(designWidth - 200, 0));
	cloud1->runAction(RepeatForever::create(Sequence::create(
		cloud11, cloud11->reverse(), nullptr)));
	cloud1->runAction(FadeTo::create(actionTime, 200));
	auto cloud21 = MoveBy::create(16 * actionTime, Vec2(-designWidth + 200, 0));
	cloud2->runAction(RepeatForever::create(Sequence::create(
		cloud21, cloud21->reverse(), nullptr)));
	cloud2->runAction(FadeTo::create(actionTime, 200));
	auto cloud31 = MoveBy::create(20 * actionTime, Vec2(designWidth/2 , 0));
	cloud3->runAction(RepeatForever::create(Sequence::create(
		cloud31, cloud31->reverse(), cloud31->reverse(), cloud31, nullptr)));
	cloud3->runAction(FadeTo::create(actionTime, 200));

	fg = Layer::create();
	addChild(fg);
	auto backButton = MenuItemImage::create("mainscence/home.png", "mainscence/home.png",
		[this](Ref *){Director::getInstance()->popScene();});
	menu = Menu::create(backButton, nullptr);
	menu->setPosition(designWidth - 100, designHeight - 100);
	fg->addChild(menu, 1000);

	auto title = LabelTTF::create("选择音乐", "fonts/Marker Felt.ttf", 70);
	title->setColor(Color3B::BLACK);
	title->setAnchorPoint(Point(0, 1));
	title->setPosition(50, designHeight-50);
	fg->addChild(title);

	//just for test;
	auto song1 = Sprite::create("songs/test.png");
	auto song2 = Sprite::create("songs/song2.png");
	auto song3 = Sprite::create("songs/song3.png");
	auto songName1 = LabelTTF::create("老子明天不上谢帝", "fonts/Marker Felt.ttf", 25);
	auto songName2 = LabelTTF::create("老子明天不上谢帝", "fonts/Marker Felt.ttf", 25);
	auto songName3 = LabelTTF::create("老子明天不上谢帝", "fonts/Marker Felt.ttf", 25);
	songName1->setColor(Color3B::BLACK);
	songName2->setColor(Color3B::BLACK);
	songName3->setColor(Color3B::BLACK);

	songs.push_back(std::make_pair(song1, songName1));
	songs.push_back(std::make_pair(song2, songName2));
	songs.push_back(std::make_pair(song3, songName3));

	focus = 0;

	for (int i = 0; i < songs.size(); i++) {
		fg->addChild(songs[i].first);
		fg->addChild(songs[i].second);
		songs[i].first->setAnchorPoint(Point(0.5, 0));
		songs[i].second->setAnchorPoint(Point(0.5, 0));
		songs[i].first->setPosition(designWidth/2 + 460*i, designHeight/2 - 150 );
		songs[i].second->setPosition(designWidth/2 + 460*i, designHeight/2 - 175);
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
	float actionTime = 0.2;
	if(endPoint.x - beginPoint.x > distance) { //向右划
		if(focus > 0) {
			for (auto song : songs) {
				auto move = MoveBy::create(actionTime, Vec2(460, 0));
				song.first->runAction(move);
			}
			focus--;
		}
	}
	else if(endPoint.x - beginPoint.x < -distance) { //向左划
		if(focus < songs.size() - 1) {
			for (auto song : songs) {
				auto move = MoveBy::create(actionTime, Vec2(-460, 0));
				song.first->runAction(move);
			}
			focus++;
		}
	}
	else { //点击
		auto game = GameWorld::create();
		Director::getInstance()->pushScene(game);
	}
}