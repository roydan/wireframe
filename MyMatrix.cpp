#include <iostream>
#include <iomanip>
#include <ostream>

#include "MyMatrix.h"

/**********************************************************
 * 
 * MyMatrix
 * 
 * return value : MyMatrix *
 * 
 *********************************************************/
MyMatrix::MyMatrix() {
	nbLignes = MATRIX_SIZE;
    nbColonnes = MATRIX_SIZE;

	// initialise tous les éléments du tableau des valeurs à 0
	Zero();
}

/**********************************************************
 * 
 * MyMatrix
 * 
 * constructeur de recopie
 * 
 * parameters IN :
 *	const MyMatrix & m
 * 
 * return value : MyMatrix *
 * 
 *********************************************************/
MyMatrix::MyMatrix (const MyMatrix & m) {
    nbLignes = m.nbLignes;
    nbColonnes = m.nbColonnes;

  	for (int i = 0; i < nbLignes; i++) {
  	  	for (int j = 0; j < nbColonnes; j++) {
    		matrix[i][j] = m.matrix[i][j];
		}
	}
}

/**********************************************************
 * 
 * ~MyMatrix
 * 
 * destructeur
 * 
 * TODO
 * 
 *********************************************************/
MyMatrix::~MyMatrix() {
}

/**********************************************************
 * 
 * operator *
 * 
 * operateur de multiplication
 * fonction amie (friend)
 * 
 * parameters IN :
 *	MyMatrix m1
 *	MyMatrix m2
 * 
 * return value : MyMatrix
 * 
 *********************************************************/
MyMatrix operator * (MyMatrix m1, MyMatrix m2) {
	MyMatrix m3;
  	m3.Zero();

  	for (int i = 0; i < MATRIX_SIZE; i++) {
  	  	for (int j = 0; j < MATRIX_SIZE; j++) {
			for (int k = 0; k < MATRIX_SIZE; k++) {
  	      		m3.matrix[i][j] = m3.matrix[i][j] + m1.matrix[i][k] * m2.matrix[k][j];
			}
		}
	}
	return m3;
}

/**********************************************************
 * 
 * Zero
 * 
 * initialise tous les éléments du tableau des valeurs à 0
 * 
 *********************************************************/
void MyMatrix::Zero() {
  	for (int i = 0; i < nbLignes; i++) {
  	  	for (int j = 0; j < nbColonnes; j++) {
  	    	matrix[i][j] = 0.;
		}
	}
}

/**********************************************************
 * 
 * Identity
 * 
 *********************************************************/
void MyMatrix::Identity() {
  	Zero();
  	for (int diag = 0; diag < MATRIX_SIZE; diag++) {
  	  	matrix[diag][diag] = 1.0;
	}
}

/**********************************************************
 * 
 * GetValue
 * 
 * parameters IN :
 *	int row
 *	int col
 * 
 * return value : double
 * 
 *********************************************************/
double MyMatrix::GetValue (int row, int col) {
	return matrix[row][col];
}

/**********************************************************
 * 
 * SetValue
 * 
 * parameters IN :
 *	int row
 *	int col
 *	double value
 * 
 *********************************************************/
void MyMatrix::SetValue (int row, int col, double value) {
	matrix[row][col] = value;
}

/**********************************************************
 * 
 * operator <<
 * 
 * parameters IN :
 *	ostream &os
 *	const MyMatrix &m
 * 
 * return value : ostream &
 * 
 *********************************************************/
ostream& operator << (ostream & os, const MyMatrix & m) {
	//?? os << setprecision (8) << setiosflags (ios::right | ios::showpoint);
  	for (int i = 0; i < MATRIX_SIZE; i++) {
  	  	for (int j = 0; j < MATRIX_SIZE; j++) {
			os << setw(8) << m[i][j];
		}
		os << endl;
	}
	return os;
}
