#ifndef __SCORE_LAYER_H__
#define __SCORE_LAYER_H__

#include "cocos2d.h"
#include "DataManager/DataVo.h"
#include "consts/Message.h"
#include "consts/ResolutionConst.h"

#include <vector>

USING_NS_CC;

class ScoreLayer : public Layer {
private:
	CCNode *label;
public:
	virtual bool init();
	CREATE_FUNC(ScoreLayer);
	virtual void onEnter();
	virtual void onExit();
	void displayCombos(EventCustom*);
	static int score;
};
#endif //__SCORE_LAYER_H__
