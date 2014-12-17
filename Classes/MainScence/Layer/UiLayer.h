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

	Sprite *progress;
	Sprite *head;
	Sprite *energy;
	int energyValue;

	virtual void onEnter();
	virtual void onExit();

	void update(float dt) override;
	void energyUp(EventCustom *);

	Sprite *scoreBox;
	LabelBMFont *score;
	void displayScore(EventCustom*);
};
#endif //__UI_LAYER_H__