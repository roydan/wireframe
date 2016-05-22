#include <iostream>

#include "Matrix4x4.h"

using namespace std;

// constructeur
Matrix4x4::Matrix4x4() {
	nbLignes = MATRIX_4X4_SIZE;
    nbColonnes = MATRIX_4X4_SIZE;

    tabValeurs = new double[nbLignes * nbColonnes];	// allocation du tableau des valeurs
    tabPtrs = new double*[nbLignes];		// allocation du tableau de pointeurs vers les lignes
	allocTest();							// test l'allocation de m�moire

	Zero();
}

// constructeur de recopie
Matrix4x4::Matrix4x4(const Matrix4x4 & m) {
    nbLignes = m.nbLignes;
    nbColonnes = m.nbColonnes;

    // instancie les 2 tableaux requis
    tabValeurs = new double[nbLignes * nbColonnes];
    tabPtrs = new double*[nbLignes];
	allocTest();		// test l'allocation de m�moire

	recopieTab(m);		// recopie les valeurs du tableau en entr�e
}

/********************************************************************
 * destructeur
 * d�truit les tableaux
 *******************************************************************/
Matrix4x4::~Matrix4x4() {
    delete [] tabPtrs;		// d�truit le tableau de pointeurs vers les lignes
    delete [] tabValeurs;	// d�truit le tableau des valeurs
}

/********************************************************************
 * surcharge de l'op�rateur d'indexation constant
 *
 *   cout << x1[i];
 *   cout << x1.operator[](i);
 *
 * entr�e:	int i		index de l'�l�ment du tableau
 * sortie:	double *
 *******************************************************************/
double * Matrix4x4::operator[](int i) const {
	if ((i >= 0) && (i < nbLignes)) {
		return tabPtrs[i];
	} else {
		cerr << "Erreur: Matrix4x4::operator[] Mauvais index : i = " << i << endl;
		exit(1);
	}
}

/********************************************************************
 * surcharge de l'op�rateur d'indexation non constant
 *
 * entr�e:	int i		index de l'�l�ment du tableau
 * sortie:	(double*)&		
 *******************************************************************/
ptrDouble & Matrix4x4::operator[](int i) {
	if ((i >= 0) && (i < nbLignes)) {
		return tabPtrs[i];
	} else {
		cerr << "Erreur: Matrix4x4::operator[] Mauvais index : i = " << i << endl;
		exit(1);
	}
}

/**************************************
 * operateur de multiplication
 * fonction amie (friend)
 *************************************/
Matrix4x4 operator * (Matrix4x4 m1, Matrix4x4 m2) {
  	int i, j, k;
	Matrix4x4 m3;
  	m3.Zero();

  	for (i = 0; i < MATRIX_4X4_SIZE; i++) {
  	  	for (j = 0; j < MATRIX_4X4_SIZE; j++) {
			for (k = 0; k < MATRIX_4X4_SIZE; k++) {
  	      		m3.matrix[i][j] = m3.matrix[i][j] + m1.matrix[i][k] * m2.matrix[k][j];
			}
		}
	}
	return m3;
}

/********************************************************************
 * test l'allocation de m�moire
 *
 * entr�e:	aucune
 * sortie:	aucune
 *******************************************************************/
void Matrix4x4::allocTest() {
    if ((tabValeurs == NULL) || (tabPtrs == NULL)) {
        cerr << "Erreur: L'allocation de la m�moire a �chou�!\n";
        exit(1);
    }
}

/********************************************************************
 * recopie les valeurs du tableau en entr�e
 * m�thode priv�e utilis�e par le constructeur de recopie ainsi que par l'op�rateur d'affectation
 *
 * entr�e:	Matrix4x4 & m	   tableau 2D de valeurs enti�res
 * sortie:	aucune
 *******************************************************************/
void Matrix4x4::recopieTab(const Matrix4x4 & m) {
	// recopie les valeurs du tableau en entr�e
    double * ptrTabSrc = m.tabValeurs;
    double * ptrTabDest = tabValeurs;
    for (int i = 0; i < nbLignes; i++) {
        tabPtrs[i] = ptrTabDest;         // initialise le tableau de pointeurs vers les lignes
        for (int j = 0; j < nbColonnes; j++) {
            *ptrTabDest++ = *ptrTabSrc++;
        }
    }
}

// initialise tous les �l�ments du tableau des valeurs � 0
void Matrix4x4::Zero() {
    double * ptr = tabValeurs;
    for (int i = 0; i < nbLignes; i++) {
        tabPtrs[i] = ptr;         // initialise le tableau de pointeurs vers les lignes
        for (int j = 0; j < nbColonnes; j++) {
            *ptr++ = 0.;
        }
    }
}

void Matrix4x4::Identity() {
  	Zero();

    double * ptr = tabValeurs;
    for (int diag = 0; diag < nbColonnes; diag++) {
        ptr += diag * nbLignes + diag;
        *ptr = 1.0;
    }
}

double Matrix4x4::GetValue(int row, int col) {
    double * ptr = tabValeurs + row * nbLignes + col;
    double val = *ptr;
	return val;
}

void Matrix4x4::SetValue(int row, int col, double value) {
    double * ptr = tabValeurs + row * nbLignes + col;
    *ptr = value;
}
