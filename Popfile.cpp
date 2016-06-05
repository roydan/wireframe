/**********************************************************
 *
 * POPFILE.CPP
 * Popup Editor File Functions
 *
 *********************************************************/

#include <windows.h>
#include <cstdio>

#include "popfile.h"

/**********************************************************
 * 
 * PopFileInit
 * 
 * parameters IN:
 * 	HWND hwnd
 * 
 *********************************************************/
void FileStruct::PopFileInit (HWND hwnd)
{
	opfn.lStructSize       = sizeof (OPENFILENAME);
	opfn.hwndOwner         = hwnd;
	opfn.hInstance         = NULL;
	opfn.lpstrFilter       = lpstrFilter;
	opfn.lpstrCustomFilter = NULL;
	opfn.nMaxCustFilter    = 0;
	opfn.nFilterIndex      = 0;
	opfn.lpstrFile         = NULL;          // Set in Open and Close functions
	opfn.nMaxFile          = _MAX_PATH;
	opfn.lpstrFileTitle    = NULL;          // Set in Open and Close functions
	opfn.nMaxFileTitle     = _MAX_FNAME + _MAX_EXT;
	opfn.lpstrInitialDir   = NULL;
	opfn.lpstrTitle        = NULL;
	opfn.Flags             = 0;             // Set in Open and Close functions
	opfn.nFileOffset       = 0;
	opfn.nFileExtension    = 0;
	opfn.lpstrDefExt       = lpstrDefExt;
	opfn.lCustData         = 0L;
	opfn.lpfnHook          = NULL;
	opfn.lpTemplateName    = NULL;
}

/**********************************************************
 * 
 * PopFileOpen
 * 
 * parameters IN:
 * 	HWND hwnd
 * 	PSTR pstrFileName
 * 	PSTR pstrTitleName
 * 
 * return value : BOOL
 * 
 **********************************************************
 * 
 * If the user specifies a file name and clicks the OK button, the return value
 * is nonzero. The buffer pointed to by the lpstrFile member of the OPENFILENAME
 * structure contains the full path and file name specified by the user.
 *
 * If the user cancels or closes the Open dialog box or an error occurs, the
 * return value is zero. To get extended error information, call the
 * CommDlgExtendedError function, which can return one of the following values.
 * 
 *********************************************************/
BOOL FileStruct::PopFileOpen (HWND hwnd, PSTR pstrFileName, PSTR pstrTitleName)
{
    opfn.hwndOwner      = hwnd;
    opfn.lpstrFile      = pstrFileName;
    opfn.lpstrFileTitle = pstrTitleName;
    opfn.Flags          = OFN_HIDEREADONLY | OFN_CREATEPROMPT;

    return GetOpenFileName (&opfn);
}

/**********************************************************
BOOL PopFileSaveDlg(HWND hwnd, PSTR pstrFileName, PSTR pstrTitleName)
{
    ofn.hwndOwner      = hwnd;
    ofn.lpstrFile      = pstrFileName;
    ofn.lpstrFileTitle = pstrTitleName;
    ofn.Flags          = OFN_OVERWRITEPROMPT;

    return GetSaveFileName (&ofn);
}

long PopFileLength(FILE *file)
{
    long iCurrentPos, iFileLength;

    iCurrentPos = ftell (file);
    fseek (file, 0, SEEK_END);
    iFileLength = ftell (file);
    fseek (file, iCurrentPos, SEEK_SET);
    return iFileLength;
}

bool PopFileRead(HWND hwndEdit, PSTR pstrFileName)
{
    FILE * file;
    int    iLength;
    PSTR   pstrBuffer;

    if (NULL == (file = fopen (pstrFileName, "rb")))
    	return false;

    iLength = PopFileLength(file);

    if (NULL == (pstrBuffer = (PSTR) malloc (iLength))) {
        fclose (file);
        return false;
    }

    fread(pstrBuffer, 1, iLength, file);
    fclose (file);
    pstrBuffer[iLength] = '\0';

    SetWindowText (hwndEdit, pstrBuffer);
    free (pstrBuffer);

    return true;
}

bool PopFileWrite (HWND hwndEdit, PSTR pstrFileName)
{
    FILE  *file;
    int    iLength;
    PSTR   pstrBuffer;

    if (NULL == (file = fopen (pstrFileName, "wb")))
        return false;

    iLength = GetWindowTextLength(hwndEdit);

    if (NULL == (pstrBuffer = (PSTR) malloc (iLength + 1))) {
        fclose (file);
        return false;
    }

    GetWindowText (hwndEdit, pstrBuffer, iLength + 1);

    if (iLength != (int)fwrite (pstrBuffer, 1, iLength, file)) {
        fclose (file);
        free (pstrBuffer);
        return false;
    }

    fclose (file);
    free (pstrBuffer);

    return true;
}
**********************************************************/

