#include <windows.h>
#include <cstdio>
#include <cmath>

#include "ObjectScene.h"
#include "graphics3d.h"
#include "wireframe.h"
#include "Util.h"
#include "wireframe_app.h"
#include "ViewPointRec.h"

const double NSF = 30.0;			// Normal Scale Factor

const double HIGH_DOUBLE = 1.E30;

#define AXIS_RED	0
#define AXIS_GREEN	1
#define AXIS_BLUE	2

ObjectScene::ObjectScene(HWND hWnd) {
  	RECT rect;

  	GetClientRect(hWnd, &rect);
  	mapOffsets.SetXMid ((int)((rect.right - rect.left) / 2));
  	mapOffsets.SetYMid ((int)((rect.bottom - rect.top) / 2));

    viewRefPoint = ViewPointRec(1000, 5000, 45, 75);

  	viewRefPoint.viewPlaneDist = GetPrivateProfileInt("VIEWING_INFORMATION", "ViewPlaneDistance", 1000, DEFAULT_INI.c_str());
  	viewRefPoint.rho = GetPrivateProfileInt("VIEWING_INFORMATION", "Rho", 5000, DEFAULT_INI.c_str());
  	viewRefPoint.theta = GetPrivateProfileInt("VIEWING_INFORMATION", "Theta", 45, DEFAULT_INI.c_str());
  	viewRefPoint.phi = GetPrivateProfileInt("VIEWING_INFORMATION", "Phi", 75, DEFAULT_INI.c_str());

  	SetViewVariables();

  	// draw vertex normals
  	drawVertexNormals = (bool)GetPrivateProfileInt("VIEWING_INFORMATION", "DrawVertexNormals", false, DEFAULT_INI.c_str());

    sceneChanged = false;

  	noOfObjects = 0;

  	objectHead = NULL;

    objectFile = NULL;
    sceneFile = NULL;
}

void ObjectScene::SetViewVariables() {
  	viewRefPoint.SetViewVariables();
  	SetViewTransformation();
}

void ObjectScene::SetViewTransformation() {
  	viewTransformation.Identity();
  	viewRefPoint.SetViewTransformation(viewTransformation);
}

void ObjectScene::WireFrameScene(HWND hWnd) {
  	DEV_COLOR wireColor = {0, 180, 130};
  	DEV_COLOR normalColor = {200, 0, 0};
  	SurfaceCell * currentSurface;
  	PolygonCell * currentPolygon;
  	VertexList *  vertexList;
  	ScreenRec startPos;
  	Vector    tw, ts;
  	ScreenRec tPos;			// original: integer tx, ty
  	HPEN      hPen, hPenOld;	

  	HDC hDC = GetDC(hWnd);

	//?? DevClearScreen();

  	ObjectCell * currentObject = objectHead;
  	while (currentObject != NULL) {

  	  	switch (currentObject->type) {

  	  	  	case OBJECT_AXIS:
            {
				int index = 0;
  	  	  	  	VertexCell * vertex = currentObject->vertexHead;
				while (vertex != NULL) {
  	  	  	  	  	switch (index) {
  	  	  	  	  	  	case AXIS_RED:
							hPen = CreatePen(PS_SOLID, 0, RGB(255,0,0));
  	  	  	  	  	  	  	break;
  	  	  	  	  	  	case AXIS_GREEN:
							hPen = CreatePen(PS_SOLID, 0, RGB(0,255,0));
  	  	  	  	  	  	  	break;
  	  	  	  	  	  	case AXIS_BLUE:
						    hPen = CreatePen(PS_SOLID, 0, RGB(0,0,255));
  	  	  	  	  	    	break;
  	  	  	  	  	}

					hPenOld = (HPEN) SelectObject(hDC, hPen);

					MoveToEx(hDC, vertex->screenPos.GetX(), vertex->screenPos.GetY(), NULL);
					vertex = vertex->next;
					LineTo(hDC, vertex->screenPos.GetX(), vertex->screenPos.GetY());
					vertex = vertex->next;
  	  	  	  	  	index++;

					SelectObject(hDC, hPenOld);
					DeleteObject(hPen);
				}
            }
  	  	    break;

  	  	  	case OBJECT_SURFACE:
            {
  	  	  	  	currentSurface = currentObject->surfaceHead;
  	  	  	  	while (currentSurface != NULL) {
  	  	  	  	  	currentPolygon = currentSurface->polygonHead;
  	  	  	  	  	while (currentPolygon != NULL) {
  	  	  	  	  	  	if (currentPolygon->polyVisible) {
							hPen = CreatePen(PS_SOLID, 0, RGB(wireColor.r, wireColor.g, wireColor.b));
							hPenOld = (HPEN) SelectObject(hDC, hPen);

  	  	  	  	  	  	  	vertexList = currentPolygon->vertexListHead;
							startPos = vertexList->vertex->screenPos;
							MoveToEx(hDC, startPos.GetX(), startPos.GetY(), NULL);

							vertexList = vertexList->next;
							while (vertexList != NULL) {
								VertexCell * currentVertex = vertexList->vertex;
							  	LineTo(hDC, currentVertex->screenPos.GetX(), currentVertex->screenPos.GetY());
							  	vertexList = vertexList->next;
							}

							LineTo (hDC, startPos.GetX(), startPos.GetY());

							SelectObject (hDC, hPenOld);
							DeleteObject (hPen);

  	  	  	  	  	  	  	if (drawVertexNormals) {
								hPen = CreatePen(PS_SOLID, 0, RGB(normalColor.r, normalColor.g, normalColor.b));
								hPenOld = (HPEN) SelectObject(hDC, hPen);

  	  	  	  	  	  	  	  	vertexList = currentPolygon->vertexListHead;
								while (vertexList != NULL) {
  									VertexCell * currentVertex = vertexList->vertex;

									MoveToEx(hDC, currentVertex->screenPos.GetX(), currentVertex->screenPos.GetY(), NULL);

  	  	  	  	  	  	  	    	tw.SetX( currentVertex->worldPos.GetX() + (NSF * currentVertex->vertexNormal.GetX()));
  	  	  	  	  	  	  	    	tw.SetY( currentVertex->worldPos.GetY() + (NSF * currentVertex->vertexNormal.GetY()));
  	  	  	  	  	  	  	    	tw.SetZ( currentVertex->worldPos.GetZ() + (NSF * currentVertex->vertexNormal.GetZ()));

  	  	  	  	  	  	  	    	ts.VectorMatrix(tw, viewTransformation);
  	  	  	  	  	  	  	    	tPos.Perspective(ts, viewRefPoint, mapOffsets);			// tPos = (tx, ty)

								    LineTo (hDC, tPos.GetX(), tPos.GetY());

								    vertexList = vertexList->next;
  	  	  	  	  	  	  	  	}
								SelectObject (hDC, hPenOld);
								DeleteObject (hPen);
  	  	  	  	  	  	  	}
  	  	  	  	  	  	}
  	  	  	  	  	currentPolygon = currentPolygon->next;
  	  	  	  	  	}
  	  	  	  	currentSurface = currentSurface->next;
  	  	  	  	}
            }
  	  	  	break;

  	  	  	case OBJECT_PATCH:
            {
            }
  	  	  	break;
  	  	}

  	  	currentObject = currentObject->next;
  	}
}

void ObjectScene::TransformScene() {
  	zParams = ZParamRec(HIGH_DOUBLE, -HIGH_DOUBLE, 0.);

  	ObjectCell * currentObject = objectHead;
  	while (currentObject != NULL) {
  	  	currentObject->TransformToWorldCoordinates();
  	  	currentObject->CalculateNormals();
  	  	currentObject->RemoveHiddenSurfaces();
	  	currentObject->CalculateEyeCoordinates();
	  	currentObject = currentObject->next;
	}

  	zParams.zRange = zParams.zMax - zParams.zMin;
  	CalculateScreenCoordinates();     //++ mapOffsets, viewRefPoint
  	sceneChanged = false;
}

void ObjectScene::CalculateScreenCoordinates() {
  	SurfaceCell * currentSurface;
  	PolygonCell * currentPolygon;
  	VertexList *  vertexList;

  	ObjectCell * currentObject = objectHead;
  	while (currentObject != NULL) {

  	  	switch (currentObject->type) {

  	  	  	case OBJECT_AXIS:
            {
  	  	  	  	VertexCell * vertex = currentObject->vertexHead;
  	  	  	  	while (vertex != NULL) {
					vertex->screenPos.Perspective(vertex->eyePos, viewRefPoint, mapOffsets);
					vertex->screenPos.SetZ( (int)((vertex->eyePos.GetZ() - zParams.zMin) / zParams.zRange * DEV_MAX_Z_RES));
  	  	  	  	  	vertex = vertex->next;
  	  	  	  	}
            }
  	  	  	break;

  	  	  	case OBJECT_SURFACE:
            {
  	  	  	  	currentSurface = currentObject->surfaceHead;
  	  	  	  	while (currentSurface != NULL) {
  	  	  	  	  	currentPolygon = currentSurface->polygonHead;
  	  	  	  	  	while (currentPolygon != NULL) {
  	  	  	  	  	  	if (currentPolygon->polyVisible) {
  	  	  	  	  	  	  	vertexList = currentPolygon->vertexListHead;
  	  	  	  	  	  	  	while (vertexList != NULL) {
  								VertexCell * currentVertex = vertexList->vertex;

								currentVertex->screenPos.Perspective(currentVertex->eyePos, viewRefPoint, mapOffsets);
								currentVertex->screenPos.SetZ( (int)((currentVertex->eyePos.GetZ() - zParams.zMin) / zParams.zRange * DEV_MAX_Z_RES));
  	  	  	  	  	  	  	  	vertexList = vertexList->next;
  	  	  	  	  	  	  	}
  	  	  	  	  	  	}
  	  	  	  	  	  	currentPolygon = currentPolygon->next;
  	  	  	  	  	}
  	  	  	  	  	currentSurface = currentSurface->next;
  	  	  	  	}
            }
  	  	  	break;

  	  	  	case OBJECT_PATCH:
            {
            }
  	  	  	break;
  	  	}
  	  	currentObject = currentObject->next;
  	}
}

bool ObjectScene::LoadScene(HWND hWnd, char * fileName, char * sceneName) {
  	sceneFile = fopen(fileName, "r");
  	if (sceneFile == NULL) return true;

    int intValue;
    int nbObjects;
    char fileNameObject[MAX_STRING_LENGTH];
  	ObjectCell * currentObject;
  	double tempX, tempY, tempZ;
    
  	fscanf(sceneFile, "%d\n", &viewRefPoint.viewPlaneDist);
  	fscanf(sceneFile, "%d\n", &viewRefPoint.rho);
  	fscanf(sceneFile, "%d\n", &viewRefPoint.theta);
  	fscanf(sceneFile, "%d\n", &viewRefPoint.phi);

  	fscanf(sceneFile, "%d\n", &intValue);
  	drawVertexNormals = (bool)intValue;
    DrawVertexNormalsCtrl(hWnd);

  	SetViewVariables();

  	fscanf(sceneFile, "%d\n", &nbObjects);

    for (int i = 0; i < nbObjects; i++) {
  	    fscanf(sceneFile, "%s\n", &fileNameObject);

  	    fscanf(sceneFile, "%lf %lf %lf\n", &tempX, &tempY, &tempZ);
  	    Vector scale(tempX, tempY, tempZ);

  	    fscanf(sceneFile, "%lf %lf %lf\n", &tempX, &tempY, &tempZ);
  	    Vector rotate(tempX, tempY, tempZ);

  	    fscanf(sceneFile, "%lf %lf %lf\n", &tempX, &tempY, &tempZ);
  	    Vector translate(tempX, tempY, tempZ);

        LoadObject(fileNameObject, fileNameObject);
    
  	    currentObject = objectHead;
  	    if (currentObject != NULL) {
	        currentObject->GetTransformation(SCALING, scale);
	        currentObject->GetTransformation(ROTATION, rotate);
	        currentObject->GetTransformation(TRANSLATION, translate);
        }
    }
    
  	fclose(sceneFile);
  	return false;
}

bool ObjectScene::LoadObject(char * fileName, char * objectName) {
  	objectFile = fopen(fileName, "r");
  	if (objectFile == NULL) return true;

  	ObjectCell * currentObject = new ObjectCell(this);
  	currentObject->next = objectHead;
  	objectHead = currentObject;

  	currentObject->name = objectName;

  	currentObject->idNo = ++noOfObjects;

  	fscanf(objectFile, "%d\n", &(currentObject->type));		//++ added

  	fscanf(objectFile, "%d\n", &(currentObject->noVertices));
  	ReadVertices(currentObject);

  	fscanf(objectFile, "%d\n", &(currentObject->noSurfaces));
  	MakeSurfaces(currentObject);

  	currentObject->transformation.Identity();

  	fclose(objectFile);
  	return false;
}

void ObjectScene::MakeSurfaces(ObjectCell * currentObject)
{
  	int surfaceId;
  	SurfaceCell * currentSurface;

  	currentObject->surfaceHead = NULL;
  	for (int surfaceCount = 1 ; surfaceCount <= currentObject->noSurfaces; surfaceCount++) {
  	  	if (currentObject->surfaceHead == NULL) {
  	  	  	currentSurface = new SurfaceCell;
  	  	  	currentObject->surfaceHead = currentSurface;
  	  	} else {
  	  	  	currentSurface->next = new SurfaceCell;
  	  	  	currentSurface = currentSurface->next;
  	  	}
  	  	currentSurface->next = NULL;				//?? moved from end of function (from original code)

  	  	fscanf(objectFile, "%d", &surfaceId);				//++ added
  	  	fscanf(objectFile, " %d\n", &(currentSurface->noPolygons));
  	  	ReadPolygons(surfaceCount, currentSurface, currentObject);
  	}
}

void ObjectScene::ReadPolygons(int surfaceCount, SurfaceCell * currentSurface, ObjectCell * currentObject)
{
  	PolygonCell * currentPolygon;

  	currentSurface->polygonHead = NULL;
  	for (int polyCount = 1 ; polyCount <= currentSurface->noPolygons; polyCount++) {
  	  	if (currentSurface->polygonHead == NULL) {
  	  	  	currentPolygon = new PolygonCell;
  	  	  	currentSurface->polygonHead = currentPolygon;
  	  	} else {
  	  	  	currentPolygon->next = new PolygonCell;
  	  	  	currentPolygon = currentPolygon->next;
  	  	}
  	  	currentPolygon->next = NULL;				//?? moved from end of function (from original code)
		ReadAPolygon(surfaceCount, currentPolygon, currentObject);
  	}
}

void ObjectScene::ReadAPolygon(int surfaceCount, PolygonCell * currentPolygon, ObjectCell * currentObject)
{
  	int currentVertex, noVerticesInPolygon;
  	VertexList * vertexList;
  	VertexCell * tempVertex;
  	int polygonId;

  	fscanf(objectFile, "%d", &polygonId);					//++ added

  	currentPolygon->vertexListHead = NULL;
  	fscanf(objectFile, " %d", &noVerticesInPolygon);

  	for (int vertexCount = 1; vertexCount <= noVerticesInPolygon; vertexCount++) {
  	  	if (currentPolygon->vertexListHead == NULL)	{
  	  	  	vertexList = new VertexList;
  	  	  	currentPolygon->vertexListHead = vertexList;
  	  	} else {
  	  	  	vertexList->next = new VertexList;
  	  	  	vertexList = vertexList->next;
  	  	}
  	  	vertexList->next = NULL;					//?? moved from end of function (from original code)

  	  	fscanf(objectFile, " %d", &currentVertex);

  	  	if (currentObject->surfaceAt[currentVertex - 1] == 0) {
  	  	  	currentObject->surfaceAt[currentVertex - 1] = surfaceCount;
		} else {
  	  	  	if (currentObject->surfaceAt[currentVertex - 1] != surfaceCount) {
  	  	  	  	(currentObject->noVertices)++;
				currentObject->surfaceAt[currentVertex - 1] = surfaceCount;

  	  	  	  	tempVertex = new VertexCell;
  	  	  	  	*tempVertex = *currentObject->vertexAt[currentVertex - 1];
  	  	  	  	tempVertex->polyListHead = NULL;
  	  	  	  	tempVertex->next = currentObject->vertexHead;
  	  	  	  	currentObject->vertexHead = tempVertex;
  	  	  	  
				currentObject->vertexAt[currentVertex - 1] = tempVertex;
  	  	  	}
  	  	}
  	  	vertexList->vertex = currentObject->vertexAt[currentVertex - 1];
  	  	AddPolygonToPolygonList(currentPolygon, &(vertexList->vertex->polyListHead));
	}
}

int ObjectScene::AddPolygonToPolygonList(PolygonCell * currentPolygon, PolygonList ** polyList)
{
  	if (*polyList == NULL) {
  	  	*polyList = new PolygonList;
  	  	if (*polyList == NULL) return 1;			//++ added
  	  	(*polyList)->poly = currentPolygon;
  	  	(*polyList)->next = NULL;
  	} else {
  	  	AddPolygonToPolygonList(currentPolygon, &((*polyList)->next));
	}

	return 0;
}

void ObjectScene::ReadVertices(ObjectCell * currentObject) {
  	int vertexId;
  	double temp;

  	for(int vertexCount = 0; vertexCount <= currentObject->noVertices - 1; vertexCount++) {
  	  	currentObject->surfaceAt[vertexCount] = 0;

  	  	currentObject->vertexAt[vertexCount] = new VertexCell;

  	  	fscanf(objectFile, "%d", &vertexId);
  	  	fscanf(objectFile, " %lf", &temp);       // %lf pour double
  	  	currentObject->vertexAt[vertexCount]->localPos.SetX(temp);
  	  	fscanf(objectFile, " %lf", &temp);
  	  	currentObject->vertexAt[vertexCount]->localPos.SetY(temp);
  	  	fscanf(objectFile, " %lf\n", &temp);
  	  	currentObject->vertexAt[vertexCount]->localPos.SetZ(temp);

  	  	currentObject->vertexAt[vertexCount]->polyListHead = NULL;
  	}

  	for(int vertexCount = 0; vertexCount < currentObject->noVertices - 1; vertexCount++)
  	  	currentObject->vertexAt[vertexCount]->next = currentObject->vertexAt[vertexCount + 1];

  	currentObject->vertexAt[currentObject->noVertices - 1]->next = NULL;
  	currentObject->vertexHead = currentObject->vertexAt[0];
}
