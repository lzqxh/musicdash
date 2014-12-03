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
	std::string lastInput;
	void roleMove();
	void evalution();

public:
	void receiveInput(EventCustom*);
	GameStatus gameStatus;
	Menu *pauseButton;
	Menu *controlMenu;

	virtual bool init();
	CREATE_FUNC(ControlCenter);


	virtual void onEnter();
	virtual void onExit();

	void fixedUpdate(float);

	void gameStart();
	void gameOver();

	void showControlMenu(Ref *);
};

#endif //__CONTROL_CENTER_H__
