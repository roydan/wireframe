#ifndef MATRIX_H
#define MATRIX_H

#include <ostream>

const int MATRIX_SIZE = 4;

using namespace std;

/**********************************************************
 *
 * classe MyMatrix
 *
 *********************************************************/

class MyMatrix {
private:
	int nbLignes;
	int nbColonnes;
	double matrix[MATRIX_SIZE][MATRIX_SIZE];

public:
	MyMatrix();
	MyMatrix (const MyMatrix & m);		// constructeur de recopie
	~MyMatrix();						// destructeur

    friend MyMatrix operator * (MyMatrix m1, MyMatrix m2);

	void Zero();
	void Identity();

	double GetValue (int row, int col);
	void SetValue (int row, int col, double value);

	double * operator [] (int row) {
    	return matrix[row];
	}

	const double * operator [] (int row) const {
		return matrix[row];
	}
};

ostream & operator << (ostream & os, const MyMatrix & m);

#endif   // MATRIX_H
