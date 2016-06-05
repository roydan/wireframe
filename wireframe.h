#ifndef WIREFRAME_H
#define WIREFRAME_H
/********************************************************************************
 *
 * v120
 * 31/05/2012
 *
 ********************************************************************************
 *
 * v110
 * 23/05/2012
 *
 ********************************************************************************
 *
 * v100
 * 21/05/2012
 * Original Baseline
 *
 ********************************************************************************/
#include <windows.h>

#include "MyVector.h"
#include "ViewPointRec.h"

// version
#define WIREFRAME_VERSION          120

// variables globales

#define WIREFRAME_WM_CREATE        1
#define WIREFRAME_LOAD_OBJECT      2
#define WIREFRAME_LOAD_SCENE       9
#define WIREFRAME_WM_PAINT         3
#define WIREFRAME_RENDER_SCENE     4
#define WIREFRAME_TRANSFORM_OBJECT 5
#define WIREFRAME_VIEW_POINT       6
#define WIREFRAME_WM_DESTROY       7
#define WIREFRAME_ABOUT            8

// prototypes

int WireframeFunction (int, HWND, WPARAM, LPARAM);
INT_PTR CALLBACK TransformationDlgProc (HWND, UINT, WPARAM, LPARAM);
void InitTransformationDlg(HWND, MyVector *, MyVector *, MyVector *);
INT_PTR CALLBACK ViewRefPointDlgProc (HWND, UINT, WPARAM, LPARAM);
void InitViewRefPointDlg(HWND, ViewPointRec *);
INT_PTR CALLBACK AboutBoxDlgProc (HWND, UINT, WPARAM, LPARAM);

#endif   // WIREFRAME_H

