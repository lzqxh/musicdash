#ifndef __UI_LAYER_H__
#define __UI_LAYER_H__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;

class UiLayer : public Layer {
public:
	virtual bool init();
	CREATE_FUNC(UiLayer);
	Menu *pauseButton;
	Menu *controlMenu;
	void showControlMenu(Ref *);
};
#endif //__UI_LAYER_H__