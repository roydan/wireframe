#include "ZParamRec.h"

/**********************************************************
 * 
 * ZParamRec
 * 
 * parameters IN:
 * 	double zMin		default = 0
 *	double zMax		default = 0
 *	double zRange	default = 0
 * 
 * return value : ZParamRec *
 * 
 *********************************************************/
ZParamRec::ZParamRec (double zMin, double zMax, double zRange) {
	this->zMin = zMin;
	this->zMax = zMax;
	this->zRange = zRange;
}

/**********************************************************
 * 
 * operator =
 * 
 * parameters IN :
 *	const ZParamRec & another
 * 
 *********************************************************/
void ZParamRec::operator = (const ZParamRec & another)
{
  	this->zMin = another.zMin;
  	this->zMax = another.zMax;
  	this->zRange = another.zRange;
}
