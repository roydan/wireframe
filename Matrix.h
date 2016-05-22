#ifndef MATRIX_H
#define MATRIX_H

#include <ostream>

const int MATRIX_SIZE = 4;

using namespace std;

/**********************************************************
 *
 * classe Matrix
 *
 *********************************************************/

class Matrix {
private:
	int nbLignes;
	int nbColonnes;
	double matrix[MATRIX_SIZE][MATRIX_SIZE];

public:
	Matrix();
	Matrix (const Matrix & m);		// constructeur de recopie
	~Matrix();						// destructeur

    friend Matrix operator * (Matrix m1, Matrix m2);

	void Zero();
	void Identity();

	double GetValue (int row, int col);
	void SetValue (int row, int col, double value);

	double* operator [] (int row) {
    	return matrix[row];
	}

	const double* operator [] (int row) const {
		return matrix[row];
	}
};

ostream& operator << (ostream &os, const Matrix &m);

#endif   // MATRIX_H
