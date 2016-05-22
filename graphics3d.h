#ifndef GRAPHICS3D_H
#define GRAPHICS3D_H

using namespace std;

#define OBJECT_AXIS    	0
#define OBJECT_SURFACE 	1
#define OBJECT_PATCH 	2     // bicubic Bezier patches

// linear transformation type
#define ROTATION    	1
#define SCALING     	2
#define TRANSLATION 	3

struct DEV_COLOR {
  	int r, g, b;		// 0..255
};

#endif   // GRAPHICS3D_H


