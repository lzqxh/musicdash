#ifndef __SCORE_LAYER_H__
#define __SCORE_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class ScoreLayer : public Layer {
private:
	LabelTTF *label1, *label2;
public:
	virtual bool init();
	CREATE_FUNC(ScoreLayer);
	virtual void onEnter();
	virtual void onExit();
};
#endif //__SCORE_LAYER_H__
