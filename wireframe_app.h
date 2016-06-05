#ifndef WIREFRAME_APP_H
#define WIREFRAME_APP_H

using namespace std;

#define IDM_NEW  10
#define IDM_CLEAR  20
#define IDM_OPEN 11
#define IDM_OPEN_SCENE 21
#define IDM_EXIT 12

#define IDM_HELP  40
#define IDM_ABOUT 41

// prototypes
void RefreshScreen (HWND hWnd);
void DrawVertexNormalsCtrl (HWND hWnd);

static void TestSub (void);

#endif   // WIREFRAME_APP_H

