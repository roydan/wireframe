#include <windows.h>
#include <cstdio>

#include "wireframe_app.h"
#include "resource.h"
#include "ObjectScene.h"
#include "wireframe.h"
#include "popfile.h"

extern ObjectScene * pObjectScene;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   PSTR szCmdLine, int iCmdShow)
{
	HWND     hwnd;
	MSG      msg;
	WNDCLASS wndclass;
    char     szAppName[] = "WireFrame";
    char     szName[] = "WireFrame";

	if (!hPrevInstance) {
		wndclass.style         = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc   = WndProc;
		wndclass.cbClsExtra    = 0;
		wndclass.cbWndExtra    = 0;
		wndclass.hInstance     = hInstance;
		wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName  = szName;
		wndclass.lpszClassName = szName;

		RegisterClass(&wndclass);
	}

	hwnd = CreateWindow(szName, szAppName,
		                WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        CW_USEDEFAULT, CW_USEDEFAULT,
		                NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
  	switch (iMsg) {
  	  	case WM_CREATE:
			WireframeFunction(WIREFRAME_WM_CREATE, hWnd, 0, lParam);
            DrawVertexNormalsCtrl(hWnd);
			return 0;

  	  	case WM_COMMAND:
  	  	  	switch (LOWORD(wParam)) {
				case IDM_NEW:
				    WireframeFunction(WIREFRAME_WM_DESTROY, 0, 0, 0);
				    WireframeFunction(WIREFRAME_WM_CREATE, hWnd, 0, 0);

					RefreshScreen(hWnd);
					return 0;

  	  	  	  	case IDM_OPEN:
  	  	  	    	// Load an object
					if (!WireframeFunction(WIREFRAME_LOAD_OBJECT, hWnd, 0, 0))
				    	RefreshScreen(hWnd);

					return 0;

				case ID_WIREFRAME_WIREFRAMESCENE:
  	  	  	    	// WireFrame Scene
				    RefreshScreen(hWnd);
				    return 0;

  	  	  	  	case IDM_RENDER_SCENE:
					if (!WireframeFunction(WIREFRAME_RENDER_SCENE, hWnd, 0, 0))
				    	RefreshScreen(hWnd);
					return 0;

				case IDM_TRANSFORMATION:
  	  	  	    	// Transform an object
					WireframeFunction(WIREFRAME_TRANSFORM_OBJECT, hWnd, 0, 0);
				    return 0;

  	  	  	  	case IDM_VIEW_POINT:
  	  	  	    	// Move view point
					WireframeFunction(WIREFRAME_VIEW_POINT, hWnd, 0, 0);
					return 0;

  	  	  	  	case ID_OPTIONS_DRAWVERTEXNORMALS:
  	  	  	    	pObjectScene->drawVertexNormals = !(pObjectScene->drawVertexNormals);
                    DrawVertexNormalsCtrl(hWnd);

				    RefreshScreen(hWnd);
					return 0;

				case IDM_HELP:
				    // Messages from Help menu
  	  	  	    	MessageBox(NULL, "La fonction Aide n'est pas encore disponible!", "Aide", MB_OK);
				    return 0;

  	  	  	  	case IDM_ABOUT:
					WireframeFunction(WIREFRAME_ABOUT, hWnd, 0, 0);
				    return 0;

  	  	  	  	case IDM_EXIT:
  	  	  	    	DestroyWindow(hWnd);
				    return 0 ;
			}

  	  	case WM_PAINT:
        {
  	        PAINTSTRUCT  ps;

  	  	  	HDC hDC = BeginPaint(hWnd, &ps);
			WireframeFunction(WIREFRAME_WM_PAINT, hWnd, 0, 0);
			EndPaint(hWnd, &ps);
  	  	  	return 0;
        }

  	  	case WM_CLOSE:
  	  	  	DestroyWindow(hWnd);
  	  	  	return 0;

  	  	case WM_DESTROY:
			WireframeFunction(WIREFRAME_WM_DESTROY, hWnd, 0, 0);
  	  	  	PostQuitMessage(0);
  	  	  	return 0;
  	  	}

  	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

void RefreshScreen(HWND hWnd) {
    InvalidateRect(hWnd, NULL, TRUE);
}

void DrawVertexNormalsCtrl(HWND hWnd) {
    UINT uCheck = MF_UNCHECKED;
	if (pObjectScene->drawVertexNormals) uCheck = MF_CHECKED;
	CheckMenuItem(GetMenu(hWnd), ID_OPTIONS_DRAWVERTEXNORMALS, uCheck);
}
