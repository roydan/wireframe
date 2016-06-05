#ifndef POPFILE_H
#define POPFILE_H

#include <windows.h>
#include "Util.h"

struct FileStruct {
    OPENFILENAME opfn;
    char lpstrFilter[MAX_STRING_LENGTH];
    char lpstrDefExt[MAX_STRING_LENGTH];

    void PopFileInit (HWND hwnd);
    BOOL PopFileOpen (HWND hwnd, PSTR pstrFileName, PSTR pstrTitleName);
};

#endif   // POPFILE_H

