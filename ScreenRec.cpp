#include "ScreenRec.h"

/**********************************************************
 *
 * representation d'un vertex sur l'ecran
 *
 *********************************************************/
 
/**********************************************************
 * 
 * ScreenRec
 * 
 * parameters IN:
 * 	int x		default = 0
 *	int y		default = 0
 *	int z		default = 0
 * 
 * return value : ScreenRec *
 * 
 *********************************************************/
ScreenRec::ScreenRec (int x, int y, int z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

/**********************************************************
 * 
 * operator =
 * 
 * parameters IN :
 *	const ScreenRec & value
 * 
 * return value : ScreenRec *
 * 
 *********************************************************/
void ScreenRec::operator = (const ScreenRec & value)
{
  	this->x = value.x;
  	this->y = value.y;
  	this->z = value.z;
}

/**********************************************************
 * 
 * Perspective
 * 
 * parameters IN:
 *	MyVector pos
 *	ViewPointRec viewRefPoint
 *	MapRec mapOffsets
 * 
 **********************************************************
 * 
 * screen coordinates :
 *   x : increase from left to right : Ok with Viewing System I
 *   y : increase from top to bottom : reversed from Viewing System I
 * 
 *********************************************************/
void ScreenRec::Perspective (MyVector pos, ViewPointRec viewRefPoint, MapRec mapOffsets) {
  	x = mapOffsets.GetXMid() + (int)(viewRefPoint.GetViewPlaneDist() * pos.GetX() / pos.GetZ());
  	y = mapOffsets.GetYMid() - (int)(viewRefPoint.GetViewPlaneDist() * pos.GetY() / pos.GetZ());     // was mapOffsets + viewRefPoint
}

