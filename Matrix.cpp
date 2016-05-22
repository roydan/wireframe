#include <iostream>

#include "Matrix.h"

// constructeur
Matrix::Matrix() {
	nbLignes = MATRIX_SIZE;
    nbColonnes = MATRIX_SIZE;

	// initialise tous les éléments du tableau des valeurs à 0
	Zero();
}

// constructeur de recopie
Matrix::Matrix(const Matrix & m) {
    nbLignes = m.nbLignes;
    nbColonnes = m.nbColonnes;

  	for (int i = 0; i < nbLignes; i++) {
  	  	for (int j = 0; j < nbColonnes; j++) {
    		matrix[i][j] = m.matrix[i][j];
		}
	}
}

// destructeur
Matrix::~Matrix() {
}

/**************************************
 * operateur de multiplication
 * fonction amie (friend)
 *************************************/
Matrix operator * (Matrix m1, Matrix m2) {
	Matrix m3;
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

// initialise tous les éléments du tableau des valeurs à 0
void Matrix::Zero() {
  	for (int i = 0; i < nbLignes; i++) {
  	  	for (int j = 0; j < nbColonnes; j++) {
  	    	matrix[i][j] = 0.;
		}
	}
}

void Matrix::Identity() {
  	Zero();
  	for (int diag = 0; diag < MATRIX_SIZE; diag++) {
  	  	matrix[diag][diag] = 1.0;
	}
}

double Matrix::GetValue(int row, int col) {
	return matrix[row][col];
}

void Matrix::SetValue(int row, int col, double value) {
	matrix[row][col] = value;
}
