#include "OptionsScene.h"
#include "consts/ResolutionConst.h"

bool OptionsScene::ifShack = true;
bool OptionsScene::ifSound = true;

bool OptionsScene::init() {
	if (!Scene::init()) return false;

	auto bg = Layer::create();
	auto bgPic = Sprite::create("helpandoptionsscene/bg.jpg");
	auto optionBox = Sprite::create("helpandoptionsscene/optionBox.png");
	auto home = MenuItemImage::create("buttons/button_home_n.png", "buttons/button_home_s.png", 
		[this](Ref *){Director::getInstance()->popScene();});
	auto menu1 = Menu::create(home, nullptr);

	auto music = MenuItemImage::create("buttons/button_music_n.png", "buttons/button_music_n.png",
		[this](Ref *){
			auto inst = CocosDenshion::SimpleAudioEngine::getInstance();
			if (inst->isBackgroundMusicPlaying())
				inst->end();
			else
				inst->playBackgroundMusic("soundeffect/bg.mp3", true);
	});
	auto sound = MenuItemImage::create("buttons/button_sound_n.png", "buttons/button_sound_n.png",
		[this](Ref *){
			auto inst = CocosDenshion::SimpleAudioEngine::getInstance();
			OptionsScene::ifSound = !OptionsScene::ifSound;
			if (OptionsScene::ifSound)
				inst->resumeAllEffects();
			else
				inst->stopAllEffects();
	});
	auto shack = MenuItemImage::create("buttons/button_shack_n.png", "buttons/button_shack_n.png",
		[this](Ref *){
			OptionsScene::ifShack = !OptionsScene::ifShack;
	});
	auto menu2 = Menu::create(music, sound, shack, nullptr);

	addChild(bg);
	bg->addChild(bgPic);
	bg->addChild(menu1);
	bg->addChild(optionBox);
	optionBox->addChild(menu2);

	home->setAnchorPoint(Point(0, 1));
	home->setPosition(50, 0);
	menu1->setAnchorPoint(Point(0, 1));
	menu1->setPosition(0, designHeight);
	
	optionBox->setAnchorPoint(Point(0.5, 0.5));
	optionBox->setPosition(designWidth/2, designHeight/2);
	menu2->setAnchorPoint(Point(0.5, 0.5));
	auto mid = Point(optionBox->getTextureRect().getMaxX()/2, optionBox->getTextureRect().getMaxY()/2);
	menu2->setPosition(mid);
	music->setPosition(0, 100);
	sound->setPosition(0, -50);
	shack->setPosition(0, -200);

	return true;
}

void OptionsScene::onEnter() {
	Scene::onEnter();
}

void OptionsScene::onExit() {
	Scene::onExit();
}