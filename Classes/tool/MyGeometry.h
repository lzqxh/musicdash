#ifndef __MY_GEOMETRY_H__
#define __MY_GEOMETRY_H__

#include "cocos2d.h"

USING_NS_CC;

class MyGeometry {
public:
	/*
	����3D����£���Ʒ�Ŵ�����Թ�ϵ����Ʒʵ���ƶ��ٶ����䱻�۲�Ĵ�С�����Թ�ϵ
	ʹ��ö�ٷ�ʽģ����ַ��� 
	*/
	static Sequence* Move3D(float falltime, Point st, Point ed, float scale1, float scale2);
};
#endif