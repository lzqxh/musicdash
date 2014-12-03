#ifndef __MUSIC_NOTE_H__
#define __MUSIC_NOTE_H__

#include <vector>
#include "cocos2d.h"

enum NoteType {
	NoteA,  //打击音符 有意义变量pos = 0，表示左边， pos = 1 表示右边
	NoteB,  //长按音符 有意义变量pos = 0, 表示左边， pos = 1 表示右边; len 表示音符长度
	NoteC,  //重力感应音符，  有意义变量pos 识别0，1，2
	NoteD   //qte音符， 有意义变量path 表示音符轨迹
};

struct MusicNote {
	NoteType type;
	int noteId;
	int pos, len;
	int time;		//音符出现的时间点
	float fallTime;	//音符掉落所用的时间
	std::vector<cocos2d::Vec3> path; //三维向量分别表示x, y, t，qte出现坐标，以及时间点

	MusicNote() {
		pos = len = time = 0;
		fallTime = 1;
		path.clear();
	}
};

#endif //__MUSIC_NOTE_H__