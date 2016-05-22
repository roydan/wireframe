#ifndef UTIL_H
#define UTIL_H

#include <windows.h>

using namespace std;

const int MAX_STRING_LENGTH = 100;

/**********************************************************
 *
 * classe Util
 *
 *********************************************************/

class Util {
public:
	//?? static const double PI = 3.1415926535;    // ISO C++ forbids initialization of member constant `PI' of non-integral type `const double' 
	static double PI() { return 3.1415926535; }
	static double InRadians (double);
};

/**********************************************************
 * public members
 *********************************************************/

BOOL WritePrivateProfileInt (char * ApplicationName, char * keyName, int value, const char * fileName);
double GetDlgItemDouble (HWND hDlg, int idDlgItem);
void SetDlgItemDouble (HWND hDlg, int idDlgItem, double value);

#endif   // UTIL_H
