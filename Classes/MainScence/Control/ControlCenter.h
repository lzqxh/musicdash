#ifndef __CONTROL_CENTER_H__
#define __CONTROL_CENTER_H__

#include <vector>
#include "cocos2d.h"

#include "DataManager\MusicNote.h"
#include "DataManager\DataVO.h"

USING_NS_CC;

enum GameStatus {
	gs_playing, 
	gs_pause,
	gs_over
};

class ControlCenter: public CCNode {
private:
	float _accTime;
	std::queue<std::pair<int, std::string> > inputQue;
	std::string lastInput;
	void roleMove();
	void evalution();

	int atLeft();
	int atRight();
	int atRail();
	int atJumping();
	void checkTrafficCone(int index);
	void checkBeer(int index);
	void checkBarrier(int index);
	void checkRailing(int index);
	void checkManholeCover(int index);
	void staticScore(int);

public:
	void receiveInput(EventCustom*);
	GameStatus gameStatus;

	virtual bool init();
	CREATE_FUNC(ControlCenter);


	virtual void onEnter();
	virtual void onExit();

	void fixedUpdate(float);

	void gameStart(EventCustom*);
	void gameOver(EventCustom*);
	void gamePause(EventCustom*);
	void gameResume(EventCustom*);
};

#endif //__CONTROL_CENTER_H__
