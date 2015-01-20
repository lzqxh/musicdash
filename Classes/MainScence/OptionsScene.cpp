#include "OptionsScene.h"
#include "consts/ResolutionConst.h"

bool OptionsScene::ifMusic = true;
bool OptionsScene::ifShack = true;
bool OptionsScene::ifSound = true;

bool OptionsScene::init() {
	if (!Scene::init()) return false;

	auto bg = Layer::create();
	auto bgPic = Sprite::create("helpandoptionsscene/bg.png");
	auto optionBox = Sprite::create("helpandoptionsscene/optionsbox.png");
	auto home = MenuItemImage::create("buttons/button_home_n.png", "buttons/button_home_s.png", 
		[this](Ref *){Director::getInstance()->popScene();});
	auto menu1 = Menu::create(home, nullptr);

	auto *music = MenuItemImage::create("helpandoptionsscene/button_music_o.png", "helpandoptionsscene/button_music_o.png",
		[this](Ref *ref){
			auto item = static_cast<MenuItemImage *>(ref);
			auto inst = CocosDenshion::SimpleAudioEngine::getInstance();
			OptionsScene::ifMusic = !OptionsScene::ifMusic;
			Rect rect(item->getNormalImage()->getBoundingBox());
			if (inst->isBackgroundMusicPlaying()) {
				inst->end();
				item->setNormalSpriteFrame(SpriteFrame::create("helpandoptionsscene/button_music_c.png", rect));
			}
			else {
				inst->playBackgroundMusic("soundeffect/bg.mp3", true);
				item->setNormalSpriteFrame(SpriteFrame::create("helpandoptionsscene/button_music_o.png", rect));
			}
	});
	if (!OptionsScene::ifMusic) {
		Rect rect(music->getNormalImage()->getBoundingBox());
		music->setNormalSpriteFrame(SpriteFrame::create("helpandoptionsscene/button_music_c.png", rect));
	}

	auto sound = MenuItemImage::create("helpandoptionsscene/button_sound_o.png", "helpandoptionsscene/button_sound_o.png",
		[this](Ref *ref){
			auto item = static_cast<MenuItemImage *>(ref);
			Rect rect(item->getNormalImage()->getBoundingBox());
			OptionsScene::ifSound = !OptionsScene::ifSound;
			if (OptionsScene::ifSound)
				item->setNormalSpriteFrame(SpriteFrame::create("helpandoptionsscene/button_sound_o.png", rect));
			else
				item->setNormalSpriteFrame(SpriteFrame::create("helpandoptionsscene/button_sound_c.png", rect));

	});
	if (!OptionsScene::ifSound) {
		Rect rect(sound->getNormalImage()->getBoundingBox());
		sound->setNormalSpriteFrame(SpriteFrame::create("helpandoptionsscene/button_sound_c.png", rect));
	}

	auto shack = MenuItemImage::create("helpandoptionsscene/button_shack_o.png", "helpandoptionsscene/button_shack_o.png",
		[this](Ref *ref){
			auto item = static_cast<MenuItemImage *>(ref);
			Rect rect(item->getNormalImage()->getBoundingBox());
			OptionsScene::ifShack = !OptionsScene::ifShack;
			if (OptionsScene::ifShack)
				item->setNormalSpriteFrame(SpriteFrame::create("helpandoptionsscene/button_shack_o.png", rect));
			else
				item->setNormalSpriteFrame(SpriteFrame::create("helpandoptionsscene/button_shack_c.png", rect));
	});
	if (!OptionsScene::ifShack) {
		Rect rect(shack->getNormalImage()->getBoundingBox());
		shack->setNormalSpriteFrame(SpriteFrame::create("helpandoptionsscene/button_shack_c.png", rect));
	}
	auto menu2 = Menu::create(music, sound, shack, nullptr);

	addChild(bg);
	bg->addChild(bgPic);
	bg->addChild(menu1);
	bg->addChild(optionBox);
	optionBox->addChild(menu2);

	bgPic->setScale(designWidth/bgPic->getTextureRect().getMaxX());
	bgPic->setPosition(designWidth/2, designHeight/2);
	home->setAnchorPoint(Point(0, 1));
	home->setPosition(50, -50);
	menu1->setAnchorPoint(Point(0, 1));
	menu1->setPosition(0, designHeight);
	
	optionBox->setScale(designWidth/4*3/optionBox->getTextureRect().getMaxX());
	optionBox->setAnchorPoint(Point(0.5, 0.5));
	optionBox->setPosition(designWidth/2, designHeight/2-50);
	menu2->setAnchorPoint(Point(0.5, 0.5));
	auto mid = Point(optionBox->getTextureRect().getMaxX()/2, optionBox->getTextureRect().getMaxY()/2);
	menu2->setPosition(mid);
	music->setPosition(0, 300);
	sound->setPosition(0, -50);
	shack->setPosition(0, -400);
	music->setScale(0.9);
	sound->setScale(0.9);
	shack->setScale(0.9);

	return true;
}

void OptionsScene::onEnter() {
	Scene::onEnter();
}

void OptionsScene::onExit() {
	Scene::onExit();
}