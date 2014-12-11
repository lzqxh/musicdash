#ifndef __SCORE_LAYER_H__
#define __SCORE_LAYER_H__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;

class ScoreLayer : public Layer {
private:
	std::vector <Sprite *> scoreSprite;
	int score;
public:
	virtual bool init();
	CREATE_FUNC(ScoreLayer);
	virtual void onEnter() override;
	virtual void onExit() override;
	void displayScore(EventCustom *event);
};
#endif //__SCORE_LAYER_H__
