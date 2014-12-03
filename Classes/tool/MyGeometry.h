#ifndef __MY_GEOMETRY_H__
#define __MY_GEOMETRY_H__

#include "cocos2d.h"

USING_NS_CC;

class MyGeometry {
public:
	/*
	假设3D情况下，物品放大呈线性关系，物品实际移动速度与其被观察的大小成线性关系
	使用枚举方式模拟积分方程 
	*/
	static Sequence* Move3D(float falltime, Point st, Point ed, float scale1, float scale2);
};
#endif