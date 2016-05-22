#ifndef ZPARAM_REC_H
#define ZPARAM_REC_H

using namespace std;

// monitor resolution
const int DEV_MAX_X_RES = 1023;		// monitor X resolution
const int DEV_MAX_Y_RES = 1023;		// monitor Y resolution
const int DEV_MAX_Z_RES = 255;		// Frame buffer bit-depth; used during rendering

class ZParamRec {
public:
  	double zMin, zMax, zRange;
  	
	ZParamRec(double zMin = 0., double zMax = 0., double zRange = 0.);
};

#endif   // ZPARAM_REC_H
