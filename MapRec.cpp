#include "MapRec.h"

/**********************************************************
 * MapRec
 * 
 * constructeur
 * 
 * parameters IN:
 * 	int xMid	default = 0
 *	int yMid	default = 0
 * 
 * return value : MapRec *
 *********************************************************/
MapRec::MapRec (int xMid, int yMid) {
  	this->xMid = xMid;
  	this->yMid = yMid;
}

/**********************************************************
 * operator =
 * 
 * parameters IN :
 *	const MapRec& another
 * 
 * return value : none
 *********************************************************/
void MapRec::operator = (const MapRec& another)
{
  	this->xMid = another.xMid;
  	this->yMid = another.yMid;
}
