#include "RecordScene.h"
#include "consts/ResolutionConst.h"
#include <string>
#include "DataManager/LocalRecord.h"

bool RecordScene::init() {
	if (!Scene::init()) return false;

	auto bg = Layer::create();
	auto bgPic = Sprite::create("helpandoptionsscene/bg.png");
	auto home = MenuItemImage::create("buttons/button_back_n.png", "buttons/button_back_n.png", 
		[this](Ref *){Director::getInstance()->popScene();});
	auto menu = Menu::create(home, nullptr);

	home->setAnchorPoint(Point(0, 1));
	home->setPosition(50, -50);
	menu->setAnchorPoint(Point(0, 1));
	menu->setPosition(0, designHeight);

	addChild(bg);
	bg->addChild(bgPic);
	bg->addChild(menu);

	bgPic->setScale(designWidth/bgPic->getTextureRect().getMaxX());
	bgPic->setPosition(designWidth/2, designHeight/2);

	for (int i = 1; i <= 3; i++) {
		std::string name = "songs/song" + std::to_string(i);
		int score = LocalRecord::inst()->getHighestRecord(name);
		auto label = LabelBMFont::create(std::to_string(score), "fonts/number_2.fnt");
		bg->addChild(label);
		label->setAnchorPoint(Point(0.5, 0.5));
		label->setPosition(designWidth/2, designHeight - 200 - i * 100);

		name += "/name.png";
		auto pic = Sprite::create(name);
		bg->addChild(pic);
		pic->setPosition(designWidth/2, designHeight - 150 - i * 100);
	}

	return true;
}

void RecordScene::onEnter() {
	Scene::onEnter();
}

void RecordScene::onExit() {
	Scene::onExit();
}