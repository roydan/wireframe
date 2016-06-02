#include <windows.h>
#include <cmath>
#include <iostream>

#include "ObjectCell.h"
#include "ObjectScene.h"
#include "graphics3d.h"
#include "Util.h"

/**********************************************************
 * ObjectCell
 * 
 * constructeur
 * 
 * objet appartient à une scène
 * 
 * parameters IN:
 * 	ObjectScene * ptrScene
 * 
 * return value : ObjectCell *
 *********************************************************/
ObjectCell::ObjectCell (ObjectScene * ptrScene) {
    this->ptrScene = ptrScene;

  	devColor = DEV_COLOR (0, 0, 0);
}

/**********************************************************
 * CalculateEyeCoordinates
 * 
 * parameters IN : none
 * 
 * return value : none
 *********************************************************/
void ObjectCell::CalculateEyeCoordinates() {
  	VertexCell * currentVertex;
	for (vector<VertexCell *>::iterator it = vertexCellList.begin(); it != vertexCellList.end(); ++it) {
		currentVertex = *it;
  	  	currentVertex->eyePos.VectorMatrix (currentVertex->worldPos, ptrScene->viewTransformation);

  	  	if (currentVertex->eyePos.GetZ() < ptrScene->zParams.GetZMin()) {
  	  	  	ptrScene->zParams.SetZMin (currentVertex->eyePos.GetZ());
		} else if (currentVertex->eyePos.GetZ() > ptrScene->zParams.GetZMax()) {
  	  	  	ptrScene->zParams.SetZMax (currentVertex->eyePos.GetZ());
		}
    }
}

/**********************************************************
 * RemoveHiddenSurfaces
 * 
 * parameters IN : none
 * 
 * return value : none
 *********************************************************/
void ObjectCell::RemoveHiddenSurfaces() {
  	SurfaceCell * currentSurface;
    for (vector<SurfaceCell *>::iterator it = surfaceCellList.begin(); it != surfaceCellList.end(); ++it) {
        currentSurface = *it;
  	  	RemovePolygonIfHidden (currentSurface);
    }
}

/**********************************************************
 * RemovePolygonIfHidden
 * 
 * private
 * 
 * parameters IN :
 *	SurfaceCell * currentSurface
 * 
 * return value : none
 *********************************************************/
void ObjectCell::RemovePolygonIfHidden (SurfaceCell * currentSurface) {
  	MyVector viewDirection;

    PolygonCell * currentPolygon;
    for (vector<PolygonCell *>::iterator it = currentSurface->polygonCellList.begin(); it != currentSurface->polygonCellList.end(); ++it) {
        currentPolygon = *it;

		VertexCell * currentVertexHead = currentPolygon->vertexListHead->vertex;
  	  	viewDirection.SetX (ptrScene->viewRefPoint.GetXView() - currentVertexHead->worldPos.GetX());
  	  	viewDirection.SetY (ptrScene->viewRefPoint.GetYView() - currentVertexHead->worldPos.GetY());
  	  	viewDirection.SetZ (ptrScene->viewRefPoint.GetZView() - currentVertexHead->worldPos.GetZ());

  	  	currentPolygon->polyVisible = (viewDirection.DotProduct (currentPolygon->polyNormal) > 0);
    }
}

/**********************************************************
 * CalculateNormals
 * 
 * parameters IN : none
 * 
 * return value : none
 *********************************************************/
void ObjectCell::CalculateNormals() {
  	CalculatePolygonNormals();
  	CalculateVertexNormals();
}

/**********************************************************
 * CalculatePolygonNormals
 * 
 * private
 * 
 * parameters IN : none
 * 
 * return value : none
 *********************************************************/
void ObjectCell::CalculatePolygonNormals() {
  	SurfaceCell * currentSurface;
    for (vector<SurfaceCell *>::iterator it_surface = surfaceCellList.begin(); it_surface != surfaceCellList.end(); ++it_surface) {
        currentSurface = *it_surface;

  		PolygonCell * currentPolygon;
        for (vector<PolygonCell *>::iterator it_polygon = currentSurface->polygonCellList.begin(); it_polygon != currentSurface->polygonCellList.end(); ++it_polygon) {
            currentPolygon = *it_polygon;
  	  	  	CalculateAPolygonNormal (currentPolygon);
  	  	}
    }
}

/**********************************************************
 * CalculateAPolygonNormal
 * 
 * private
 * 
 * parameters IN :
 *	PolygonCell * currentPolygon
 * 
 * return value : none
 *********************************************************/
void ObjectCell::CalculateAPolygonNormal (PolygonCell * currentPolygon) {
  	MyVector w1, w2, w3;

  	VertexList * vertexList = currentPolygon->vertexListHead;

  	w1 = vertexList->vertex->worldPos;
  	vertexList = vertexList->next;
  	w2 = vertexList->vertex->worldPos;
  	vertexList = vertexList->next;
  	w3 = vertexList->vertex->worldPos;

  	currentPolygon->polyNormal.SetX (((w1.GetY() - w2.GetY()) * (w2.GetZ() + w1.GetZ())) + ((w2.GetY() - w3.GetY()) * (w3.GetZ() + w2.GetZ())) + ((w3.GetY() - w1.GetY()) * (w1.GetZ() + w3.GetZ())));
  	currentPolygon->polyNormal.SetY (((w1.GetZ() - w2.GetZ()) * (w2.GetX() + w1.GetX())) + ((w2.GetZ() - w3.GetZ()) * (w3.GetX() + w2.GetX())) + ((w3.GetZ() - w1.GetZ()) * (w1.GetX() + w3.GetX())));
  	currentPolygon->polyNormal.SetZ (((w1.GetX() - w2.GetX()) * (w2.GetY() + w1.GetY())) + ((w2.GetX() - w3.GetX()) * (w3.GetY() + w2.GetY())) + ((w3.GetX() - w1.GetX()) * (w1.GetY() + w3.GetY())));

  	currentPolygon->polyNormal.Normalize();
}

/**********************************************************
 * CalculateVertexNormals
 * 
 * private
 * 
 * parameters IN : none
 * 
 * return value : none
 *********************************************************/
void ObjectCell::CalculateVertexNormals() {
  	VertexCell * currentVertex;
	for (vector<VertexCell *>::iterator it = vertexCellList.begin(); it != vertexCellList.end(); ++it) {
		currentVertex = *it;
  		CalculateAVertexNormal (currentVertex);
    }
}

/**********************************************************
 * CalculateAVertexNormal
 * 
 * private
 * 
 * parameters IN :
 *	VertexCell * currentVertex
 * 
 * return value : none
 *********************************************************/
void ObjectCell::CalculateAVertexNormal (VertexCell * currentVertex) {
  	MyVector sumVector(0, 0, 0);
  	int    polyCount = 0;

  	PolygonList * polyList = currentVertex->polyListHead;
  	while (polyList != NULL) {
  	  	polyCount++;
  	  	sumVector.SetX (sumVector.GetX() + polyList->poly->polyNormal.GetX());
  	  	sumVector.SetY (sumVector.GetY() + polyList->poly->polyNormal.GetY());
  	  	sumVector.SetZ (sumVector.GetZ() + polyList->poly->polyNormal.GetZ());
  	  	polyList = polyList->next;
  	}

  	if (polyCount > 0) {
  	  	currentVertex->vertexNormal.SetX (sumVector.GetX() / polyCount);
  	  	currentVertex->vertexNormal.SetY (sumVector.GetY() / polyCount);
  	  	currentVertex->vertexNormal.SetZ (sumVector.GetZ() / polyCount);
  	  	currentVertex->vertexNormal.Normalize();
  	} else {
  	  	currentVertex->vertexNormal = MyVector (0, 0, 0);		//?? was 1,1,1
    }
}

/**********************************************************
 * TransformToWorldCoordinates
 * 
 * parameters IN : none
 * 
 * return value : none
 *********************************************************/
void ObjectCell::TransformToWorldCoordinates() {
  	VertexCell * currentVertex;
	for (vector<VertexCell *>::iterator it = vertexCellList.begin(); it != vertexCellList.end(); ++it) {
		currentVertex = *it;
  	  	currentVertex->worldPos.VectorMatrix (currentVertex->localPos, transformation);
    }
}

/**********************************************************
 * GetTransformation
 * 
 * parameters IN :
 *	int transType
 *	MyVector transVector
 * 
 * return value : none
 *********************************************************/
void ObjectCell::GetTransformation (int transType, MyVector transVector) {
  	MyMatrix transMatrix;
  	transMatrix.Identity();

  	switch (transType) {
  	  	case ROTATION:
		{
  	    	// angles of rotation about x,y,z axes
  	    	// angle positif -> rotation anti-horaire
  	    	transMatrix = GetRotationMatrix (transVector);
  	    	transformation = transformation * transMatrix;
		}
    	break;

  	  	case SCALING:
		{
  	    	// scaling factors for x,y,z
  	    	transMatrix = GetScalingMatrix (transVector);
  	    	transformation = transformation * transMatrix;
		}
  	    break;

  	  	case TRANSLATION:
		{
  	    	// translation factors for x,y,z
  	    	transMatrix = GetTranslationMatrix (transVector);
  	    	transformation = transformation * transMatrix;
		}
  	    break;

  	  	default:
		{
  	    	transformation.Identity();
		}
	}
}

/**********************************************************
 * PRIVATE
 *********************************************************/

/**********************************************************
 * GetTranslationMatrix
 * 
 * parameters IN :
 *	MyVector tv
 * 
 * return value : MyMatrix
 *********************************************************/
MyMatrix ObjectCell::GetTranslationMatrix (MyVector tv) {
	MyMatrix tm;
  	tm.Identity();
  	tm[3][0] = tv.GetX();
  	tm[3][1] = tv.GetY();
  	tm[3][2] = tv.GetZ();
	return tm;
}

/**********************************************************
 * GetScalingMatrix
 * 
 * parameters IN :
 *	MyVector sv
 * 
 * return value : MyMatrix
 *********************************************************/
MyMatrix ObjectCell::GetScalingMatrix (MyVector sv) {
	MyMatrix sm;
  	sm.Identity();
  	sm[0][0] = sv.GetX();
  	sm[1][1] = sv.GetY();
  	sm[2][2] = sv.GetZ();
	return sm;
}

/**********************************************************
 * GetRotationMatrix
 * 
 * rotation en degre
 * angle positif -> rotation anti-horaire
 * 
 * parameters IN :
 *	MyVector rv
 * 
 * return value : MyMatrix
 *********************************************************/
MyMatrix ObjectCell::GetRotationMatrix (MyVector rv) {
	MyMatrix rm;
  	MyMatrix xRot, yRot, zRot;

	xRot.Identity();
  	yRot.Identity();
  	zRot.Identity();

  	rv.SetX (Util::InRadians (rv.GetX()));
  	rv.SetY (Util::InRadians (rv.GetY()));
  	rv.SetZ (Util::InRadians (rv.GetZ()));

  	xRot[1][1] =  cos (rv.GetX());
  	xRot[1][2] =  sin (rv.GetX());
  	xRot[2][1] = -sin (rv.GetX());
  	xRot[2][2] =  cos (rv.GetX());

  	yRot[0][0] =  cos (rv.GetY());
  	yRot[0][2] = -sin (rv.GetY());
  	yRot[2][0] =  sin (rv.GetY());
  	yRot[2][2] =  cos (rv.GetY());

  	zRot[0][0] =  cos (rv.GetZ());
  	zRot[0][1] =  sin (rv.GetZ());
  	zRot[1][0] = -sin (rv.GetZ());
  	zRot[1][1] =  cos (rv.GetZ());

  	rm = zRot * yRot;
  	rm = xRot * rm;
	return rm;
}
