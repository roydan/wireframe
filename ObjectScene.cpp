#include <windows.h>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <vector>

#include "ObjectScene.h"
#include "graphics3d.h"
#include "wireframe.h"
#include "Util.h"
#include "wireframe_app.h"

const double NSF = 30.0;			// Normal Scale Factor

const double HIGH_DOUBLE = 1.E30;

#define AXIS_RED	0
#define AXIS_GREEN	1
#define AXIS_BLUE	2

/**********************************************************
 * 
 * ObjectScene
 * 
 * parameters IN:
 * 	HWND hWnd
 * 
 * return value : ObjectScene *
 * 
 *********************************************************
 * 
 * TODO
 * polygon number 1..last
 * surface number
 * 
 *********************************************************/
ObjectScene::ObjectScene (HWND hWnd) {
  	RECT rect;
  	GetClientRect (hWnd, &rect);
	int xMid = (int)((rect.right - rect.left) / 2);
	int yMid = (int)((rect.bottom - rect.top) / 2);
	mapOffsets = MapRec (xMid, yMid);

	// TODO validate GetPrivateProfileInt
	
	char strDefault [MAX_STRING_LENGTH];
	char inBuf [MAX_STRING_LENGTH];
	int intValue;
			
	sprintf (strDefault, "%d", 1000);
 	GetPrivateProfileString ("VIEWING_INFORMATION", "ViewPlaneDistance", strDefault, inBuf, sizeof(inBuf), ".\\wireframe.ini");
  	sscanf (inBuf, "%d", &intValue);
  	int viewPlaneDist = intValue;

	sprintf (strDefault, "%d", 5000);
 	GetPrivateProfileString ("VIEWING_INFORMATION", "Rho", strDefault, inBuf, sizeof(inBuf), ".\\wireframe.ini");
  	sscanf (inBuf, "%d", &intValue);
  	int rho = intValue;

	sprintf (strDefault, "%d", 45);
 	GetPrivateProfileString ("VIEWING_INFORMATION", "Theta", strDefault, inBuf, sizeof(inBuf), ".\\wireframe.ini");
  	sscanf (inBuf, "%d", &intValue);
  	int theta = intValue;

	sprintf (strDefault, "%d", 75);
 	GetPrivateProfileString ("VIEWING_INFORMATION", "Phi", strDefault, inBuf, sizeof(inBuf), ".\\wireframe.ini");
  	sscanf (inBuf, "%d", &intValue);
  	int phi = intValue;

    viewRefPoint = ViewPointRec (viewPlaneDist, rho, theta, phi);

  	SetViewVariables();

  	// draw vertex normals
	// TODO validate GetPrivateProfileInt

	sprintf (strDefault, "%d", (int)false);
 	GetPrivateProfileString ("VIEWING_INFORMATION", "DrawVertexNormals", strDefault, inBuf, sizeof(inBuf), ".\\wireframe.ini");
  	sscanf (inBuf, "%d", &intValue);
  	drawVertexNormals = (bool)intValue;

    sceneChanged = false;

  	objectCellList.clear();
}

/**********************************************************
 * 
 * SetViewVariables
 * 
 *********************************************************/
void ObjectScene::SetViewVariables() {
  	viewRefPoint.SetViewVariables (viewTransformation);
}

/**********************************************************
 * 
 * SetViewTransformation
 * 
 *********************************************************/
void ObjectScene::SetViewTransformation() {
  	viewRefPoint.SetViewTransformation (viewTransformation);
}

/**********************************************************
 * 
 * WireFrameScene
 * 
 * parameters IN :
 *	HWND hWnd
 * 
 *********************************************************/
void ObjectScene::WireFrameScene (HWND hWnd) {
  	DEV_COLOR wireColor = DEV_COLOR (0, 180, 130);
  	DEV_COLOR normalColor = DEV_COLOR (200, 0, 0);
  	HPEN      hPen, hPenOld;	

  	HDC hDC = GetDC(hWnd);

	for (vector<ObjectCell *>::iterator it = objectCellList.begin(); it != objectCellList.end(); ++it) {
		ObjectCell * currentObject = *it;
  	  	switch (currentObject->type) {

  	  	  	case OBJECT_AXIS:
            {
				int index = 0;
				for (vector<VertexCell *>::iterator it = currentObject->vertexCellList.begin(); it != currentObject->vertexCellList.end(); ++it, ++index) {
  	  	  	  	  	switch (index) {
  	  	  	  	  	  	case AXIS_RED:
							hPen = CreatePen (PS_SOLID, 0, RGB (255,0,0));
  	  	  	  	  	  	  	break;

  	  	  	  	  	  	case AXIS_GREEN:
							hPen = CreatePen (PS_SOLID, 0, RGB (0,255,0));
  	  	  	  	  	  	  	break;

  	  	  	  	  	  	case AXIS_BLUE:
						    hPen = CreatePen (PS_SOLID, 0, RGB (0,0,255));
  	  	  	  	  	    	break;

  	  	  	  	  	  	default:
							hPen = CreatePen (PS_SOLID, 0, RGB (0,0,0));
  	  	  	  	  	}

					hPenOld = (HPEN) SelectObject (hDC, hPen);

					// first point
					VertexCell * currentVertex = *it;
					MoveToEx (hDC, currentVertex->screenPos.GetX(), currentVertex->screenPos.GetY(), NULL);

					// second point
					++it;
					currentVertex = *it;
					LineTo (hDC, currentVertex->screenPos.GetX(), currentVertex->screenPos.GetY());

					SelectObject (hDC, hPenOld);
					DeleteObject (hPen);
				}
            }
  	  	    break;

  	  	  	case OBJECT_SURFACE:
            {
                for (vector<SurfaceCell *>::iterator it = currentObject->surfaceCellList.begin(); it != currentObject->surfaceCellList.end(); ++it) {
  	  	  	  	    SurfaceCell * currentSurface = *it;
                    
                    for (vector<PolygonCell *>::iterator it = currentSurface->polygonCellList.begin(); it != currentSurface->polygonCellList.end(); ++it) {
  	  	  	  	        PolygonCell * currentPolygon = *it;

  	  	  	  	  	  	if (currentPolygon->polyVisible) {
							// use currentObject->devColor instead of wireColor
							hPen = CreatePen (PS_SOLID, 0, RGB (currentObject->devColor.r, currentObject->devColor.g, currentObject->devColor.b));
							hPenOld = (HPEN) SelectObject (hDC, hPen);

                            VertexList * vertexList = currentPolygon->vertexListHead;
							ScreenRec startPos = vertexList->vertex->screenPos;
							MoveToEx (hDC, startPos.GetX(), startPos.GetY(), NULL);

							vertexList = vertexList->next;
							while (vertexList != NULL) {
								VertexCell * currentVertex = vertexList->vertex;
							  	LineTo (hDC, currentVertex->screenPos.GetX(), currentVertex->screenPos.GetY());
							  	vertexList = vertexList->next;
							}

							LineTo (hDC, startPos.GetX(), startPos.GetY());

							SelectObject (hDC, hPenOld);
							DeleteObject (hPen);

  	  	  	  	  	  	  	if (drawVertexNormals) {
								hPen = CreatePen (PS_SOLID, 0, RGB (normalColor.r, normalColor.g, normalColor.b));
								hPenOld = (HPEN) SelectObject (hDC, hPen);

  	  	  	  	  	  	  	  	VertexList * vertexList = currentPolygon->vertexListHead;
								while (vertexList != NULL) {
  									VertexCell * currentVertex = vertexList->vertex;

									MoveToEx (hDC, currentVertex->screenPos.GetX(), currentVertex->screenPos.GetY(), NULL);

  	                                MyVector tw;
  	  	  	  	  	  	  	    	tw.SetX (currentVertex->worldPos.GetX() + (NSF * currentVertex->vertexNormal.GetX()));
  	  	  	  	  	  	  	    	tw.SetY (currentVertex->worldPos.GetY() + (NSF * currentVertex->vertexNormal.GetY()));
  	  	  	  	  	  	  	    	tw.SetZ (currentVertex->worldPos.GetZ() + (NSF * currentVertex->vertexNormal.GetZ()));

  	                                MyVector ts;
  	  	  	  	  	  	  	    	ts.VectorMatrix (tw, viewTransformation);

  	                                ScreenRec tPos;
  	  	  	  	  	  	  	    	tPos.Perspective (ts, viewRefPoint, mapOffsets);			// tPos = (tx, ty)

								    LineTo (hDC, tPos.GetX(), tPos.GetY());

								    vertexList = vertexList->next;
  	  	  	  	  	  	  	  	}
  	  	  	  	  	  	  	  	
								SelectObject (hDC, hPenOld);
								DeleteObject (hPen);
  	  	  	  	  	  	  	}
  	  	  	  	  	  	}
  	  	  	  	  	}
  	  	  	  	}
            }
  	  	  	break;

  	  	  	case OBJECT_PATCH:
            {
            	// TODO
            }
  	  	  	break;
  	  	}
  	}
}

/**********************************************************
 * 
 * TransformScene
 * 
 *********************************************************/
void ObjectScene::TransformScene() {
  	zParams = ZParamRec (HIGH_DOUBLE, -HIGH_DOUBLE, 0.);

	for (vector<ObjectCell *>::iterator it = objectCellList.begin(); it != objectCellList.end(); ++it) {
		ObjectCell * currentObject = *it;
  	  	currentObject->TransformToWorldCoordinates();
  	  	currentObject->CalculateNormals();
  	  	currentObject->RemoveHiddenSurfaces();
	  	currentObject->CalculateEyeCoordinates();
	}

  	zParams.SetZRange (zParams.GetZMax() - zParams.GetZMin());
  	CalculateScreenCoordinates();     //++ mapOffsets, viewRefPoint
  	sceneChanged = false;
}

/**********************************************************
 * 
 * CalculateScreenCoordinates
 * 
 *********************************************************/
void ObjectScene::CalculateScreenCoordinates() {
	for (vector<ObjectCell *>::iterator it = objectCellList.begin(); it != objectCellList.end(); ++it) {
		ObjectCell * currentObject = *it;
  	  	switch (currentObject->type) {

  	  	  	case OBJECT_AXIS:
            {
				for (vector<VertexCell *>::iterator it = currentObject->vertexCellList.begin(); it != currentObject->vertexCellList.end(); ++it) {
					VertexCell * currentVertex = *it;
					currentVertex->screenPos.Perspective (currentVertex->eyePos, viewRefPoint, mapOffsets);
					currentVertex->screenPos.SetZ ((int)((currentVertex->eyePos.GetZ() - zParams.GetZMin()) / zParams.GetZRange() * DEV_MAX_Z_RES));
  	  	  	  	}
            }
  	  	  	break;

  	  	  	case OBJECT_SURFACE:
            {
                for (vector<SurfaceCell *>::iterator it = currentObject->surfaceCellList.begin(); it != currentObject->surfaceCellList.end(); ++it) {
  	  	  	  	    SurfaceCell * currentSurface = *it;

                    for (vector<PolygonCell *>::iterator it = currentSurface->polygonCellList.begin(); it != currentSurface->polygonCellList.end(); ++it) {
  	  	  	  	        PolygonCell * currentPolygon = *it;

  	  	  	  	  	  	if (currentPolygon->polyVisible) {
  							VertexList * vertexList = currentPolygon->vertexListHead;
  	  	  	  	  	  	  	while (vertexList != NULL) {
  								VertexCell * currentVertex = vertexList->vertex;
								currentVertex->screenPos.Perspective (currentVertex->eyePos, viewRefPoint, mapOffsets);
								currentVertex->screenPos.SetZ ((int)((currentVertex->eyePos.GetZ() - zParams.GetZMin()) / zParams.GetZRange() * DEV_MAX_Z_RES));
  	  	  	  	  	  	  	  	vertexList = vertexList->next;
  	  	  	  	  	  	  	}
  	  	  	  	  	  	}
  	  	  	  	  	}
  	  	  	  	}
            }
  	  	  	break;

  	  	  	case OBJECT_PATCH:
            {
            	// TODO
            }
  	  	  	break;
  	  	}
  	}
}

/**********************************************************
 * 
 * LoadScene
 * 
 * parameters IN :
 *	HWND hWnd
 *	char * fileName
 *	char * sceneName
 * 
 * return value : bool
 *					true if error
 * 
 **********************************************************
 * 
 * data structure for scene file (example)
 *
 *						viewing information
 *  1					1 = perspective, 0 = parallel
 * -7000				view distance
 *  10000				perspective distance
 * -0.7	-0.4 -0.3	  	view plane normal
 *  0.0	 0.0  1.0		view up vector
 *  0					draw vertex normals
 *							0 = false, 1 = true
 *
 *  4					number of objects
 *
 *  cube.obj			object data filename
 * 						object transformations
 *    0.4  0.4  0.4		scale vector
 *    0.0  0.0  0.0		rotate vector
 *    0.5  1.7 -0.6		translate vector
 * 
 **********************************************************
 * 
 * data structure for scene file (implemented)
 * 
 * view distance			ViewPointRec.viewPlaneDist
 * rho						ViewPointRec.rho
 * theta					ViewPointRec.theta
 * phi						ViewPointRec.phi
 * draw vertex normals		bool drawVertexNormals
 *
 * object data filename
 * object transformations	MyMatrix transformation
 * 		scale vector
 *		rotate vector
 *		translate vector
 * object color
 *		rgb vector
 * 
 *********************************************************/
bool ObjectScene::LoadScene (HWND hWnd, char * fileName, char * sceneName) {
  	FILE * sceneFile = fopen (fileName, "r");
  	if (sceneFile == NULL) return true;

    char objectFilename[MAX_STRING_LENGTH];
  	char mystring [MAX_STRING_LENGTH];
    int intValue;
  	double tempX, tempY, tempZ;
	
	// viewing information
	
	if (getStringNoComments(sceneFile, mystring)) {
	  	sscanf (mystring, "%d", &intValue);
  		viewRefPoint.SetViewPlaneDist (intValue);
	}
		  	
	if (getStringNoComments(sceneFile, mystring)) {
	  	sscanf (mystring, "%d", &intValue);
  		viewRefPoint.SetRho (intValue);
	}
		  	
	if (getStringNoComments(sceneFile, mystring)) {
	  	sscanf (mystring, "%d", &intValue);
  		viewRefPoint.SetTheta (intValue);
	}
		  	
	if (getStringNoComments(sceneFile, mystring)) {
	  	sscanf (mystring, "%d", &intValue);
  		viewRefPoint.SetPhi (intValue);
	}
	
	if (getStringNoComments(sceneFile, mystring)) {
	  	sscanf (mystring, "%d", &intValue);
	  	drawVertexNormals = (bool)intValue;
	}
	
    DrawVertexNormalsCtrl (hWnd);

  	SetViewVariables();

    int i = 0;

	bool found = false;
	do {
		found = getStringNoComments(sceneFile, mystring);
		if (found) {
			
			// object data

		  	sscanf (mystring, "%s", &objectFilename);

		  	MyVector scale, rotate, translate;
		  	
			if (getStringNoComments(sceneFile, mystring)) {
	  			sscanf (mystring, "%lf %lf %lf", &tempX, &tempY, &tempZ);
		  	    scale = MyVector (tempX, tempY, tempZ);
	  		}
	
			if (getStringNoComments(sceneFile, mystring)) {
	  			sscanf (mystring, "%lf %lf %lf", &tempX, &tempY, &tempZ);
		  	    rotate = MyVector (tempX, tempY, tempZ);
	  		}
	
			if (getStringNoComments(sceneFile, mystring)) {
	  			sscanf (mystring, "%lf %lf %lf", &tempX, &tempY, &tempZ);
		  	    translate = MyVector (tempX, tempY, tempZ);
	  		}
	
	  	    DEV_COLOR aColor_scn = DEV_COLOR();
			if (getStringNoComments(sceneFile, mystring)) {
	  			sscanf (mystring, "%d %d %d", &aColor_scn.r, &aColor_scn.g, &aColor_scn.b);
	  		}
	  	    
            char * ptr = strstr(objectFilename, ".obj");
            if (ptr != NULL) {
	            // load an object
    			char objectName[MAX_STRING_LENGTH];
    			int len = ptr - objectFilename;
    			strncpy(objectName, objectFilename, len);
    			objectName[len] = '\0';
	            
	  	  	  	if (LoadObject (objectFilename, objectName) == false) {
	  		    	ObjectCell * currentObject = objectCellList [i];
	
	               	currentObject->GetTransformation (SCALING, scale);
	               	currentObject->GetTransformation (ROTATION, rotate);
	               	currentObject->GetTransformation (TRANSLATION, translate);
	
	               	if (!((aColor_scn.r == 0) && (aColor_scn.g == 0) && (aColor_scn.b == 0))) {
						// if (0,0,0) then use default
	  	              	currentObject->devColor = aColor_scn;
	               	}
	
	  	  	    	sceneChanged = true;
				}
	        }
	
	        i++;
		}
	} while (found);
    
  	fclose (sceneFile);
  	return false;
}

 /*********************************************************
 * 
 * ignoreComments
 * 
 * parameters IN :
 *	const char * s
 * 
 * return value : bool
 *					true if found line without comments
 * 
 *********************************************************/
bool ObjectScene::getStringNoComments(FILE * ptrFile, char * s) {
  	if (ptrFile == NULL) return false;
  	
	bool found = false;
	bool exitLoop = false;
	do {
	  	if (fgets (s, MAX_STRING_LENGTH, ptrFile) != NULL) {
	  		removeSpaces(s);
		  	char * ptrchar = strstr (s, "!");
		  	if ((ptrchar == NULL) || (ptrchar != s)) {
		  		// comment not found OR comment not first character
		  		found = true;
				exitLoop = true;
			}
		} else {
			// read past end-of-file
			exitLoop = true;
		}
	} while (!exitLoop);
	
	return found;
}

void ObjectScene::removeSpaces (char * s) {
	do {
	    if ((*s == '\0') || (!isspace (*s))) {
	    	break;
		} else {
			s++;
		}
	} while (true);
}

/**********************************************************
 * 
 * LoadObject
 * 
 * parameters IN :
 *	char * fileName
 *	char * objectName
 * 
 * return value : bool
 *					true if fail
 * 
 **********************************************************
 * 
 * object data file structure (example)
 * 
 * number of vertices
 * number of polygons
 * number of surfaces
 *
 * vertex information
 * 		vertex number
 *		vertex x y z
 *
 * polygon information
 * 		polygon number
 *		surface number
 *		number of vertices
 *		list of vertex numbers
 *
 **********************************************************
 * 
 * object data file structure (implemented)
 * 
 * object type		0 = TBD  1 = TBD  2 = TBD
 *
 * vertex information
 * 		vertex number
 *			vertex number = 0 -> end-of-vertex-information
 *		vertex x y z
 *
 * polygon information
 *		surface number
 * 		polygon number
 *		number of vertices
 *		list of vertex numbers
 * 
 * object color
 *		rgb vector
 *
 *********************************************************/
bool ObjectScene::LoadObject (char * fileName, char * objectName) {
  	FILE * objectFile = fopen (fileName, "r");
  	if (objectFile == NULL) return true;

  	ObjectCell * currentObject = new ObjectCell (this);

  	currentObject->name = objectName;

  	char mystring [MAX_STRING_LENGTH];
  	
   	// read currentObject type
	if (getStringNoComments(objectFile, mystring)) {
	  	sscanf (mystring, "%d", &(currentObject->type));
  	}

  	ReadVertices (objectFile, currentObject);

  	MakeSurfaces (objectFile, currentObject);

   	// read color
  	DEV_COLOR aColor = DEV_COLOR (0, 180, 130);
	if (getStringNoComments(objectFile, mystring)) {
		sscanf (mystring, "%d %d %d", &aColor.r, &aColor.g, &aColor.b);
	}
  	currentObject->devColor = aColor;

	// default transformation
  	currentObject->transformation.Identity();

  	objectCellList.push_back (currentObject);              // add to end of vector

  	fclose (objectFile);
  	return false;
}

/**********************************************************
 * 
 * MakeSurfaces
 * 
 * parameters IN :
 *	ObjectCell * currentObject
 * 
 *********************************************************/
void ObjectScene::MakeSurfaces (FILE * ptrFile, ObjectCell * currentObject)
{
    SurfaceCell * currentSurface;
  	int surfaceId, currSurfaceId = 0;
  	int polygonId;
	
  	char mystring [MAX_STRING_LENGTH];
  	char * ptr;
  	int count;
  	
  	do {
		if (getStringNoComments(ptrFile, mystring)) {
		  	sscanf (mystring, "%d%n", &surfaceId, &count);
  			ptr = mystring;
			ptr += count;
		}
		
 		if (surfaceId != 0) {
			if (currSurfaceId != surfaceId) {
	            // nouvelle surface
				currSurfaceId = surfaceId;
	 			currentSurface = new SurfaceCell;
	 	  		currentObject->surfaceCellList.push_back (currentSurface);		// add to end of vector
	 	  		
				if (currentObject->surfaceCellList.size() != surfaceId) {
	  				char errString [MAX_STRING_LENGTH];
					sprintf (errString, "Error Message: currentSurface (%d) != surfaceId (%d)", currentObject->surfaceCellList.size(), surfaceId);
					cout << errString << endl;
				}
			}
	
		  	sscanf (ptr, "%d%n", &polygonId, &count);
			ptr += count;
			
	  		PolygonCell * currentPolygon = new PolygonCell;
		  	currentSurface->polygonCellList.push_back (currentPolygon);         // add to end of vector

			if (currentSurface->polygonCellList.size() != polygonId) {
  				char errString [MAX_STRING_LENGTH];
				sprintf (errString, "Error Message: currentPolygon (%d) != polygonId (%d)", currentSurface->polygonCellList.size(), polygonId);
				cout << errString << endl;
			}
	
	        // read a polygon
			ReadAPolygon (ptr, surfaceId, currentPolygon, currentObject);
		}
		
  	} while (surfaceId != 0);
}

/**********************************************************
 * 
 * ReadAPolygon
 * 
 * parameters IN :
 *	int surfaceId
 *	PolygonCell * currentPolygon
 *	ObjectCell * currentObject
 * 
 *********************************************************/
void ObjectScene::ReadAPolygon (char * ptr, int surfaceId, PolygonCell * currentPolygon, ObjectCell * currentObject)
{
  	int noVerticesInPolygon;
  	int currentVertex, currentVertexIndex;
  	VertexList * vertexList;
  	VertexCell * tempVertex;
  	int count;

  	currentPolygon->vertexListHead = NULL;
  	
  	sscanf (ptr, "%d%n", &noVerticesInPolygon, &count);
	ptr += count;

  	for (int vertexCount = 1; vertexCount <= noVerticesInPolygon; vertexCount++) {
  	  	if (currentPolygon->vertexListHead == NULL)	{
  	  	  	vertexList = new VertexList;
  	  	  	currentPolygon->vertexListHead = vertexList;
  	  	} else {
  	  	  	vertexList->next = new VertexList;
  	  	  	vertexList = vertexList->next;
  	  	}

  	  	sscanf (ptr, "%d%n", &currentVertex, &count);
		ptr += count;

		currentVertexIndex = currentVertex -1 ;
		
  	  	if (currentObject->surfaceAt[currentVertexIndex] == 0) {
  	  		
  	  	  	currentObject->surfaceAt[currentVertexIndex] = surfaceId;
  	  	  	
		} else if (currentObject->surfaceAt[currentVertexIndex] != surfaceId) {

			currentObject->surfaceAt[currentVertexIndex] = surfaceId;

  	  	  	tempVertex = new VertexCell;
  	  	  	*tempVertex = *(currentObject->vertexAt[currentVertexIndex]);

  	  	  	tempVertex->polyListHead = NULL;

  			currentObject->vertexCellList.push_back (tempVertex);              //?? add to end of vector
  	  	}
  	  	
  	  	vertexList->vertex = currentObject->vertexAt[currentVertexIndex];
  	  	AddPolygonToPolygonList (currentPolygon, &(vertexList->vertex->polyListHead));
	}

  	vertexList->next = NULL;					// original code
}

/**********************************************************
 * 
 * AddPolygonToPolygonList
 * 
 * parameters IN :
 *	PolygonCell * currentPolygon
 *	PolygonList ** polyList
 * 
 * return value : bool
 *					true if error
 * 
 *********************************************************/
bool ObjectScene::AddPolygonToPolygonList (PolygonCell * currentPolygon, PolygonList ** polyList)
{
  	if (*polyList == NULL) {
  	  	*polyList = new PolygonList;
  	  	if (*polyList == NULL) return true;			//++ added
  	  	(*polyList)->poly = currentPolygon;
  	  	(*polyList)->next = NULL;
  	} else {
  	  	AddPolygonToPolygonList (currentPolygon, &((*polyList)->next));
	}

	return false;
}

/**********************************************************
 * 
 * ReadVertices
 * 
 * parameters IN :
 *	ObjectCell * currentObject
 * 
 *********************************************************/
void ObjectScene::ReadVertices (FILE * ptrFile, ObjectCell * currentObject) {
  	int vertexId;
  	double tempX, tempY, tempZ;

  	char mystring [MAX_STRING_LENGTH];
  	char * ptr;
  	int count;
  	
  	do {
		if (getStringNoComments(ptrFile, mystring)) {
		  	sscanf (mystring, "%d%n", &vertexId, &count);
  			ptr = mystring;
			ptr += count;
		}
		
 		if (vertexId != 0) {
		    VertexCell * currentVertex = new VertexCell;

	  	  	sscanf (ptr, "%lf %lf %lf", &tempX, &tempY, &tempZ);       // %lf pour double
	  	  	currentVertex->localPos.SetX (tempX);
	  	  	currentVertex->localPos.SetY (tempY);
	  	  	currentVertex->localPos.SetZ (tempZ);
	
	  	  	currentVertex->polyListHead = NULL;
	
			// add point to vertexCellList
			currentObject->vertexCellList.push_back (currentVertex);
			
			if (currentObject->vertexCellList.size() != vertexId) {
  				char errString [MAX_STRING_LENGTH];
				sprintf (errString, "currentVertex (%d) != vertexId (%d)", currentObject->vertexCellList.size(), vertexId);
				MessageBox (NULL, errString, "Error Message", MB_OK || MB_ICONERROR);
			}
	
	  	  	// add point to vertexAt[]
			currentObject->vertexAt.push_back (currentVertex);
	
	  	    currentObject->surfaceAt [currentObject->vertexCellList.size() - 1] = 0;
		}
		
 	} while (vertexId != 0);
}

