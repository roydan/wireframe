#include "MapRec.h"

/**********************************************************
 * 
 * MapRec
 * 
 * parameters IN:
 * 	int xMid	default = 0
 *	int yMid	default = 0
 * 
 * return value : MapRec *
 * 
 *********************************************************/
MapRec::MapRec (int xMid, int yMid) {
  	this->xMid = xMid;
  	this->yMid = yMid;
}

/**********************************************************
 * 
 * operator =
 * 
 * parameters IN :
 *	const MapRec & value
 * 
 * return value : MapRec *
 * 
 *********************************************************/
void MapRec::operator = (const MapRec & value)
{
  	this->xMid = value.xMid;
  	this->yMid = value.yMid;
}

