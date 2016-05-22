#ifndef OBJECT_CELL_H
#define OBJECT_CELL_H

#include <string>

#include "Vector.h"
#include "ScreenRec.h"
#include "Matrix.h"

using namespace std;

const int MAX_NO_OF_VERTICES = 500;

class PolygonList;			// declaration

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

// liste de pointeurs de vertex
class VertexList {
public:
  	VertexCell * vertex;
  	VertexList * next;
};

// polygone
class PolygonCell {
public:
  	VertexList * vertexListHead;
  	Vector polyNormal;
  	bool   polyVisible;
  	PolygonCell * next;
};

// liste de pointeurs de polygone
class PolygonList {
public:
  	PolygonCell * poly;
  	PolygonList * next;
};

// surface
class SurfaceCell {
public:
  	int noPolygons;
  	PolygonCell * polygonHead;
  	SurfaceCell * next;
};

// objet graphique
class ObjectCell {
private:
    class ObjectScene * pObjectScene;

	void RemovePolygonIfHidden(PolygonCell *);
	void CalculatePolygonNormals();
	void CalculateAPolygonNormal(PolygonCell *);
	void CalculateVertexNormals();
	void CalculateAVertexNormal(VertexCell *);
	Matrix GetTranslationMatrix(Vector);
	Matrix GetScalingMatrix(Vector);
	Matrix GetRotationMatrix(Vector);

public:
  	int    type;		//++ added
  	string name;
  	int    idNo;
  	int    noVertices;
  	int    noSurfaces;
  	SurfaceCell * surfaceHead;
  	VertexCell *  vertexHead;
  	Matrix        transformation;
  	ObjectCell *  next;
    VertexCell *  vertexAt[MAX_NO_OF_VERTICES];
    int           surfaceAt[MAX_NO_OF_VERTICES];
  	
    ObjectCell();
  	ObjectCell(ObjectScene * pObjectScene);
	void CalculateEyeCoordinates();
	void RemoveHiddenSurfaces();
	void CalculateNormals();
	void TransformToWorldCoordinates();
	void GetTransformation(int, Vector);
};

#endif   // OBJECT_CELL_H
