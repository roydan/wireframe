#ifndef OBJECT_CELL_H
#define OBJECT_CELL_H

#include <string>
#include <vector>

#include "MyVector.h"
#include "ScreenRec.h"
#include "MyMatrix.h"
#include "graphics3d.h"

using namespace std;

const int MAX_NO_OF_VERTICES = 500;

class PolygonList;			// declaration

/**********************************************************
 *
 * vertex
 *
 *********************************************************/
class VertexCell {
public:
  	MyVector localPos;		// coordonnees des points
  	MyVector worldPos;
  	MyVector eyePos;
  	MyVector vertexNormal;
  	ScreenRec screenPos;
  	PolygonList * polyListHead;
};

/**********************************************************
 *
 * liste de pointeurs de vertex
 * 
 *********************************************************/
class VertexList {
public:
  	VertexCell * vertex;
  	VertexList * next;
};

/**********************************************************
 *
 * polygone
 *
 *********************************************************/
class PolygonCell {
public:
  	VertexList * vertexListHead;
  	MyVector	polyNormal;
  	bool		polyVisible;		// Culled
};

/**********************************************************
 *
 * liste de pointeurs de polygone
 *
 *********************************************************/
class PolygonList {
public:
  	PolygonCell * poly;
  	PolygonList * next;
};

/**********************************************************
 *
 * surface
 *
 *********************************************************/
class SurfaceCell {
public:
  	vector<PolygonCell *> polygonCellList;
};

/**********************************************************
 *
 * objet graphique
 *
 *********************************************************/
class ObjectCell {
private:
    class ObjectScene * ptrScene;

public:
  	int    		type;		//++ added
  	string 		name;
  	vector<SurfaceCell *> surfaceCellList;
  	vector<VertexCell *>  vertexCellList;
  	MyMatrix      		  transformation;

  	DEV_COLOR 	devColor;
  	
    vector<VertexCell *>  vertexAt;
    int surfaceAt[MAX_NO_OF_VERTICES];
    
  	ObjectCell (ObjectScene * ptrScene);

	void CalculateEyeCoordinates();
	void RemoveHiddenSurfaces();
	void CalculateNormals();
	void TransformToWorldCoordinates();
	void GetTransformation (int, MyVector);

private:
	void RemovePolygonIfHidden (SurfaceCell *);
	void CalculatePolygonNormals();
	void CalculateAPolygonNormal (PolygonCell *);
	void CalculateVertexNormals();
	void CalculateAVertexNormal (VertexCell *);

	MyMatrix GetTranslationMatrix (MyVector);
	MyMatrix GetScalingMatrix (MyVector);
	MyMatrix GetRotationMatrix (MyVector);
};

#endif   // OBJECT_CELL_H

