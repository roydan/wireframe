#ifndef RENDERER_H
#define RENDERER_H

#include <windows.h>
#include <vector>

#include "ObjectCell.h"
#include "MyVector.h"
#include "graphics3d.h"

/**********************************************************
 *
 * classe EdgeBox
 *
 *********************************************************/

class EdgeBox {
public:
  	double x, z, i;
  	MyVector w;
  	EdgeBox * next;
};

void RenderScene (HWND, ObjectScene *);
void initializeZBuffer();
void RenderObject (ObjectCell *);
void RenderPolygon (PolygonCell *);
void AddEdgeToList (VertexCell *, VertexCell *);
void RenderSpan (int, EdgeBox **, EdgeBox **);
void RenderPixel (int, int, double, MyVector);
double woodGrain (MyVector);
double arcTangent (double, double);

#endif   // RENDERER_H

