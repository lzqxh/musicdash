#include "InputCenter.h"
#include "tool/Message.h"
#include "DataManager/DataVo.h"

bool InputCenter::init() {
	if (!Layer::init()) return false;
	this->setAccelerometerEnabled(true);
	return true;
}

void InputCenter::onEnter() {
	Layer::onEnter();
}

void InputCenter::onExit() {
	Layer::onExit();
}

