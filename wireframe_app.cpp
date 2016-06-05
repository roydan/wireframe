#include <iostream>
#include <cstdio>
#include <windows.h>

#include "wireframe_app.h"
#include "resource.h"
#include "ObjectScene.h"
#include "wireframe.h"
#include "popfile.h"
#include "guicon.h"

extern ObjectScene * ptrScene;

// prototypes
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

/**********************************************************
 * 
 * WinMain
 * 
 * parameters IN:
 * 	HINSTANCE hInstance
 *	HINSTANCE hPrevInstance
 *	PSTR szCmdLine
 *	int iCmdShow
 * 
 * return value : int
 * 
 *********************************************************/
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   PSTR szCmdLine, int iCmdShow)
{
	HWND     hwnd;
	MSG      msg;
	WNDCLASS wndclass;
    char     szAppName[] = "WireFrame";
    char     szName[] = "WireFrame";

	RedirectIOToConsole();

	if (!hPrevInstance) {
		wndclass.style         = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc   = WndProc;
		wndclass.cbClsExtra    = 0;
		wndclass.cbWndExtra    = 0;
		wndclass.hInstance     = hInstance;
		wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
		wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)GetStockObject (WHITE_BRUSH);
		wndclass.lpszMenuName  = szName;
		wndclass.lpszClassName = szName;

		RegisterClass (&wndclass);
	}

	hwnd = CreateWindow(szName, szAppName,
		                WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        CW_USEDEFAULT, CW_USEDEFAULT,
		                NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage (&msg, NULL, 0, 0)) {
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}

	return msg.wParam;
}

/**********************************************************
 * 
 * WndProc
 * 
 * parameters IN:
 * 	HWND hWnd
 *	UINT iMsg
 *		WM_CREATE
 *		WM_COMMAND
 *	  	  	LOWORD(wParam)
 *				IDM_NEW
 *				IDM_CLEAR
 *	  	  	  	IDM_OPEN
 *	  	  	  	IDM_OPEN_SCENE
 *				ID_WIREFRAME_WIREFRAMESCENE
 *	  	  	  	IDM_RENDER_SCENE
 *				IDM_TRANSFORMATION
 *	  	  	  	IDM_VIEW_POINT
 *	  	  	  	ID_OPTIONS_DRAWVERTEXNORMALS
 *				IDM_HELP
 *	  	  	  	IDM_ABOUT
 *	  	  	  	IDM_EXIT
 * 
 *	  	WM_PAINT
 *	  	WM_CLOSE
 *	  	WM_DESTROY
 * 
 *	WPARAM wParam
 *	LPARAM lParam
 * 
 * return value : LRESULT
 * 
 *********************************************************/
LRESULT CALLBACK WndProc (HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
  	switch (iMsg) {
  	  	case WM_CREATE:
		{
			WireframeFunction (WIREFRAME_WM_CREATE, hWnd, 0, lParam);
            DrawVertexNormalsCtrl (hWnd);
			return 0;
		}

  	  	case WM_COMMAND:
		{
  	  	  	switch (LOWORD(wParam)) {
				case IDM_NEW:
               	{
                }
                
				case IDM_CLEAR:
               	{
				    WireframeFunction (WIREFRAME_WM_DESTROY, 0, 0, 0);
				    WireframeFunction (WIREFRAME_WM_CREATE, hWnd, 0, 0);
					RefreshScreen (hWnd);
					return 0;
           		}

  	  	  	  	case IDM_OPEN:
               	{
  	  	  	    	// Load an object
					if (!WireframeFunction (WIREFRAME_LOAD_OBJECT, hWnd, 0, 0)) {
				    	RefreshScreen (hWnd);
					}
					return 0;
           		}

  	  	  	  	case IDM_OPEN_SCENE:
               	{
  	  	  	    	// Load an object
					if (!WireframeFunction (WIREFRAME_LOAD_SCENE, hWnd, 0, 0)) {
				    	RefreshScreen (hWnd);
					}
					return 0;
           		}

				case ID_WIREFRAME_WIREFRAMESCENE:
               	{
  	  	  	    	// WireFrame Scene
				    RefreshScreen (hWnd);
		    		return 0;
				}

  	  	  	  	case IDM_RENDER_SCENE:
               	{
					if (!WireframeFunction (WIREFRAME_RENDER_SCENE, hWnd, 0, 0)) {
				    	RefreshScreen (hWnd);
					}
					return 0;
				}

				case IDM_TRANSFORMATION:
				{
  	  	  	    	// Transform an object
					WireframeFunction (WIREFRAME_TRANSFORM_OBJECT, hWnd, 0, 0);
				    return 0;
				}

  	  	  	  	case IDM_VIEW_POINT:
				{
  	  	  	    	// Move view point
					WireframeFunction (WIREFRAME_VIEW_POINT, hWnd, 0, 0);
					return 0;
				}

  	  	  	  	case ID_OPTIONS_DRAWVERTEXNORMALS:
				{
  	  	  	    	ptrScene->drawVertexNormals = !(ptrScene->drawVertexNormals);
                    DrawVertexNormalsCtrl (hWnd);
				    RefreshScreen (hWnd);
					return 0;
				}

				case IDM_HELP:
				{
				    // Messages from Help menu
  	  	  	    	MessageBox (NULL, "La fonction Aide n'est pas encore disponible!", "Aide", MB_OK);
				    return 0;
				}

  	  	  	  	case IDM_ABOUT:
				{
                     TestSub();       //??
                    
					WireframeFunction (WIREFRAME_ABOUT, hWnd, 0, 0);
				    return 0;
				}

  	  	  	  	case IDM_EXIT:
				{
  	  	  	    	DestroyWindow(hWnd);
				    return 0 ;
				}
			}
		}	// WM_COMMAND

  	  	case WM_PAINT:
        {
  	        PAINTSTRUCT  ps;
  	  	  	HDC hDC = BeginPaint (hWnd, &ps);
			WireframeFunction (WIREFRAME_WM_PAINT, hWnd, 0, 0);
			EndPaint (hWnd, &ps);
  	  	  	return 0;
        }

  	  	case WM_CLOSE:
        {
  	  	  	DestroyWindow(hWnd);
  	  	  	return 0;
        }

  	  	case WM_DESTROY:
        {
			WireframeFunction (WIREFRAME_WM_DESTROY, hWnd, 0, 0);
  	  	  	PostQuitMessage (0);
  	  	  	return 0;
  	  	}
	
	}	// switch(iMsg)
  	return DefWindowProc (hWnd, iMsg, wParam, lParam);
}

/**********************************************************
 * 
 * RefreshScreen
 * 
 * parameters IN:
 * 	HWND hWnd
 * 
 *********************************************************/
void RefreshScreen (HWND hWnd) {
    InvalidateRect (hWnd, NULL, TRUE);
}

/**********************************************************
 * 
 * DrawVertexNormalsCtrl
 * 
 * parameters IN:
 * 	HWND hWnd
 * 
 *********************************************************/
void DrawVertexNormalsCtrl (HWND hWnd) {
    UINT uCheck = MF_UNCHECKED;
	if (ptrScene->drawVertexNormals) uCheck = MF_CHECKED;
	CheckMenuItem(GetMenu(hWnd), ID_OPTIONS_DRAWVERTEXNORMALS, uCheck);
}

/**********************************************************
 * 
 * TestSub
 * static
 * 
 *********************************************************/
static void TestSub (void) {
	cout << "TestSub" << endl;
       
	/**********************************
	MyMatrix m;
    cout << "cout << MyMatrix" << endl;
	cout << m << endl;
	cout << endl;

	m.Identity();
    cout << "m.Identity()" << endl;
	cout << m << endl;
	cout << endl;
	
    cout << "GetValue (0,0)" << endl;
	cout << m.GetValue (0,0) << endl;
	cout << endl;
	
    cout << "m[0][0]" << endl;
	cout << m[0][0] << endl;
	cout << endl;
	
    cout << "SetValue (0, 0, 2.34)" << endl;
	m.SetValue (0, 0, 2.34);
	cout << m.GetValue (0,0) << endl;
	cout << endl;

    cout << "m[0][0]=1.23)" << endl;
	m[0][0] = 1.23;
	cout << m[0][0] << endl;
	cout << endl;

	m.Zero();
    cout << "m.Zero()" << endl;
	cout << m << endl;
	cout << endl;

    m.Identity();
    MyMatrix m2(m);
    cout << "constructeur de recopie: MyMatrix m2(m.Identity())" << endl;
	cout << m2 << endl;
	cout << endl;
    
    m2.Identity();
	m2[0][0] = 2.;
	m2[1][1] = 2.;
	m2[2][2] = 2.;
	m2[3][3] = 2.;

    MyMatrix m3;
    m3.Identity();
	m3[0][0] = 2.;
	m3[1][1] = 2.;
	m3[2][2] = 2.;
	m3[3][3] = 2.;

    cout << "m = m2 * m3" << endl;
    m = m2 * m3;
	cout << m << endl;
	cout << endl;
	**********************************/

	/**********************************
    DemoXML();
	**********************************/

	//**********************************
	MapRec map;
    cout << "MapRec= " << map.GetXMid() << " , " << map.GetYMid() << endl;

	map = MapRec (10, 20);
    cout << "MapRec(10,20)= " << map.GetXMid() << " , " << map.GetYMid() << endl;
	cout << endl;

	ScreenRec screenPos;
    cout << "ScreenRec= " << screenPos.GetX() << " , " << screenPos.GetY() << " , " << screenPos.GetZ() << endl;

	screenPos = ScreenRec (123, 234, 345);
    cout << "ScreenRec(123, 234, 345)= " << screenPos.GetX() << " , " << screenPos.GetY() << " , " << screenPos.GetZ() << endl;
	cout << endl;

	ZParamRec zParams;
    cout << "ZParamRec= " << zParams.GetZMin() << " , " << zParams.GetZMax() << " , " << zParams.GetZRange() << endl;

	zParams = ZParamRec (1.2, 2.3, 3.4);
    cout << "ZParamRec(1.2, 2.3, 3.4)= " << zParams.GetZMin() << " , " << zParams.GetZMax() << " , " << zParams.GetZRange() << endl;
	cout << endl;
	//**********************************/
}

