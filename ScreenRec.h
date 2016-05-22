#ifndef SCREEN_REC_H
#define SCREEN_REC_H

#include "Vector.h"
#include "ViewPointRec.h"
#include "MapRec.h"

using namespace std;

/**********************************************************
 *
 * classe ScreenRec
 *
 * representation d'un vertex sur l'ecran
 *
 *********************************************************/

class ScreenRec {
private:
	int x, y, z;

public:
	ScreenRec (int x = 0, int y = 0, int z = 0);
    void operator = (const ScreenRec& another);
	void Perspective (Vector pos, ViewPointRec viewRefPoint, MapRec mapOffsets);

	int GetX() { return x; }
	int GetY() { return y; }
	int GetZ() { return z; }
	void SetX (int value) { x = value; }
	void SetY (int value) { y = value; }
	void SetZ (int value) { z = value; }
};

#endif   // SCREEN_REC_H
