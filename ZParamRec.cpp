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
 *	const ZParamRec & value
 * 
 *********************************************************/
void ZParamRec::operator = (const ZParamRec & value)
{
  	this->zMin = value.zMin;
  	this->zMax = value.zMax;
  	this->zRange = value.zRange;
}

