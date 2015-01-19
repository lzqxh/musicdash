#ifndef __HELP_SCENE_H__
#define __HELP_SCENE_H__

#include "cocos2d.h"
#include <vector>
using namespace cocos2d;

class HelpScene : public Scene {
public:
	virtual bool init();
	CREATE_FUNC(HelpScene);

	virtual void onEnter();
	virtual void onExit();
	bool onTouchBegan(Touch *, Event *);
	void onTouchMoved(Touch *, Event *);
	void onTouchEnded(Touch *, Event *);

private:
	std::vector<Sprite *> cards;
	Point start;
	float direction;
	EventListenerTouchOneByOne *tl;
};

#endif