#ifndef __INPUT_CENTER_H__
#define __INPUT_CENTER_H__

#include "cocos2d.h"

USING_NS_CC;

class InputCenter: public Layer {
public:
	virtual bool init();
	CREATE_FUNC(InputCenter);

	virtual void onEnter();
	virtual void onExit();

	virtual bool onTouchBegan(Touch *touch, Event *event) override;
	virtual void onTouchMoved(Touch *touch, Event *event) override;
	virtual void onTouchEnded(Touch *touch, Event *event) override;

private:
	EventListenerTouchOneByOne *listener;
	MotionStreak * ms;
	Point start;
	bool touchStatus; //0为未产生有效touch，1表示已产生有效touch
	std::string lastInput;
};

#endif //__INPUT_CENTER_H__
