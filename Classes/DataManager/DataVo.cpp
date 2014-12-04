#include <string>

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//#include "tinyxml2.h"
//#include "tinyxml2/tinyxml2.h"
//#else
//#include "tinyxml2\tinyxml2.h"
//#endif


#include "DataVo.h"
#include "DataManager\MusicNote.h"
#include "consts\ResolutionConst.h"
#include "consts\MyConsts.h"

USING_NS_CC;

DataVo* DataVo::_inst = nullptr;

int getInt(unsigned char *pBuffer, int pos) {
	if (!isdigit(pBuffer[pos])) return -1;
	int ret = 0;
	while (isdigit(pBuffer[pos])) 
		ret = ret * 10 + pBuffer[pos++] - '0';
	return ret;
}

float getFloat(unsigned char *pBuffer, int pos) {
	if (!isdigit(pBuffer[pos])) return -1;
	float ret = 0;
	while (isdigit(pBuffer[pos])) 
		ret = ret * 10 + pBuffer[pos++] - '0';
	if (pBuffer[pos++] != '.') return ret;
	float base = 0.1;
	while (isdigit(pBuffer[pos])) {
		ret += (pBuffer[pos++] - '0') * base;
		base /= 10;
	}
	return ret;
}

void DataVo::resolve(unsigned char *pBuffer, int st, int ed) {
	int time = (int)(getFloat(pBuffer, st)*1000), type = -1, last = 0;
	int i = 1;
	for (i = st + 1; i < ed; i++) 
		if (pBuffer[i] == ',') {
			type = getInt(pBuffer, i + 1);
			break;
		}
	for (i = i + 1; i < ed; i++)
		if (pBuffer[i] == ',') {
			last = (int)(getFloat(pBuffer, i + 1) * 1000);
			break;
		}
	if (time < 0 || type < 0) return;
	if (last < 0) last = 0;
	CCLog("%d %d %d", time, type, last);

	int index = time / TIMESLICE_SIZE;
    CCLog("index %d", index);
    while (index >= data.size())
    	data.push_back(std::vector<int>(9, false));
    data[index][type] = 1;
    while ((index + 1) * TIMESLICE_SIZE <= time + last) {
    	index++;
    	CCLog("%d", index);
    	while (index >= data.size())
    		data.push_back(std::vector<int>(9, false));
    	data[index][type] = data[index - 1][type] + 1;
    }
}

void DataVo::load(std::string name) {
	data.clear();
	musicname = name;
	std::string filepath = name + ".csv";

	unsigned char* pBuffer = NULL;
	ssize_t bufferSize = 0;
      
	pBuffer = CCFileUtils::getInstance()->getFileData(filepath, "r", &bufferSize);

	for (int r = 0, l = 0; r < bufferSize; r++) 
		if (pBuffer[r] == '\n') {
			resolve(pBuffer, l, r);
			l = r + 1;
		}
	musicLength = data.size();

	musicFile = name + ".mp3";
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(musicFile.c_str());
}
