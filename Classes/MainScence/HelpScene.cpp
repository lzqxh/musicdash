#include "HelpScene.h"
#include "consts/ResolutionConst.h"
#include <string>

bool HelpScene::init() {
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

	for (int i = 0; i < 6; i++) {
		std::string filename = "helpandoptionsscene/helpcard_" + std::to_string(i) + ".png";
		auto helpCard = Sprite::create(filename);
		cards.push_back(helpCard);
		addChild(helpCard);
		helpCard->setAnchorPoint(Point(0.5, 0.5));
		helpCard->setScale(designWidth/4*3/helpCard->getTextureRect().getMaxX());
		auto x = helpCard->getTextureRect().getMaxX() * helpCard->getScale() + 50;
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
	if (center.x - start.x != 0) {
		for (auto card : cards)
			card->runAction(MoveBy::create(0, Point(center.x - start.x, 0)));
		direction = center.x - start.x;
		start = center;
	}
}

void HelpScene::onTouchEnded(Touch *touch, Event *event) {
	int midCard = 0;
	float min = designWidth;
	float midX = (direction < 0) ? designWidth/3*2 : designWidth/3;

	for (int i = 0; i < cards.size(); i++) {
		auto dis = abs(cards[i]->getPosition().x - midX);
		if (dis < min) {
			midCard = i;
			min = dis;
		}
	}

	auto x = cards[0]->getTextureRect().getMaxX() * cards[0]->getScale() + 50;
	for (int i = 0; i < cards.size(); i++)
		cards[i]->runAction(MoveTo::create(0.1, Point(designWidth/2 + x * (i - midCard), designHeight/2)));
}