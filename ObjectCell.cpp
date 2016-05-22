#include <windows.h>
#include <cmath>

#include "ObjectCell.h"
#include "ObjectScene.h"
#include "graphics3d.h"
#include "Util.h"

ObjectCell::ObjectCell() {
    this->pObjectScene = NULL;
}

// objet appartient à une scène
ObjectCell::ObjectCell(ObjectScene * pObjectScene) {
    this->pObjectScene = pObjectScene;
}

void ObjectCell::CalculateEyeCoordinates() {
  	VertexCell * currentVertex = vertexHead;
  	while (currentVertex != NULL) {
  	  	currentVertex->eyePos.VectorMatrix(currentVertex->worldPos, pObjectScene->viewTransformation);

  	  	if (currentVertex->eyePos.GetZ() < pObjectScene->zParams.zMin) {
  	  	  	pObjectScene->zParams.zMin = currentVertex->eyePos.GetZ();
		} else if (currentVertex->eyePos.GetZ() > pObjectScene->zParams.zMax) {
  	  	  	pObjectScene->zParams.zMax = currentVertex->eyePos.GetZ();
		}

  	  	currentVertex = currentVertex->next;
    }
}

void ObjectCell::RemoveHiddenSurfaces() {
  	SurfaceCell * currentSurface = surfaceHead;
  	while (currentSurface != NULL) {
  	  	RemovePolygonIfHidden(currentSurface->polygonHead);
  	  	currentSurface = currentSurface->next;
    }
}

void ObjectCell::RemovePolygonIfHidden(PolygonCell * currentPolygon) {
  	Vector viewDirection;

  	while (currentPolygon != NULL) {
		VertexCell * currentVertexHead = currentPolygon->vertexListHead->vertex;
  	  	viewDirection.SetX( pObjectScene->viewRefPoint.GetXView() - currentVertexHead->worldPos.GetX());
  	  	viewDirection.SetY( pObjectScene->viewRefPoint.GetYView() - currentVertexHead->worldPos.GetY());
  	  	viewDirection.SetZ( pObjectScene->viewRefPoint.GetZView() - currentVertexHead->worldPos.GetZ());

  	  	currentPolygon->polyVisible = (viewDirection.DotProduct(currentPolygon->polyNormal) > 0);
  	  	currentPolygon = currentPolygon->next;
    }
}

void ObjectCell::CalculateNormals() {
  	CalculatePolygonNormals();
  	CalculateVertexNormals();
}

void ObjectCell::CalculatePolygonNormals() {
  	PolygonCell * currentPolygon;

  	SurfaceCell * currentSurface = surfaceHead;
  	while (currentSurface != NULL) {
  	  	currentPolygon = currentSurface->polygonHead;
  	  	while (currentPolygon != NULL) {
  	  	  	CalculateAPolygonNormal(currentPolygon);
  	  	  	currentPolygon = currentPolygon->next;
  	  	}
  	  	currentSurface = currentSurface->next;
    }
}

void ObjectCell::CalculateAPolygonNormal(PolygonCell * currentPolygon) {
  	VertexList * vertexList;
  	Vector w1, w2, w3;

  	vertexList = currentPolygon->vertexListHead;

  	w1 = vertexList->vertex->worldPos;
  	vertexList = vertexList->next;
  	w2 = vertexList->vertex->worldPos;
  	vertexList = vertexList->next;
  	w3 = vertexList->vertex->worldPos;

  	currentPolygon->polyNormal.SetX( ((w1.GetY() - w2.GetY()) * (w2.GetZ() + w1.GetZ())) + ((w2.GetY() - w3.GetY()) * (w3.GetZ() + w2.GetZ())) + ((w3.GetY() - w1.GetY()) * (w1.GetZ() + w3.GetZ())));
  	currentPolygon->polyNormal.SetY( ((w1.GetZ() - w2.GetZ()) * (w2.GetX() + w1.GetX())) + ((w2.GetZ() - w3.GetZ()) * (w3.GetX() + w2.GetX())) + ((w3.GetZ() - w1.GetZ()) * (w1.GetX() + w3.GetX())));
  	currentPolygon->polyNormal.SetZ( ((w1.GetX() - w2.GetX()) * (w2.GetY() + w1.GetY())) + ((w2.GetX() - w3.GetX()) * (w3.GetY() + w2.GetY())) + ((w3.GetX() - w1.GetX()) * (w1.GetY() + w3.GetY())));

  	currentPolygon->polyNormal.Normalize();
}

void ObjectCell::CalculateVertexNormals()	{
  	VertexCell * currentVertex = vertexHead;
  	while (currentVertex != NULL) {
  	  CalculateAVertexNormal(currentVertex);
  	  currentVertex = currentVertex->next;
    }
}

void ObjectCell::CalculateAVertexNormal(VertexCell * currentVertex) {
  	PolygonList * polyList;
  	Vector sumVector(0, 0, 0);
  	int    polyCount;

  	polyCount = 0;
  	polyList = currentVertex->polyListHead;
  	while (polyList != NULL) {
  	  	polyCount++;
  	  	sumVector.SetX( sumVector.GetX() + polyList->poly->polyNormal.GetX());
  	  	sumVector.SetY( sumVector.GetY() + polyList->poly->polyNormal.GetY());
  	  	sumVector.SetZ( sumVector.GetZ() + polyList->poly->polyNormal.GetZ());
  	  	polyList = polyList->next;
  	}

  	if (polyCount > 0) {
  	  	currentVertex->vertexNormal.SetX( sumVector.GetX() / polyCount);
  	  	currentVertex->vertexNormal.SetY( sumVector.GetY() / polyCount);
  	  	currentVertex->vertexNormal.SetZ( sumVector.GetZ() / polyCount);
  	  	currentVertex->vertexNormal.Normalize();
  	} else {
  	  	currentVertex->vertexNormal.SetX(1);
  	  	currentVertex->vertexNormal.SetY(1);
  	  	currentVertex->vertexNormal.SetZ(1);
    }
}

void ObjectCell::TransformToWorldCoordinates() {
  	VertexCell * currentVertex = vertexHead;
  	while (currentVertex != NULL) {
  	  	currentVertex->worldPos.VectorMatrix(currentVertex->localPos, transformation);
  	  	currentVertex = currentVertex->next;
    }
}

void ObjectCell::GetTransformation(int transType, Vector transVector) {
  	Matrix transMatrix;
  	transMatrix.Identity();

  	switch (transType) {
  	  	case ROTATION:
  	    	// angles of rotation about x,y,z axes
  	    	// angle positif -> rotation anti-horaire
  	    	transMatrix = GetRotationMatrix(transVector);
  	    	transformation = transformation * transMatrix;
  	    	break;

  	  	case SCALING:
  	    	// scaling factors for x,y,z
  	    	transMatrix = GetScalingMatrix(transVector);
  	    	transformation = transformation * transMatrix;
  	    	break;

  	  	case TRANSLATION:
  	    	// translation factors for x,y,z
  	    	transMatrix = GetTranslationMatrix(transVector);
  	    	transformation = transformation * transMatrix;
  	    	break;

  	  	default:
  	    	transformation.Identity();
	}
}

Matrix ObjectCell::GetTranslationMatrix(Vector tv) {
	Matrix tm;
  	tm.Identity();
  	tm.SetValue(3, 0, tv.GetX());
  	tm.SetValue(3, 1, tv.GetY());
  	tm.SetValue(3, 2, tv.GetZ());
	return tm;
}

Matrix ObjectCell::GetScalingMatrix(Vector sv) {
	Matrix sm;
  	sm.Identity();
  	sm.SetValue(0, 0, sv.GetX());
  	sm.SetValue(1, 1, sv.GetY());
  	sm.SetValue(2, 2, sv.GetZ());
	return sm;
}

//*******************************************************************
// rotation en degre
// angle positif -> rotation anti-horaire
//*******************************************************************
Matrix ObjectCell::GetRotationMatrix(Vector rv) {
	Matrix rm;
  	Matrix xRot, yRot, zRot;
  	Matrix temp;

	xRot.Identity();
  	yRot.Identity();
  	zRot.Identity();

  	rv.SetX( InRadians(rv.GetX()));
  	rv.SetY( InRadians(rv.GetY()));
  	rv.SetZ( InRadians(rv.GetZ()));

  	xRot.SetValue(1, 1, cos(rv.GetX()));
  	xRot.SetValue(1, 2, sin(rv.GetX()));
  	xRot.SetValue(2, 1, -sin(rv.GetX()));
  	xRot.SetValue(2, 2, cos(rv.GetX()));

  	yRot.SetValue(0, 0, cos(rv.GetY()));
  	yRot.SetValue(0, 2, -sin(rv.GetY()));
  	yRot.SetValue(2, 0, sin(rv.GetY()));
  	yRot.SetValue(2, 2, cos(rv.GetY()));

  	zRot.SetValue(0, 0, cos(rv.GetZ()));
  	zRot.SetValue(0, 1, sin(rv.GetZ()));
  	zRot.SetValue(1, 0, -sin(rv.GetZ()));
  	zRot.SetValue(1, 1, cos(rv.GetZ()));

  	rm = zRot * yRot;
  	rm = xRot * rm;
	return rm;
}
