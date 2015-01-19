#ifndef __LOGINSCENE_H__
#define __LOGINSCENE_H__

#include "cocos2d.h"
#include "ui\UIButton.h"
using namespace cocos2d;

class LoginScene : public Scene {
public:
	virtual bool init();
	CREATE_FUNC(LoginScene);

	virtual void onEnter();
	void onStartButtonClicked(Ref *);
	void onOptionsButtonClicked(Ref *);
	void onHelpButtonClicked(Ref *);

private:
	Layer *layer;
	Menu *mainMenu;
};

#endif
