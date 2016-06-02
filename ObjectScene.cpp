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
// #include "ViewPointRec.h"

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
 *********************************************************/
ObjectScene::ObjectScene (HWND hWnd) {
  	RECT rect;
  	GetClientRect (hWnd, &rect);
	int rl = (int)((rect.right - rect.left) / 2);
	int bt = (int)((rect.bottom - rect.top) / 2);
	mapOffsets = MapRec (rl, bt);

  	int viewPlaneDist = GetPrivateProfileInt ("VIEWING_INFORMATION", "ViewPlaneDistance", 1000, DEFAULT_INI.c_str());
  	int rho = GetPrivateProfileInt ("VIEWING_INFORMATION", "Rho", 5000, DEFAULT_INI.c_str());
  	int theta = GetPrivateProfileInt ("VIEWING_INFORMATION", "Theta", 45, DEFAULT_INI.c_str());
  	int phi = GetPrivateProfileInt ("VIEWING_INFORMATION", "Phi", 75, DEFAULT_INI.c_str());
    viewRefPoint = ViewPointRec (viewPlaneDist, rho, theta, phi);

  	SetViewVariables();

  	// draw vertex normals
  	drawVertexNormals = (bool)GetPrivateProfileInt ("VIEWING_INFORMATION", "DrawVertexNormals", false, DEFAULT_INI.c_str());

    sceneChanged = false;

  	objectCellList.clear();

    objectFile = NULL;
    sceneFile = NULL;
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

	//?? DevClearScreen();

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

  	ObjectCell * currentObject;
	for (vector<ObjectCell *>::iterator it = objectCellList.begin(); it != objectCellList.end(); ++it) {
		currentObject = *it;
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
 * scene file
 * 
 **********************************************************
 * 
 * 						viewing information
 *  1						  1 = perspective , 0 = parallel
 * -7000					  view distance			ViewPointRec.viewPlaneDist
 *  10000					  perspective distance
 * -0.7	-0.4	-0.3	  view plane normal		ViewPointRec.rho .theta .phi
 *  0.0	0.0		1.0		  view up vector
 *  0						  draw vertex normals	bool drawVertexNormals
 *  4						no. of objects
 *  1						object id
 *  cube.obj				object data filename
 * 						object transformations	MyMatrix transformation
 *  0.4 0.4 0.4		  	  scale vector
 *  0.0 0.0 0.0		  	  rotate vector
 *  0.5 1.7 -0.6		  	  translate vector
 * 
 **********************************************************
 * 
 *  view distance
 *  rho
 *  theta
 *  phi
 *  draw vertex normals
 *  no. of objects
 *  object data filename
 *  object transformations / scale / rotate / translate
 * 
 **********************************************************
 * 
 * LoadScene
 * 
 * parameters IN :
 *	HWND hWnd
 *	char * fileName
 *	char * sceneName
 * 
 * return value : bool
 * 
 *********************************************************/
bool ObjectScene::LoadScene (HWND hWnd, char * fileName, char * sceneName) {
  	sceneFile = fopen (fileName, "r");
  	if (sceneFile == NULL) return true;

    int intValue;
    int nbObjects;
    int objectId;
    char fileNameObject[MAX_STRING_LENGTH];
  	double tempX, tempY, tempZ;
    
  	fscanf (sceneFile, "%d\n", &intValue);
  	viewRefPoint.SetViewPlaneDist (intValue);
  	fscanf (sceneFile, "%d\n", &intValue);
  	viewRefPoint.SetRho (intValue);
  	fscanf (sceneFile, "%d\n", &intValue);
  	viewRefPoint.SetTheta (intValue);
  	fscanf (sceneFile, "%d\n", &intValue);
  	viewRefPoint.SetPhi (intValue);

  	fscanf (sceneFile, "%d\n", &intValue);
  	drawVertexNormals = (bool)intValue;
    DrawVertexNormalsCtrl (hWnd);

  	SetViewVariables();

    int i = 0;

    fscanf (sceneFile, "%d\n", &objectId);
    cout << "objectId= " << objectId << endl;
		
  	while (objectId != 0) {
  	    fscanf (sceneFile, "%s\n", &fileNameObject);

  	    fscanf (sceneFile, "%lf %lf %lf\n", &tempX, &tempY, &tempZ);
  	    MyVector scale (tempX, tempY, tempZ);

  	    fscanf (sceneFile, "%lf %lf %lf\n", &tempX, &tempY, &tempZ);
  	    MyVector rotate (tempX, tempY, tempZ);

  	    fscanf (sceneFile, "%lf %lf %lf\n", &tempX, &tempY, &tempZ);
  	    MyVector translate (tempX, tempY, tempZ);

  	    DEV_COLOR aColor_scn = DEV_COLOR();
  	    fscanf (sceneFile, "%d\n", &(aColor_scn.r));
  	    fscanf (sceneFile, "%d\n", &(aColor_scn.g));
  	    fscanf (sceneFile, "%d\n", &(aColor_scn.b));
  	    //?? cout << "aColor_scn.r,g,b = " << aColor_scn.r << " , " << aColor_scn.g << " , " << aColor_scn.b << endl;

        if (strstr(fileNameObject, ".obj") != NULL) {
            // load an object
  	  	  	if (LoadObjectExt (fileNameObject, fileNameObject) == false) {

  		       ObjectCell * currentObject = objectCellList [i];

               currentObject->GetTransformation (SCALING, scale);
               currentObject->GetTransformation (ROTATION, rotate);
               currentObject->GetTransformation (TRANSLATION, translate);

              //?? cout << "currentObject->devColor.r,g,b (scn) = " << currentObject->devColor.r << " , " << currentObject->devColor.g << " , " << currentObject->devColor.b << endl;
               if (!((aColor_scn.r == 0) && (aColor_scn.g == 0) && (aColor_scn.b == 0))) {
  	              currentObject->devColor = aColor_scn;
  	              cout << "currentObject->devColor (scn)= " << currentObject->devColor.r << " , " << currentObject->devColor.g << " , " << currentObject->devColor.b << endl;
               }

  	  	    	sceneChanged = true;
			}
        }

        i++;
        
        fscanf (sceneFile, "%d\n", &objectId);
        cout << "objectId= " << objectId << endl;
    }
    
  	fclose (sceneFile);
  	return false;
}


/**********************************************************
 * 
 * object file
 * 
 **********************************************************
 * 
 * type
 * no_vertices
 * ReadVertices()
 * 		vertex.id vertex.x vertex.y vertex.z
 * no_surfaces
 * MakeSurfaces()
 * 		surface.id no_polygons
 * 		polygon.id no_vertices_in_polygon vertex.id
 * 
 **********************************************************
 * 
 * LoadObjectExt
 * 
 * parameters IN :
 *	char * fileName
 *	char * objectName
 * 
 * return value : bool
 * 
 *********************************************************/
bool ObjectScene::LoadObjectExt (char * fileName, char * objectName) {
  	objectFile = fopen (fileName, "r");
  	if (objectFile == NULL) return true;

  	ObjectCell * currentObject = new ObjectCell (this);

  	currentObject->idNo = objectCellList.size() + 1;
  	cout << "currentObject->idNo= " << currentObject->idNo << endl;

  	currentObject->name = objectName;
  	cout << "currentObject->name= " << currentObject->name << endl;

   // read currentObject type
  	fscanf (objectFile, "%d\n", &(currentObject->type));

  	ReadVerticesExt (currentObject);

  	MakeSurfacesExt (currentObject);

   // read color
  	DEV_COLOR aColor = DEV_COLOR (0, 180, 130);
  	fscanf (objectFile, "%d\n", &(aColor.r));
  	fscanf (objectFile, "%d\n", &(aColor.g));
  	fscanf (objectFile, "%d\n", &(aColor.b));
  	currentObject->devColor = aColor;
  	cout << "currentObject->devColor (obj)= " << currentObject->devColor.r << endl;
  	cout << "currentObject->devColor (obj)= " << currentObject->devColor.g << endl;
  	cout << "currentObject->devColor (obj)= " << currentObject->devColor.b << endl;

	// default transformation
  	currentObject->transformation.Identity();

  	objectCellList.push_back (currentObject);              // add to end of vector

  	fclose (objectFile);
  	return false;
}

/**********************************************************
 * 
 * MakeSurfacesExt
 * 
 * parameters IN :
 *	ObjectCell * currentObject
 * 
 *********************************************************/
void ObjectScene::MakeSurfacesExt (ObjectCell * currentObject)
{
    SurfaceCell * currentSurface;
  	int surfaceId, currId = 0;
	
  	fscanf (objectFile, "%d", &surfaceId);
    //?? cout << "MakeSurfacesExt: surfaceId= " << surfaceId << endl;
		
  	while (surfaceId != 0) {
		if (currId != surfaceId) {
            // nouvelle surface
			currId = surfaceId;
 			currentSurface = new SurfaceCell;
 	  		currentObject->surfaceCellList.push_back (currentSurface);              // add to end of vector
		}

  		PolygonCell * currentPolygon = new PolygonCell;
	  	currentSurface->polygonCellList.push_back (currentPolygon);              // add to end of vector

        // read a polygon
		ReadAPolygonExt (surfaceId, currentPolygon, currentObject);

 	    fscanf (objectFile, "%d", &surfaceId);
        //?? cout << "MakeSurfacesExt: surfaceId= " << surfaceId << endl;
  	}
}

/**********************************************************
 * 
 * ReadAPolygonExt
 * 
 * parameters IN :
 *	int surfaceId
 *	PolygonCell * currentPolygon
 *	ObjectCell * currentObject
 * 
 *********************************************************/
void ObjectScene::ReadAPolygonExt (int surfaceId, PolygonCell * currentPolygon, ObjectCell * currentObject)
{
  	int currentVertex, noVerticesInPolygon;
  	VertexList * vertexList;
  	VertexCell * tempVertex;
  	int polygonId;

  	fscanf (objectFile, "%d", &polygonId);					//++ added

  	currentPolygon->vertexListHead = NULL;
  	
  	fscanf (objectFile, " %d", &noVerticesInPolygon);

  	for (int vertexCount = 1; vertexCount <= noVerticesInPolygon; vertexCount++) {
  	  	if (currentPolygon->vertexListHead == NULL)	{
  	  	  	vertexList = new VertexList;
  	  	  	currentPolygon->vertexListHead = vertexList;
  	  	} else {
  	  	  	vertexList->next = new VertexList;
  	  	  	vertexList = vertexList->next;
  	  	}

  	  	fscanf (objectFile, " %d", &currentVertex);

  	  	if (currentObject->surfaceAt[currentVertex - 1] == 0) {
  	  	  	currentObject->surfaceAt[currentVertex - 1] = surfaceId;
		} else {
  	  	  	if (currentObject->surfaceAt[currentVertex - 1] != surfaceId) {

				currentObject->surfaceAt[currentVertex - 1] = surfaceId;

  	  	  	  	tempVertex = new VertexCell;
				//?? cout << "tempVertex= " << tempVertex->localPos.GetX() << " " << tempVertex->localPos.GetY() << " " << tempVertex->localPos.GetZ() << endl;
  	  	  	  	*tempVertex = *(currentObject->vertexAt[currentVertex - 1]);
				//?? cout << "tempVertex= " << tempVertex->localPos.GetX() << " " << tempVertex->localPos.GetY() << " " << tempVertex->localPos.GetZ() << endl;

  	  	  	  	tempVertex->polyListHead = NULL;

  	  			currentObject->vertexCellList.push_back (tempVertex);              // add to end of vector
                //?? cout << "ReadAPolygon: add to vertexCellList -> vertexAt[ (currentVertex= " << currentVertex << ") - 1 ]" << endl;
  	  	  	}
  	  	}
  	  	vertexList->vertex = currentObject->vertexAt[currentVertex - 1];
  	  	AddPolygonToPolygonListExt (currentPolygon, &(vertexList->vertex->polyListHead));
	}

  	vertexList->next = NULL;					// original code
}

/**********************************************************
 * 
 * AddPolygonToPolygonListExt
 * 
 * parameters IN :
 *	PolygonCell * currentPolygon
 *	PolygonList ** polyList
 * 
 * return value : int
 * 
 *********************************************************/
int ObjectScene::AddPolygonToPolygonListExt (PolygonCell * currentPolygon, PolygonList ** polyList)
{
  	if (*polyList == NULL) {
  	  	*polyList = new PolygonList;
  	  	if (*polyList == NULL) return 1;			//++ added
  	  	(*polyList)->poly = currentPolygon;
  	  	(*polyList)->next = NULL;
  	} else {
  	  	AddPolygonToPolygonListExt (currentPolygon, &((*polyList)->next));
	}

	return 0;
}

/**********************************************************
 * 
 * ReadVerticesExt
 * 
 * parameters IN :
 *	ObjectCell * currentObject
 * 
 *********************************************************/
void ObjectScene::ReadVerticesExt (ObjectCell * currentObject) {
  	int vertexId;
  	double temp;

  	fscanf (objectFile, "%d", &vertexId);
    //?? cout << "ReadVerticesExt: vertexId= " << vertexId << endl;
		
  	while (vertexId != 0) {
	      VertexCell * currentVertex = new VertexCell;
	      
  	  	fscanf (objectFile, " %lf", &temp);       // %lf pour double
  	  	currentVertex->localPos.SetX (temp);
  	  	fscanf (objectFile, " %lf", &temp);
  	  	currentVertex->localPos.SetY (temp);
  	  	fscanf (objectFile, " %lf\n", &temp);
  	  	currentVertex->localPos.SetZ (temp);
		//?? cout << "currentVertex->localPos= " << currentVertex->localPos.GetX() << " , " << currentVertex->localPos.GetY() << " , " << currentVertex->localPos.GetZ() << endl;

  	  	currentVertex->polyListHead = NULL;

  	  	// add point to vertexAt[]
		currentObject->vertexAt.push_back (currentVertex);
        //?? cout << "ReadVerticesExt: add to vertexAt[ " << currentObject->vertexAt.size() - 1 << " ]" << endl;

		// add point to vertexCellList
		currentObject->vertexCellList.push_back (currentVertex);
    	//?? cout << "ReadVerticesExt: add to vertexCellList -> vertexAt[] = " << currentVertex << endl;

  	      currentObject->surfaceAt [currentObject->vertexCellList.size() - 1] = 0;
  	      //?? currentObject->surfaceAt [currentObject->vertexCellList.size() - 1] = 0;
		
  	    fscanf (objectFile, "%d", &vertexId);
        //?? cout << "ReadVerticesExt: vertexId= " << vertexId << endl;
 	}
}

