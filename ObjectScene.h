#ifndef OBJECT_SCENE_H
#define OBJECT_SCENE_H

#include <windows.h>
#include <cstdio>
#include <string>

#include "ObjectCell.h"
#include "MapRec.h"
#include "ViewPointRec.h"
#include "Matrix.h"
#include "ZParamRec.h"

using namespace std;

const string DEFAULT_INI = "wireframe.ini";

class ObjectScene {
private:
	void MakeSurfaces(ObjectCell *);
	void ReadPolygons(int, SurfaceCell *, ObjectCell *);
	void ReadAPolygon(int, PolygonCell *, ObjectCell *);
	int AddPolygonToPolygonList(PolygonCell *, PolygonList **);
	void ReadVertices(ObjectCell *);

public:
    MapRec	     mapOffsets;
    Matrix       viewTransformation;
    ViewPointRec viewRefPoint;
	ObjectCell * objectHead;
    bool         drawVertexNormals;
    ZParamRec    zParams;
    bool         sceneChanged;

  	int          noOfObjects;
    FILE *       sceneFile;
    FILE *       objectFile;

	void WireFrameScene(HWND);
	void CalculateScreenCoordinates();

	ObjectScene(HWND hWnd);
    void SetViewVariables();
    void SetViewTransformation();
	void TransformScene();
    bool LoadScene(HWND, char *, char *);
    bool LoadObject(char *, char *);
};

#endif   // OBJECT_SCENE_H
