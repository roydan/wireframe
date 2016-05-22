#ifndef MAP_REC_H
#define MAP_REC_H

using namespace std;

class MapRec {
private:
  	int xMid, yMid;

public:
  	MapRec(int xMid = 0, int yMid = 0);
  	int GetXMid();
    int GetYMid();
    void SetXMid(int value);
    void SetYMid(int value);
};

#endif   // MAP_REC_H
