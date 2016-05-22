#ifndef OBJECT_CELL_H
#define OBJECT_CELL_H

#include <string>
#include <vector>

#include "Vector.h"
#include "ScreenRec.h"
#include "Matrix.h"
#include "graphics3d.h"

using namespace std;

const int MAX_NO_OF_VERTICES = 500;

class PolygonList;			// declaration

/**********************************************************
 *
 * classe VertexCell
 *
 *********************************************************/

// vertex
class VertexCell {
public:
  	Vector localPos;		// coordonnees des points
  	Vector worldPos;
  	Vector eyePos;
  	Vector vertexNormal;
  	ScreenRec screenPos;
  	PolygonList * polyListHead;
  	VertexCell * next;		// pointeur sur le point suivant
};

/**********************************************************
 *
 * classe VertexList
 *
 *********************************************************/

// liste de pointeurs de vertex
class VertexList {
public:
  	VertexCell * vertex;
  	VertexList * next;
};

/**********************************************************
 *
 * classe PolygonCell
 *
 *********************************************************/

// polygone
class PolygonCell {
public:
  	VertexList * vertexListHead;
  	Vector polyNormal;
  	bool   polyVisible;
  	PolygonCell * next;
};

/**********************************************************
 *
 * classe PolygonList
 *
 *********************************************************/

// liste de pointeurs de polygone
class PolygonList {
public:
  	PolygonCell * poly;
  	PolygonList * next;
};

/**********************************************************
 *
 * classe SurfaceCell
 *
 *********************************************************/

// surface
class SurfaceCell {
public:
  	int noPolygons;
  	vector<PolygonCell *> polygonHead;
};

/**********************************************************
 *
 * classe ObjectCell
 *
 *********************************************************/

// objet graphique
class ObjectCell {
private:
    class ObjectScene * ptrScene;

public:
  	int    		type;		//++ added
  	string 		name;
  	int    		idNo;
  	int    		noVertices;
  	int    		noSurfaces;
  	vector<SurfaceCell *> surfaceHead;
  	vector<VertexCell *>  vertexHead;
  	Matrix      		  transformation;
    vector<VertexCell *>  vertexAt;
    int         surfaceAt[MAX_NO_OF_VERTICES];

  	DEV_COLOR devColor;
  	
  	ObjectCell (ObjectScene * ptrScene);

	void CalculateEyeCoordinates();
	void RemoveHiddenSurfaces();
	void CalculateNormals();
	void TransformToWorldCoordinates();
	void GetTransformation (int, Vector);

private:
	void RemovePolygonIfHidden (SurfaceCell *);
	void CalculatePolygonNormals();
	void CalculateAPolygonNormal (PolygonCell *);
	void CalculateVertexNormals();
	void CalculateAVertexNormal (VertexCell *);

	Matrix GetTranslationMatrix (Vector);
	Matrix GetScalingMatrix (Vector);
	Matrix GetRotationMatrix (Vector);
};

#endif   // OBJECT_CELL_H
