#include <cmath>
#include <iostream>
#include <iomanip>

#include "MyVector.h"

/**********************************************************
 * 
 * vecteur 3D
 * 
 *********************************************************/

/**********************************************************
 * 
 * MyVector
 * 
 * parameters IN:
 * 	double x	default = 0.0
 *	double y	default = 0.0
 *	double z	default = 0.0
 * 
 * return value : MyVector *
 * 
 *********************************************************/
MyVector::MyVector (double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

/**********************************************************
 * 
 * MyVector
 * 
 * constructeur de recopie
 * 
 * parameters IN:
 * 	const MyVector & v
 * 
 * return value : MyVector *
 * 
 *********************************************************/
MyVector::MyVector (const MyVector & v) {
	x = v.x;
	y = v.y;
	z = v.z;
}

/**********************************************************
 * 
 * DotProduct
 * 
 * parameters IN:
 * 	const MyVector & v
 * 
 * return value : double
 * 
 *********************************************************/
double MyVector::DotProduct (const MyVector & v) {
  	return (x * v.x) + (y * v.y) + (z * v.z);
}

/**********************************************************
 * 
 * Normalize
 * 
 *********************************************************/
void MyVector::Normalize() {
  	double denominator = Magnitude();
	if (denominator != 0) {
  	  	x /= denominator;
  	  	y /= denominator;
  	  	z /= denominator;
  	}
}

/**********************************************************
 * 
 * Magnitude
 * 
 * return value : double
 * 
 *********************************************************/
double MyVector::Magnitude() {
  	return sqrt (x*x + y*y + z*z);
}

/**********************************************************
 * 
 * VectorMatrix
 * 
 * parameters IN:
 * 	const MyVector & v
 *	MyMatrix m
 * 
 *********************************************************/
void MyVector::VectorMatrix (const MyVector & v, MyMatrix m) {
  	x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0];
  	y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1];
  	z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2];
}

/**********************************************************
 * 
 * VectorMatrix
 * 
 * parameters IN:
 *	MyMatrix m
 * 
 *********************************************************/
void MyVector::VectorMatrix (MyMatrix m) {
	MyVector v = MyVector(x,y,z);
  	x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0];
  	y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1];
  	z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2];
}

/**********************************************************
 * 
 * operator <<
 * 
 * parameters IN :
 *	ostream &os
 *	const MyVector &v
 * 
 * return value : ostream &
 * 
 *********************************************************/
ostream& operator << (ostream & os, const MyVector & v) {
	os << "[ " << v.GetX() << " , " << v.GetY() << " , " << v.GetZ() << " ]" << endl;
	return os;
}

