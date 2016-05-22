#ifndef POPFILE_H
#define POPFILE_H

#include <windows.h>
#include "Util.h"

struct FileStruct {
    OPENFILENAME opfn;
    char lpstrFilter[MAX_STRING_LENGTH];
    char lpstrDefExt[MAX_STRING_LENGTH];
    void PopFileInit(HWND hwnd);
    BOOL PopFileOpen(HWND hwnd, PSTR pstrFileName, PSTR pstrTitleName);
};

/*
void PopFileInitialize(HWND hwnd, const char * lpstrFilter, const char * lpstrDefExt);
BOOL PopFileOpenDlg(HWND, PSTR, PSTR);
BOOL PopFileSaveDlg(HWND, PSTR, PSTR);
long PopFileLength(FILE *);
bool PopFileRead(HWND, PSTR);
bool PopFileWrite(HWND, PSTR);
*/

#endif   // POPFILE_H
