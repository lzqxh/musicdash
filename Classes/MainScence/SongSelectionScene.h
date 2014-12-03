#ifndef __SONGSELECTIONSCENE_H__
#define __SONGSELECTIONSCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class SongSelectionScene : public cocos2d::Scene {
public:
	virtual bool init();
	CREATE_FUNC(SongSelectionScene);

	virtual void onEnter();
	bool onTouchBegan( Touch *, Event *);
	void onTouchEnded( Touch *, Event *);

private:
	Layer *bg;
	Menu *menu;
	std::vector<Sprite *> songs;
	Point beginPoint;
	int focus;
};

#endif