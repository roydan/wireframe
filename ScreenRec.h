#ifndef SCREEN_REC_H
#define SCREEN_REC_H

#include "Vector.h"
#include "ViewPointRec.h"
#include "MapRec.h"

using namespace std;

// representation d'un vertex sur l'ecran
class ScreenRec {
private:
	int x, y, z;

public:
	ScreenRec(int x = 0, int y = 0, int z = 0);
	int GetX() { return x; }
	int GetY() { return y; }
	int GetZ() { return z; }
	void SetX(int value) { x = value; }
	void SetY(int value) { y = value; }
	void SetZ(int value) { z = value; }

	void Perspective(Vector pos, ViewPointRec viewRefPoint, MapRec mapOffsets);
};

#endif   // SCREEN_REC_H
