#include <cstdio>

#include "Util.h"

double InRadians(double degrees) {
  	return (degrees * Util::PI() / 180.0);
}

BOOL WritePrivateProfileInt(char * ApplicationName, char * keyName, int value, const char * fileName) {
    char str[MAX_STRING_LENGTH];
 	sprintf(str, "%d", value);
 	return WritePrivateProfileString(ApplicationName, keyName, str, fileName);
}

double GetDlgItemDouble(HWND hDlg, int idDlgItem) {
    char str[MAX_STRING_LENGTH];
    double value;
    GetDlgItemText(hDlg, idDlgItem, str, MAX_STRING_LENGTH);
    sscanf(str, "%lf", &value);
    return value;
}

void SetDlgItemDouble(HWND hDlg, int idDlgItem, double value) {
  	char str[MAX_STRING_LENGTH];
  	sprintf(str, "%.2f", value);
  	SetDlgItemText(hDlg, idDlgItem, str);
}
