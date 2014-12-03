#ifndef __UI_LAYER_H__
#define __UI_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class UiLayer : public Layer {
public:
	virtual bool init();
	CREATE_FUNC(UiLayer);
};
#endif //__UI_LAYER_H__