#ifndef VECTOR_H
#define VECTOR_H

#include "MyMatrix.h"

using namespace std;

/**********************************************************
 *
 * vecteur 3D
 *
 *********************************************************/

class MyVector {
private:
  	double x, y, z;

public:
	MyVector (double x = 0., double y = 0., double z = 0.);
	MyVector (const MyVector & v);

	double DotProduct (const MyVector & v);
	void Normalize();
	double Magnitude();
	void VectorMatrix (const MyVector &, MyMatrix);
	void VectorMatrix (MyMatrix);
	
	double GetX() const { return x; }
	double GetY() const { return y; }
	double GetZ() const { return z; }
	void SetX (double value) { x = value; }
	void SetY (double value) { y = value; }
	void SetZ (double value) { z = value; }
};

ostream & operator << (ostream & os, const MyVector & v);

#endif   // VECTOR_H

