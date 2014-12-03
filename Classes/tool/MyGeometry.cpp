#include "MyGeometry.h"

USING_NS_CC;
/*
假设3D情况下，物品放大呈线性关系，物品实际移动速度与其被观察的大小成线性关系
使用枚举方式模拟积分方程 
*/
Sequence* MyGeometry::Move3D(float falltime, Point st, Point ed, float scale1, float scale2) {
	int k = 25;
	float dScale = (scale2 - scale1) / k;
	Point dDist = (ed - st) / ((scale1 + scale2 - dScale) / 2 * k);

	Sequence * reslut = Sequence::create(
		ScaleTo::create(0, scale1),
		MoveTo::create(0, st),
		nullptr);
	for (int i = 0; i < k; i++) {
		st += dDist * (scale1 + i * dScale);
		reslut = Sequence::create(
			reslut,
			Spawn::create(
				ScaleTo::create(falltime / k, scale1 + i * dScale),
				MoveTo::create(falltime / k, st),
//				CallFunc::create([&](){CCLog("test %.3f",dScale );}),
				nullptr),
	//		DelayTime::create(falltime / k),
			nullptr);
	}
	return reslut;
}