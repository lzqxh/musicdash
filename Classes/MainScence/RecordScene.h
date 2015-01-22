#ifndef __RECORD_SCENE_H__
#define __RECORD_SCENE_H__

#include "cocos2d.h"
#include <vector>
using namespace cocos2d;

class RecordScene : public Scene {
public:
	virtual bool init();
	CREATE_FUNC(RecordScene);

	virtual void onEnter();
	virtual void onExit();

private:
};

#endif
