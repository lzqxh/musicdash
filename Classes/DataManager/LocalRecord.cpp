#include <string>

#include "LocalRecord.h"

USING_NS_CC;

LocalRecord* LocalRecord::_inst = nullptr;

void LocalRecord::readFromFile() {
	data = CCFileUtils::getInstance()->getValueMapFromFile("localrecord.plist");
}

void LocalRecord::writeToFile() {
	CCFileUtils::getInstance()->writeToFile(data, "localrecord.plist");
}

void LocalRecord::setCoins(int val) {
	data["coins"] = val;
}
int LocalRecord::getCoins() {
	return data["coins"].asInt();
}

void LocalRecord::setHighestRecord(std::string name, int val) {
	int result = data["highest_record"].asValueMap()[name].asInt();
	if (val > result) 
		data["highest_record"].asValueMap()[name] = result = val;	
}

int LocalRecord::getHighestRecord(std::string name) {
	return data["highest_record"].asValueMap()[name].asInt();
}

