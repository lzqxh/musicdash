#ifndef __UI_LAYER_H__
#define __UI_LAYER_H__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;

class UiLayer : public Layer {
public:
	virtual bool init();
	CREATE_FUNC(UiLayer);

	virtual bool onTouchBegan(Touch *touch, Event *event) override;
	virtual void onTouchMoved(Touch *touch, Event *event) override;
	virtual void onTouchEnded(Touch *touch, Event *event) override;
	virtual void update(float dt) override;

private:
	std::vector<Sprite *>ts;
	MotionStreak * ms;
};
#endif //__UI_LAYER_H__