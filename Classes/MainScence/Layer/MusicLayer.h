#ifndef __MUSIC_LAYER_H__
#define __MUSIC_LAYER_H__

#include <vector>

#include "cocos2d.h"

USING_NS_CC;

class MusicLayer : public Layer {
private:
	std::vector<Node*> objects;
	std::vector<Node*> roads;

	Texture2D* roadTexture;
	void run(EventCustom*);
	void addNote(EventCustom*);
//	void dispEffect(EventCustom*);
	void explode(EventCustom*);
	void explodeAll(EventCustom*);

	void roadMove();
	void addTrafficCone(int index);
	void addBeer(int index);
	void addBarrier(int index);
	void addRailing(int index);
	void addManholeCover(int index);
	void addBuilding();

public:
	std::string skinPath;
	virtual bool init();
	static MusicLayer* create(std::string);
//	CREATE_FUNC(MusicLayer);

	virtual void onEnter();
	virtual void onExit();
};
#endif //__MUSIC_LAYER_H__
