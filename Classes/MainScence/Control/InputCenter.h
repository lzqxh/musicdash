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

};

#endif //__INPUT_CENTER_H__
