#ifndef RENDERER_H
#define RENDERER_H

#include <windows.h>

#include "ObjectCell.h"
#include "Vector.h"

void RenderScene(HWND, ObjectCell *);

class EdgeBox {
public:
  	double x, z, i;
  	Vector w;
  	EdgeBox * next;
};

#endif   // RENDERER_H
