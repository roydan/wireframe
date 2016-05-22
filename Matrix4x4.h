#ifndef MATRIX_4X4_H
#define MATRIX_4X4_H

typedef double* ptrDouble;

const int MATRIX_4X4_SIZE = 4;

using namespace std;

class Matrix4x4 {
private:
	int nbLignes;
	int nbColonnes;
	double * tabValeurs;		// tableau des valeurs
	double ** tabPtrs;			// tableau de pointeurs vers les lignes du tableau des valeurs

	void allocTest();			// test l'allocation de mémoire
	void recopieTab(const Matrix4x4 & m);

public:
	Matrix4x4();
	Matrix4x4(const Matrix4x4 & m);			// constructeur de recopie
	~Matrix4x4();							// destructeur
	double * operator[](int i) const;       // opérateur d'indexation constant
	ptrDouble & operator[](int i);			// opérateur d'indexation non constant
    friend Matrix4x4 operator * (Matrix4x4 m1, Matrix4x4 m2);

	void Zero();
	void Identity();
	double GetValue(int row, int col);
	void SetValue(int row, int col, double value);
};

#endif   // MATRIX_4X4_H
