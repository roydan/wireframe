#ifndef ZPARAM_REC_H
#define ZPARAM_REC_H

using namespace std;

// monitor resolution
const int DEV_MAX_X_RES = 1024;		// monitor X resolution
const int DEV_MAX_Y_RES = 1024;		// monitor Y resolution
const int DEV_MAX_Z_RES = 255;		// Frame buffer bit-depth; used during rendering

/**********************************************************
 *
 * classe ZParamRec
 *
 *********************************************************/

class ZParamRec {
private:
  	double zMin, zMax, zRange;
  	
public:
	ZParamRec (double zMin = 0., double zMax = 0., double zRange = 0.);
    void operator = (const ZParamRec& another);

  	double GetZMin() { return zMin; }
    double GetZMax() { return zMax; }
    double GetZRange() { return zRange; }
    void SetZMin (double value) { zMin = value; }
    void SetZMax (double value) { zMax = value; }
    void SetZRange (double value) { zRange = value; }
};

#endif   // ZPARAM_REC_H
