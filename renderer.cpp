// Gouraud shading plus simple texture mapping

#include <windows.h>
#include <cmath>

#include "renderer.h"
#include "Vector.h"
#include "graphics3d.h"
#include "ObjectCell.h"
#include "ZParamRec.h"
#include "Util.h"

#define Ka 0.2
#define Kd 0.5
#define Kr 0.8
#define Kg 0.4
#define Kb 0.6

BOOL TextureMapping = TRUE;

HWND   ghWnd;
EdgeBox * edgeListAt[DEV_MAX_Y_RES];
int    zBufferAt[DEV_MAX_X_RES][DEV_MAX_Y_RES];
Vector lightVector;

void RenderPixel(int, int, double, Vector);
double woodGrain(Vector);
double arcTangent(double, double);
void RenderSpan(int, EdgeBox **, EdgeBox **);
void AddEdgeToList(VertexCell *, VertexCell *);
void RenderPolygon(PolygonCell *);
void RenderObject(ObjectCell *);
void initializeZBuffer();

void RenderScene(HWND hWnd, ObjectCell * currentObject)
{
  	ghWnd = hWnd;

  	initializeZBuffer();

  	Vector lightVector(2, -3, 2);

  	lightVector.Normalize();

  	while (currentObject != NULL) {
		RenderObject(currentObject);
		currentObject = currentObject->next;
  	}
}

void RenderPixel(int x, int y, double i, Vector w)
{
  	DEV_COLOR c;
  	HPEN hPen, hPenOld;
  	HDC  hDC;

  	if (i < 0) {
  	  	i = Ka;
	} else {
  	  	i = Ka + (Kd * woodGrain(w));
	}

  	if (i > 1) i = 1;

  	c.r = (int)(i * Kr * DEV_MAX_Z_RES);
  	c.g = (int)(i * Kg * DEV_MAX_Z_RES);
  	c.b = (int)(i * Kb * DEV_MAX_Z_RES);

  	hDC = GetDC(ghWnd);
  	hPen = CreatePen(PS_SOLID, 0, RGB(c.r, c.g, c.b));
  	hPenOld = (HPEN) SelectObject(hDC, hPen);

  	MoveToEx(hDC, x, y, NULL);
  	LineTo(hDC, x + 1, y + 1);

  	SelectObject(hDC, hPenOld);
  	DeleteObject(hPen);
}

double woodGrain(Vector w)
{
  	double ang;

  	double rad = sqrt(w.GetX() * w.GetX() + w.GetZ() * w.GetZ());

  	if (w.GetZ() == 0) {
  	  	ang = Util::PI() / 2.;
	} else {
  	  	ang = arcTangent(w.GetX(), w.GetZ());
	}

  	rad += 2. * sin(20. * ang + w.GetY() / 150.);

  	int grain = (int)fmod(rad, 60);		// return remainder
  	if (grain < 30) {
  	  	return 1.4;
	} else {
  	  	return 0.25;
	}
}

double arcTangent(double x, double z)
{
  	double arcTan;

  	if (z == 0)	{
  	  	if (x > 0) {
  	    	arcTan = Util::PI() / 2.;
		} else {
  	    	arcTan = 3. * Util::PI() / 2.;
		}
  	} else {
  	  	if (z < 0) {
  	  	  arcTan = atan(x / z) + Util::PI();
		} else {
			if (x < 0) {
  	  	    	arcTan = atan(x / z) + 2. * Util::PI();
			} else {
  	  	    	arcTan = atan(x / z);
			}
		}
  	}

  	return arcTan;
}

void RenderSpan(int y, EdgeBox ** edgeBox1, EdgeBox ** edgeBox2)
{
  	EdgeBox * tempEdgeBox;
  	int       x, z;
  	double    dz, di;
  	Vector    dw;

  	if ((*edgeBox1)->x > (*edgeBox2)->x) {
  	  	tempEdgeBox = *edgeBox1;
		*edgeBox1 = *edgeBox2;
  	  	*edgeBox2 = tempEdgeBox;
  	}

  	int x1 = (int)(*edgeBox1)->x;
  	int x2 = (int)(*edgeBox2)->x;
  	if (x1 != x2) {
  	  	int dx = x2 - x1;

		double currZ = (*edgeBox1)->z;
		double currI = (*edgeBox1)->i;
		Vector currW = (*edgeBox1)->w;

  	  	dz = ((*edgeBox2)->z - currZ) / dx;
  	  	di = ((*edgeBox2)->i - currI) / dx;

  	  	dw.SetX( ((*edgeBox2)->w.GetX() - currW.GetX()) / dx);
  	  	dw.SetY( ((*edgeBox2)->w.GetY() - currW.GetY()) / dx);
  	  	dw.SetZ( ((*edgeBox2)->w.GetZ() - currW.GetZ()) / dx);

  	  	for (x = x1; x <= x2 - 1; x++) {
			z = (int)currZ;
			if (z < zBufferAt[x][y]) {
			    zBufferAt[x][y] = z;
  	  	    	RenderPixel(x, y, currI, currW);
  	  	  	}

  	  	  	currZ += dz;
			currI += di;

  	  	  	currW.SetX( currW.GetX() + dw.GetX());
  	  	  	currW.SetY( currW.GetY() + dw.GetY());
  	  	  	currW.SetZ( currW.GetZ() + dw.GetZ());
		}
  	}
}

void AddEdgeToList(VertexCell * vertex1, VertexCell * vertex2)
{
  	VertexCell * tempVertex;
  	Vector    dw;
  	EdgeBox * edgeBox;

  	if (vertex1->screenPos.GetY() > vertex2->screenPos.GetY()) {
		tempVertex = vertex1;
		vertex1 = vertex2;
		vertex2 = tempVertex;
  	}

  	int y1 = vertex1->screenPos.GetY();
  	int y2 = vertex2->screenPos.GetY();

  	if (y1 != y2) {
		int dy = y2 - y1;

		double currX = vertex1->screenPos.GetX();
		double currZ = vertex1->screenPos.GetZ();
		double currI = lightVector.DotProduct(vertex1->vertexNormal);
		Vector currW = vertex1->worldPos;

		double dx = (vertex2->screenPos.GetX() - currX) / dy;
		double dz = (vertex2->screenPos.GetZ() - currZ) / dy;
		double di = (lightVector.DotProduct(vertex2->vertexNormal) - currI) / dy;

		dw.SetX( (vertex2->worldPos.GetX() - currW.GetX()) / dy);
		dw.SetY( (vertex2->worldPos.GetY() - currW.GetY()) / dy);
		dw.SetZ( (vertex2->worldPos.GetZ() - currW.GetZ()) / dy);

		for (int y = y1; y <= y2 - 1; y++) {
  	    	edgeBox = new EdgeBox;
  	    
		  	edgeBox->x = currX;
  	    	edgeBox->z = (int)currZ;
  	    	edgeBox->i = currI;
  	    	edgeBox->w = currW;
  	    	edgeBox->next = edgeListAt[y];
 
  	    	edgeListAt[y] = edgeBox;
  	    
		  	currX += dx;
  	    	currZ += dz;
  	    	currI += di;
		  
		  	currW.SetX( currW.GetX() + dw.GetX());
		  	currW.SetY( currW.GetY() + dw.GetY());
		  	currW.SetZ( currW.GetZ() + dw.GetZ());
		}
  	}
}

void RenderPolygon(PolygonCell * currentPolygon)
{
  	int y;

  	for (y = 0; y <= DEV_MAX_Y_RES; y++) {
		edgeListAt[y] = NULL;
	}

  	VertexList * vertex1 = currentPolygon->vertexListHead;
  	VertexList * vertex0 = vertex1;
  	VertexList * vertex2 = vertex1->next;

  	do {
		AddEdgeToList(vertex1->vertex, vertex2->vertex);
		vertex1 = vertex2;
  	  	vertex2 = vertex2->next;
  	} while (vertex2 != NULL);

  	AddEdgeToList(vertex1->vertex, vertex0->vertex);

  	for (y = 0; y <= DEV_MAX_Y_RES; y++) {
		if (edgeListAt[y] != NULL) {
		  	RenderSpan(y, &edgeListAt[y], &edgeListAt[y]->next);
		}
	}
}

void RenderObject(ObjectCell * currentObject)
{
  	PolygonCell * currentPolygon;

  	SurfaceCell * currentSurface = currentObject->surfaceHead;
  	while (currentSurface != NULL) {
  	  	currentPolygon = currentSurface->polygonHead;
  	  	while (currentPolygon != NULL) {
  	  	  	if (currentPolygon->polyVisible) {
  	  	    	RenderPolygon(currentPolygon);
			}
			currentPolygon = currentPolygon->next;
		}
  	  	currentSurface = currentSurface->next;
  	}
}

void initializeZBuffer()
{
  	int x, y;

  	for (x = 0; x <= DEV_MAX_X_RES; x++) {
  	  	for (y = 0; y <= DEV_MAX_Y_RES; y++) {
		  	zBufferAt[x][y] = DEV_MAX_Z_RES;
		}
	}
}

