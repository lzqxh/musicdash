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
	gs_over,
	gs_counting
};

class ControlCenter: public CCNode {
private:
	float _accTime;
	std::queue<std::pair<int, std::string> > inputQue;
	std::string lastInput;
	Sprite* _countSprite;
	void roleMove();

	int evaluateLim;
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

	void staticDis();

public:
	void receiveInput(EventCustom*);
	GameStatus gameStatus;

	virtual bool init();
	CREATE_FUNC(ControlCenter);


	virtual void onEnter();
	virtual void onExit();

	//main loop
	void fixedUpdate(float);

	//game status control
	void gameStart(EventCustom*);
	void gameOver(EventCustom*);
	void gamePause(EventCustom*);
	void gameResume(EventCustom*);


	//input from uiLayer
	void clickBeer(EventCustom*);
	void beerEffectStart();
	void beerEffectStop();
};

#endif //__CONTROL_CENTER_H__
