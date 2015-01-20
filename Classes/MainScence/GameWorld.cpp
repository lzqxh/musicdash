#include "GameWorld.h"
#include "DataManager\DataVo.h"
#include "Control\ControlCenter.h"

using namespace cocos2d;

bool GameWorld::init() {
	if ( !Scene::init()) return false;


	musicLayer = MusicLayer::create(DataVo::inst()->skinPath);
	this->addChild(musicLayer);

	roleLayer = RoleLayer::create();
	this->addChild(roleLayer);

	scoreLayer = ScoreLayer::create();
	this->addChild(scoreLayer);

	uiLayer = UiLayer::create();
	this->addChild(uiLayer);

	auto controlCenter = ControlCenter::create();
	addChild(controlCenter);

	return true;
}



