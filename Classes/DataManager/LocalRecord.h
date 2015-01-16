#ifndef __LOCAL_RECORD_H__
#define __LOCAL_RECORD_H__

#include <string>

#include "cocos2d.h"

class LocalRecord {
private:
	static LocalRecord* _inst;
	LocalRecord() {
		readFromFile();
	}
public:
	cocos2d::ValueMap data;
	static LocalRecord* inst() {
		if (!_inst) _inst = new LocalRecord();
		return _inst;
	}

	void readFromFile();
	void writeToFile();

	void setCoins(int);
	int getCoins();
	void setHighestRecord(std::string, int);
	int getHighestRecord(std::string);
};

#endif