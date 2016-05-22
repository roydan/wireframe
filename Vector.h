#ifndef VECTOR_H
#define VECTOR_H

#include "Matrix.h"

using namespace std;

// structure pour la representation d'un vecteur 3D
class Vector {
private:
  	double x, y, z;

public:
	Vector(double x = 0., double y = 0., double z = 0.);
	Vector(const Vector & v);
	double GetX() { return x; }
	double GetY() { return y; }
	double GetZ() { return z; }
	void SetX(double value) { x = value; }
	void SetY(double value) { y = value; }
	void SetZ(double value) { z = value; }
	double DotProduct(const Vector & v);
	void Normalize();
	double Magnitude();
	void VectorMatrix(const Vector &, Matrix);
};

#endif   // VECTOR_H
