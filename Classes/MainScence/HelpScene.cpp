#include "HelpScene.h"
#include "consts/ResolutionConst.h"
#include <string>

bool HelpScene::init() {
	if (!Scene::init()) return false;

	auto bg = Layer::create();
	auto bgPic = Sprite::create("helpandoptionsscene/bg.jpg");
	auto home = MenuItemImage::create("buttons/button_home_n.png", "buttons/button_home_s.png", 
		[this](Ref *){Director::getInstance()->popScene();});
	auto menu = Menu::create(home, nullptr);

	home->setAnchorPoint(Point(0, 1));
	home->setPosition(50, 0);
	menu->setAnchorPoint(Point(0, 1));
	menu->setPosition(0, designHeight);

	addChild(bg);
	bg->addChild(bgPic);
	bg->addChild(menu);

	for (int i = 0; i < 3; i++) {
		std::string filename = "helpandoptionsscene/helpcard_" + std::to_string(i) + ".png";
		auto helpCard = Sprite::create(filename);
		cards.push_back(helpCard);
		addChild(helpCard);
		helpCard->setAnchorPoint(Point(0.5, 0.5));
		auto x = helpCard->getTextureRect().getMaxX() + 50;
		helpCard->setPosition(designWidth/2 + x * i, designHeight/2);
	}

	return true;
}

void HelpScene::onEnter() {
	Scene::onEnter();
	tl = EventListenerTouchOneByOne::create();
	tl->onTouchMoved = CC_CALLBACK_2(HelpScene::onTouchMoved, this);
	tl->onTouchBegan = CC_CALLBACK_2(HelpScene::onTouchBegan, this);
	tl->onTouchEnded = CC_CALLBACK_2(HelpScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(tl, this);
}

void HelpScene::onExit() {
	_eventDispatcher->removeEventListener(tl);
	Scene::onExit();
}

bool HelpScene::onTouchBegan(Touch *touch, Event *event) {
	start = touch->getLocation();
	direction = 0;
	return true;
}

void HelpScene::onTouchMoved(Touch *touch, Event *event) {
	auto center = touch->getLocation();
	for (auto card : cards) {
		card->runAction(MoveBy::create(0, Point(center.x - start.x, 0)));
	}
	direction = center.x - start.x;
	start = center;
}

void HelpScene::onTouchEnded(Touch *touch, Event *event) {
	Sprite *midCard = nullptr;
	float min = designWidth;
	float midX = (direction < 0) ? designWidth/3*2 : designWidth/3;

	for (auto card : cards) {
		auto dis = abs(card->getPosition().x - midX);
		if (dis < min) {
			midCard = card;
			min = dis;
		}
	}
	if (!midCard)
		midCard = cards[0];

	for (auto card : cards) {
		card->runAction(MoveBy::create(0.2, Point(designWidth/2 - midCard->getPosition().x, 0)));
	}
}