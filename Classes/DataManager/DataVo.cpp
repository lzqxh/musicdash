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

	if (!isdigit(pBuffer[++pos])) return -1;
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
	std::vector<int> v(5, false);
	v[0] = getInt(pBuffer, st) == 1 ? true : false;
	for (int i = st + 1, k = 1; i < ed && k < 5; i++) 
		if (pBuffer[i] == ',') {
			v[k++] = getInt(pBuffer, i + 1) == 1 ? true : false;
		}
	CCLog("%d %d %d %d %d", v[0], v[1], v[2], v[3], v[4]);
	data.push_back(v);
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
