#ifndef OBJECT_SCENE_H
#define OBJECT_SCENE_H

#include <windows.h>
#include <cstdio>
#include <string>
#include <vector>

#include "ObjectCell.h"
#include "MapRec.h"
#include "ViewPointRec.h"
#include "MyMatrix.h"
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
    void SetViewTransformation();
	void CalculateScreenCoordinates();

	void MakeSurfaces (FILE * ptrFile, ObjectCell *);
	void ReadAPolygon (char *, int, PolygonCell *, ObjectCell *);
	bool AddPolygonToPolygonList (PolygonCell *, PolygonList **);
	void ReadVertices (FILE * ptrFile, ObjectCell *);
	
	bool getStringNoComments(FILE * ptrFile, char * s);
	void removeSpaces (char * s);

public:
    MapRec	     mapOffsets;
    MyMatrix     viewTransformation;
    ViewPointRec viewRefPoint;
	vector<ObjectCell *> objectCellList;
    bool         drawVertexNormals;
    ZParamRec    zParams;
    bool         sceneChanged;

	ObjectScene (HWND hWnd);

    void SetViewVariables();
	void WireFrameScene (HWND);
	void TransformScene();
    bool LoadScene (HWND, char *);
    bool LoadObject (char *);
};

#endif   // OBJECT_SCENE_H

