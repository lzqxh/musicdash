#ifndef __GAMEWORLD_SCENE_H__
#define __GAMEWORLD_SCENE_H__

#include "cocos2d.h"
#include "Layer\UiLayer.h"
#include "Layer\ScoreLayer.h"
#include "Layer\RoleLayer.h"
#include "Layer\MusicLayer.h"

class GameWorld: public cocos2d::Scene {
private:
	UiLayer *uiLayer;
	ScoreLayer *scoreLayer;
	RoleLayer *roleLayer;
	MusicLayer *musicLayer;

public:
	virtual bool init();
	CREATE_FUNC(GameWorld);
};

#endif // __GAMEWORLD_SCENE_H__
