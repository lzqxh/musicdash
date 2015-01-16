#ifndef __DATA_VO_H__
#define __DATA_VO_H__

#include <string>
#include <queue>

#include "MusicNote.h"

#define game_speed	(DataVo::inst()->speed)
#define curTime		(DataVo::inst()->_curTime)
#define roleStatus	(DataVo::inst()->_roleStatus)

enum RoleStatus {
	Sliding_M,
	Sliding_L,
	Sliding_R,
	Action_L2M,
	Action_M2L,
	Action_R2M,
	Action_M2R,
	Action_JUMP,
	Sliding_UL,
	Sliding_UR,
	Action_M2UL,
	Action_UL2M,
	Action_M2UR,
	Action_UR2M
};
class DataVo {
private:
	void resolve(unsigned char *, int, int);

public:
	static DataVo* _inst;
	static DataVo* inst() {
		if (!_inst) _inst = new DataVo();
		return _inst;
	}

	void load(std::string);
	void updateRecord();
	std::string musicname, musicFile;
	int musicLength;
	std::vector<std::vector<int> > data;

	RoleStatus _roleStatus;
	int actionCount;
	int combos;
	int energyValue;
	int coins, distance;
	int speed, speedX;
	int _curTime;

	//”Œœ∑ÀŸ∂»
	bool isBeerEffectStart;
};

#endif //__DATA_VO_H__