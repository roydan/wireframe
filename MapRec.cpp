#include "MapRec.h"

MapRec::MapRec(int xMid, int yMid) {
  	this->xMid = xMid;
  	this->yMid = yMid;
}

int MapRec::GetXMid() { return xMid; }

int MapRec::GetYMid() { return yMid; }

void MapRec::SetXMid(int value) { xMid = value; }

void MapRec::SetYMid(int value) { yMid = value; }
