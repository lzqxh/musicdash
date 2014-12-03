#ifndef __MUSIC_NOTE_H__
#define __MUSIC_NOTE_H__

#include <vector>
#include "cocos2d.h"

enum NoteType {
	NoteA,  //������� ���������pos = 0����ʾ��ߣ� pos = 1 ��ʾ�ұ�
	NoteB,  //�������� ���������pos = 0, ��ʾ��ߣ� pos = 1 ��ʾ�ұ�; len ��ʾ��������
	NoteC,  //������Ӧ������  ���������pos ʶ��0��1��2
	NoteD   //qte������ ���������path ��ʾ�����켣
};

struct MusicNote {
	NoteType type;
	int noteId;
	int pos, len;
	int time;		//�������ֵ�ʱ���
	float fallTime;	//�����������õ�ʱ��
	std::vector<cocos2d::Vec3> path; //��ά�����ֱ��ʾx, y, t��qte�������꣬�Լ�ʱ���

	MusicNote() {
		pos = len = time = 0;
		fallTime = 1;
		path.clear();
	}
};

#endif //__MUSIC_NOTE_H__