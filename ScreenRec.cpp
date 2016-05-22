#include "ScreenRec.h"

// constructeur
ScreenRec::ScreenRec(int x, int y, int z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void ScreenRec::Perspective(Vector pos, ViewPointRec viewRefPoint, MapRec mapOffsets) {
  	x = mapOffsets.GetXMid() + (int)(viewRefPoint.viewPlaneDist * pos.GetX() / pos.GetZ());
  	y = mapOffsets.GetYMid() + (int)(viewRefPoint.viewPlaneDist * pos.GetY() / pos.GetZ());
}
