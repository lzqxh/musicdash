#ifndef __ROLE_LAYER_H__
#define __ROLE_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class RoleLayer : public Layer {
public:
	virtual bool init();
	CREATE_FUNC(RoleLayer);

	virtual void onEnter();
	virtual void onExit();
	void run(EventCustom *);

	Sprite *role;
};
#endif //__ROLE_LAYER_H__
