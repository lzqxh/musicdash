#ifndef __OPTIONS_SCENE_H__
#define __OPTIONS_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class OptionsScene : public Scene {
public:
	virtual bool init();
	CREATE_FUNC(OptionsScene);

	virtual void onEnter();
	virtual void onExit();

	static bool ifMusic;
	static bool ifSound;
	static bool ifShack;
};

#endif