#ifndef __MUSIC_LAYER_H__
#define __MUSIC_LAYER_H__

#include <vector>

#include "cocos2d.h"

USING_NS_CC;

class MusicLayer : public Layer {
private:
	std::vector<Node*> barrier[5];
	std::vector<Node*> obstacleLR[2];
	void run(EventCustom*);
public:
	virtual bool init();
	CREATE_FUNC(MusicLayer);

	virtual void onEnter();
	virtual void onExit();
};
#endif //__MUSIC_LAYER_H__
