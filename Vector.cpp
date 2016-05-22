#include <cmath>

#include "Vector.h"
#include "Matrix.h"

// constructeur
Vector::Vector(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

// constructeur de recopie
Vector::Vector(const Vector & v) {
	x = v.x;
	y = v.y;
	z = v.z;
}

double Vector::DotProduct(const Vector & v) {
  	return (x * v.x) + (y * v.y) + (z * v.z);
}

void Vector::Normalize() {
  	double denominator = Magnitude();
	if (denominator != 0) {
  	  	x /= denominator;
  	  	y /= denominator;
  	  	z /= denominator;
  	}
}

double Vector::Magnitude() {
  	return sqrt(x*x + y*y + z*z);
}

void Vector::VectorMatrix(const Vector & v, Matrix transformation) {
  	x = v.x * transformation.GetValue(0, 0) + v.y * transformation.GetValue(1, 0) + v.z * transformation.GetValue(2, 0) + transformation.GetValue(3, 0);
  	y = v.x * transformation.GetValue(0, 1) + v.y * transformation.GetValue(1, 1) + v.z * transformation.GetValue(2, 1) + transformation.GetValue(3, 1);
  	z = v.x * transformation.GetValue(0, 2) + v.y * transformation.GetValue(1, 2) + v.z * transformation.GetValue(2, 2) + transformation.GetValue(3, 2);
}
