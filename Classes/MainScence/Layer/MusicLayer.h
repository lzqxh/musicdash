#ifndef __MUSIC_LAYER_H__
#define __MUSIC_LAYER_H__

#include <vector>

#include "cocos2d.h"

USING_NS_CC;

class MusicLayer : public Layer {
private:
	std::vector<Node*> objects;
	void run(EventCustom*);
	void addTrafficCone(int index);
	void addBeer(int index);
	void addBarrier(int index);
	void addRailing(int index);
	void addManholeCover(int index);
public:
	virtual bool init();
	CREATE_FUNC(MusicLayer);

	virtual void onEnter();
	virtual void onExit();
};
#endif //__MUSIC_LAYER_H__
