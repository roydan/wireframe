#ifndef MAP_REC_H
#define MAP_REC_H

using namespace std;

class MapRec {
private:
  	int xMid, yMid;

public:
  	MapRec (int xMid = 0, int yMid = 0);
    void operator = (const MapRec & another);
  	
  	int GetXMid() { return xMid; }
    int GetYMid() { return yMid; }
    void SetXMid (int value) { xMid = value; }
    void SetYMid (int value) { yMid = value; }
};

#endif   // MAP_REC_H

