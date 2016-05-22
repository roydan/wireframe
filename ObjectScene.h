#ifndef OBJECT_SCENE_H
#define OBJECT_SCENE_H

#include <windows.h>
#include <cstdio>
#include <string>
#include <vector>

#include "ObjectCell.h"
#include "MapRec.h"
#include "ViewPointRec.h"
#include "Matrix.h"
#include "ZParamRec.h"

using namespace std;

const string DEFAULT_INI = "wireframe.ini";

/**********************************************************
 *
 * classe ObjectScene
 *
 *********************************************************/

class ObjectScene {
private:
    FILE * sceneFile;
    FILE * objectFile;

    void SetViewTransformation();
	void CalculateScreenCoordinates();

	void MakeSurfacesExt (ObjectCell *);
	void ReadAPolygonExt (int, PolygonCell *, ObjectCell *);
	int AddPolygonToPolygonListExt (PolygonCell *, PolygonList **);
	void ReadVerticesExt (ObjectCell *);

public:
    MapRec	     mapOffsets;
    Matrix       viewTransformation;
    ViewPointRec viewRefPoint;
	vector<ObjectCell *> objectHead;
    bool         drawVertexNormals;
    ZParamRec    zParams;
    bool         sceneChanged;

	ObjectScene (HWND hWnd);

    void SetViewVariables();
	void WireFrameScene (HWND);
	void TransformScene();
    bool LoadScene (HWND, char *, char *);
    bool LoadObjectExt (char *, char *);
};

#endif   // OBJECT_SCENE_H
