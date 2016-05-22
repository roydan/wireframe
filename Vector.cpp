#include <cmath>
#include <iostream>
#include <iomanip>

#include "Vector.h"
#include "Matrix.h"

/**********************************************************
 * Vector
 * 
 * constructeur
 * 
 * parameters IN:
 * 	double x	default = 0.0
 *	double y	default = 0.0
 *	double z	default = 0.0
 * 
 * return value : Vector *
 *********************************************************/
Vector::Vector (double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

/**********************************************************
 * Vector
 * 
 * constructeur de recopie
 * 
 * parameters IN:
 * 	const Vector & v
 * 
 * return value : Vector *
 *********************************************************/
Vector::Vector (const Vector & v) {
	x = v.x;
	y = v.y;
	z = v.z;
}

/**********************************************************
 * DotProduct
 * 
 * parameters IN:
 * 	const Vector & v
 * 
 * return value : double
 *********************************************************/
double Vector::DotProduct (const Vector & v) {
  	return (x * v.x) + (y * v.y) + (z * v.z);
}

/**********************************************************
 * Normalize
 * 
 * parameters IN: none
 * 
 * return value : none
 *********************************************************/
void Vector::Normalize() {
  	double denominator = Magnitude();
	if (denominator != 0) {
  	  	x /= denominator;
  	  	y /= denominator;
  	  	z /= denominator;
  	}
}

/**********************************************************
 * Magnitude
 * 
 * parameters IN: none
 * 
 * return value : double
 *********************************************************/
double Vector::Magnitude() {
  	return sqrt (x*x + y*y + z*z);
}

/**********************************************************
 * VectorMatrix
 * 
 * parameters IN:
 * 	const Vector & v
 *	Matrix m
 * 
 * return value : none
 *********************************************************/
void Vector::VectorMatrix (const Vector & v, Matrix m) {
  	x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0];
  	y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1];
  	z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2];
}

/**********************************************************
 * operator <<
 * 
 * parameters IN :
 *	ostream &os
 *	const Vector &v
 * 
 * return value : ostream &
 *********************************************************/
ostream& operator << (ostream & os, const Vector & v) {
	os << "[ " << v.GetX() << " , " << v.GetY() << " , " << v.GetZ() << " ]" << endl;
	return os;
}
