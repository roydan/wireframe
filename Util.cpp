#include <cstdio>

#include "Util.h"

using namespace std;

/**********************************************************
 *
 * classe Util
 *
 *********************************************************/
 
/**********************************************************
 * InRadians
 * 
 * parameters IN :
 *	double degrees
 * 
 * return value : double
 *********************************************************/
double Util::InRadians (double degrees) {
  	return (degrees * Util::PI() / 180.0);
}

/**********************************************************
 * public members
 *********************************************************/

/**********************************************************
 * WritePrivateProfileInt
 * 
 * parameters IN :
 *	char * ApplicationName
 * 	char * keyName
 * 	int value
 * 	const char * fileName
 * 
 * return value : BOOL
 *********************************************************/
BOOL WritePrivateProfileInt (char * ApplicationName, char * keyName, int value, const char * fileName) {
    char str[MAX_STRING_LENGTH];
 	sprintf (str, "%d", value);
 	return WritePrivateProfileString (ApplicationName, keyName, str, fileName);
}

/**********************************************************
 * GetDlgItemDouble
 * 
 * parameters IN :
 *	HWND hDlg
 *	int idDlgItem
 * 
 * return value : double
 *********************************************************/
double GetDlgItemDouble (HWND hDlg, int idDlgItem) {
    char str[MAX_STRING_LENGTH];
    double value;
    GetDlgItemText (hDlg, idDlgItem, str, MAX_STRING_LENGTH);
    sscanf (str, "%lf", &value);
    return value;
}

/**********************************************************
 * SetDlgItemDouble
 * 
 * parameters IN :
 *	HWND hDlg
 *	int idDlgItem
 *  double value
 * 
 * return value : none
 *********************************************************/
void SetDlgItemDouble (HWND hDlg, int idDlgItem, double value) {
  	char str[MAX_STRING_LENGTH];
  	sprintf (str, "%.2f", value);
  	SetDlgItemText (hDlg, idDlgItem, str);
}

