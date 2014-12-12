#ifndef __ROLE_LAYER_H__
#define __ROLE_LAYER_H__

#include "cocos2d.h"
#include <string>

USING_NS_CC;

class RoleLayer : public Layer {
public:
	virtual bool init();
	CREATE_FUNC(RoleLayer);

	virtual void onEnter();
	virtual void onExit();
	void run(EventCustom *);
	void roleEffect(EventCustom *);

	Sprite *role;

private:
	std::string frameName;
	std::string nextFrameName(std::string);
	Vec2 nextM2ULPosition(int);
	Vec2 nextM2URPosition(int);
	int eventCount;
};
#endif //__ROLE_LAYER_H__
