//*******************************************************************
//
// Program to draw wireframe projection of 3-D objects in 2-D screen space
//
// Implementation of linear transformation
// Implementation of 3-D geometry
// Implementation of viewing system I
//
// data structure:
//   object - surface - polygon
//   vertex details stored once for polygons
//   a shared vertex is stored for each surface
//   polygon normals are stored for culling
//   vertex normals are stored for shading
//   vertices are stored in a defined order:
//     counterclockwise with respect to viewing from the outside of the object
//
//*******************************************************************

#include <windows.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "wireframe.h"
#include "resource.h"
#include "popfile.h"
#include "renderer.h"
#include "ObjectScene.h"
#include "graphics3d.h"
#include "Vector.h"
#include "wireframe_app.h"

const char FILTER_OBJECT[] = "Object Files (*.OB)\0*.ob\0" \
	                         "All Files (*.*)\0*.*\0\0";

const char EXT_OBJECT[] = "ob";

const char FILTER_SCENE[] = "Scene Files (*.SCN)\0*.scn\0" \
	                         "All Files (*.*)\0*.*\0\0";

const char EXT_SCENE[] = "scn";

ObjectScene * pObjectScene = NULL;

FileStruct fileObject;
FileStruct fileScene;
 
int WireframeFunction(int iMsg, HWND hWnd, WPARAM wParam, LPARAM lParam) {
  	static HINSTANCE hInst;

  	switch (iMsg) {
  	  	case WIREFRAME_WM_CREATE:
  	  	  	pObjectScene = new ObjectScene(hWnd);
  	  	  	if (lParam) hInst = ((LPCREATESTRUCT) lParam)->hInstance;

            memcpy(fileObject.lpstrFilter, FILTER_OBJECT, 46);
            strcpy(fileObject.lpstrDefExt, EXT_OBJECT);
            fileObject.PopFileInit(hWnd);

            memcpy(fileScene.lpstrFilter, FILTER_SCENE, 48);
            strcpy(fileScene.lpstrDefExt, EXT_SCENE);
            fileScene.PopFileInit(hWnd);
		    break;

  	  	case WIREFRAME_LOAD_OBJECT:
        {
  	  		// Load an object
  	  	  	char fileName[MAX_STRING_LENGTH];
  	  	  	char objectName[MAX_STRING_LENGTH];

			strcpy(fileName, "..\\objects\\object.ob");
			strcpy(objectName, "");

			if (fileObject.PopFileOpen(hWnd, fileName, objectName)) {
  	  	  		if (pObjectScene->LoadObject(fileName, objectName) == false) {
  	  	    		pObjectScene->sceneChanged = true;
				} else {
					return 1;
				}
			}
        }	
  	    break;

  	  	case WIREFRAME_WM_PAINT:
  	  	  	// Wireframe scene
			if (pObjectScene->noOfObjects > 0) {
  	  	    	if (pObjectScene->sceneChanged) {
  	  	      		pObjectScene->TransformScene();
				}
  	  	    	pObjectScene->WireFrameScene(hWnd);
			}
  	  	    break;

  	  	case WIREFRAME_RENDER_SCENE:
        {
            //*******************************************************
  	  		// Load a scene
  	  	  	char fileName[MAX_STRING_LENGTH];
  	  	  	char sceneName[MAX_STRING_LENGTH];

			strcpy(fileName, "..\\objects\\object.scn");
			strcpy(sceneName, "");

			if (fileScene.PopFileOpen(hWnd, fileName, sceneName)) {
  	  	  		if (pObjectScene->LoadScene(hWnd, fileName, sceneName) == false) {
  	  	    		pObjectScene->sceneChanged = true;
				} else {
					return 1;
				}
			}
			return 0;
            //*******************************************************
             
			// Render Scene
			if (pObjectScene->noOfObjects > 0) {
				RenderScene(hWnd, pObjectScene->objectHead);
			} else {
  	  	    	MessageBox(NULL, "No objects currently loaded", NULL, MB_OK);
			}
        }
        break;

  	  	case WIREFRAME_TRANSFORM_OBJECT:
  	  	  	// Transform an object
			if (pObjectScene->noOfObjects > 0) {
  	  	    	DialogBoxParam(hInst, "TRANSFORMATION_DLG", hWnd, TransformationDlgProc, 0L);
			} else {
  	  	    	MessageBox(NULL, "No objects currently loaded", NULL, MB_OK);
			}
  	  	  	break;

  	  	case WIREFRAME_VIEW_POINT:
  	  	  	// Move view point
			if (pObjectScene->noOfObjects > 0) {
  	  	    	DialogBoxParam(hInst, "POINT_VUE_DLG", hWnd, ViewRefPointDlgProc, 0L);
			} else {
  	  	    	MessageBox(NULL, "No objects currently loaded", NULL, MB_OK);
			}
  	  	  	break;

  	  	case WIREFRAME_WM_DESTROY:
  	  	 	// store settings
  	  	 	WritePrivateProfileInt("VIEWING_INFORMATION", "ViewPlaneDistance", pObjectScene->viewRefPoint.viewPlaneDist, DEFAULT_INI.c_str());
  	  	 	WritePrivateProfileInt("VIEWING_INFORMATION", "Rho", pObjectScene->viewRefPoint.rho, DEFAULT_INI.c_str());
  	  	 	WritePrivateProfileInt("VIEWING_INFORMATION", "Theta", pObjectScene->viewRefPoint.theta, DEFAULT_INI.c_str());
  	  	 	WritePrivateProfileInt("VIEWING_INFORMATION", "Phi", pObjectScene->viewRefPoint.phi, DEFAULT_INI.c_str());

  	  	 	WritePrivateProfileInt("VIEWING_INFORMATION", "DrawVertexNormals", pObjectScene->drawVertexNormals, DEFAULT_INI.c_str());
  	  		break;

  	  	case WIREFRAME_ABOUT:
  	  	  	DialogBox(hInst, "ABOUT_BOX", hWnd, AboutBoxDlgProc);
  	  	    break;
  	}
	return 0;
}

BOOL CALLBACK TransformationDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  	static ObjectCell * currentObject;
  	static Vector rv(0,0,0);
  	static Vector sv(1,1,1);
  	static Vector tv(0,0,0);
  	static LONG index = -1;
  	static HWND hwndParent = NULL;

	switch (uMsg) {
  	  	case WM_INITDIALOG:
		{
		  	hwndParent = GetParent(hwndDlg);

		  	// select object
		  	currentObject = pObjectScene->objectHead;
  	    	while (currentObject != NULL) {
		    	index = SendDlgItemMessage(hwndDlg, IDC_CB_OBJECT, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)currentObject->name.c_str());
				currentObject = currentObject->next;
		  	}

		  	index = 0;
		  	SendDlgItemMessage(hwndDlg, IDC_CB_OBJECT, CB_SETCURSEL, (WPARAM)index, 0);

		  	currentObject = pObjectScene->objectHead;
		  	for (LONG i = 0; i < index; i++) {
				currentObject = currentObject->next;
			}

  	    	InitTransformationDlg(hwndDlg, &rv, &sv, &tv);
		  	return FALSE;
		}

  	  	case WM_COMMAND:
  	  	  	switch (LOWORD(wParam)) {
  	  	  		case IDC_CB_OBJECT:
					switch (HIWORD(wParam)) {
					  	case CBN_SELCHANGE:
						{
				        	index = SendDlgItemMessage(hwndDlg, IDC_CB_OBJECT, CB_GETCURSEL, 0, 0);
				        	currentObject = pObjectScene->objectHead;
				        	for (LONG i = 0; i < index; i++) {
					    	  	currentObject = currentObject->next;
							}
				        	return TRUE;
						}
					}
					return FALSE;

				case DL_ROTATION_X:
                    rv.SetX( GetDlgItemDouble(hwndDlg, DL_ROTATION_X));
				    return TRUE;

				case DL_ROTATION_Y:
                    rv.SetY( GetDlgItemDouble(hwndDlg, DL_ROTATION_Y));
				    return TRUE;

				case DL_ROTATION_Z:
                    rv.SetZ( GetDlgItemDouble(hwndDlg, DL_ROTATION_Z));
				    return TRUE;

				case DL_SCALING_X:
                    sv.SetX( GetDlgItemDouble(hwndDlg, DL_SCALING_X));
				    return TRUE;

				case DL_SCALING_Y:
                    sv.SetY( GetDlgItemDouble(hwndDlg, DL_SCALING_Y));
				    return TRUE;

				case DL_SCALING_Z:
                    sv.SetZ( GetDlgItemDouble(hwndDlg, DL_SCALING_Z));
				    return TRUE;

				case DL_TRANSLATION_X:
                    tv.SetX( GetDlgItemDouble(hwndDlg, DL_TRANSLATION_X));
				    return TRUE;

				case DL_TRANSLATION_Y:
                    tv.SetY( GetDlgItemDouble(hwndDlg, DL_TRANSLATION_Y));
				    return TRUE;

				case DL_TRANSLATION_Z:
                    tv.SetZ( GetDlgItemDouble(hwndDlg, DL_TRANSLATION_Z));
				    return TRUE;

  	  	  		case IDC_EXEC_ROTATION:
				    currentObject->GetTransformation(ROTATION, rv);
  	  	  	  		pObjectScene->sceneChanged = true;
				    RefreshScreen(hwndParent);
					return TRUE;

  	  	  		case IDC_EXEC_SCALING:
					currentObject->GetTransformation(SCALING, sv);
  	  	  	  		pObjectScene->sceneChanged = true;
				    RefreshScreen(hwndParent);
					return TRUE;

  	  	  		case IDC_EXEC_TRANSLATION:
					currentObject->GetTransformation(TRANSLATION, tv);
  	  	  	  		pObjectScene->sceneChanged = true;
				    RefreshScreen(hwndParent);
					return TRUE;

  	  	  		case IDC_TRANSFORMATION_RESET:
  	  	  	  		InitTransformationDlg(hwndDlg, &rv, &sv, &tv);
					currentObject->GetTransformation(0, tv);
  	  	  	  		pObjectScene->sceneChanged = true;
				    RefreshScreen(hwndParent);
					return TRUE;

				case IDOK:
					EndDialog(hwndDlg, 0);
				    return TRUE;

				case IDCANCEL:
					EndDialog(hwndDlg, 1);
				    return TRUE;
			}
  	}
  	return FALSE;
}

void InitTransformationDlg(HWND hwndDlg, Vector * rv, Vector * sv, Vector * tv)
{
  	*rv = Vector(0., 0., 0.);
  	*sv = Vector(1., 1., 1.);
  	*tv = Vector(0., 0., 0.);

    SetDlgItemDouble(hwndDlg, DL_ROTATION_X, rv->GetX());
    SetDlgItemDouble(hwndDlg, DL_ROTATION_Y, rv->GetY());
    SetDlgItemDouble(hwndDlg, DL_ROTATION_Z, rv->GetZ());

    SetDlgItemDouble(hwndDlg, DL_SCALING_X, sv->GetX());
    SetDlgItemDouble(hwndDlg, DL_SCALING_Y, sv->GetY());
    SetDlgItemDouble(hwndDlg, DL_SCALING_Z, sv->GetZ());

    SetDlgItemDouble(hwndDlg, DL_TRANSLATION_X, tv->GetX());
    SetDlgItemDouble(hwndDlg, DL_TRANSLATION_Y, tv->GetY());
    SetDlgItemDouble(hwndDlg, DL_TRANSLATION_Z, tv->GetZ());
}

BOOL CALLBACK ViewRefPointDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  	static HWND hwndParent = NULL;
  	HWND hwndCtrl;

  	switch(uMsg) {
  	  	case WM_INITDIALOG:
		  	hwndParent = GetParent(hwndDlg);
  	      	InitViewRefPointDlg(hwndDlg, &pObjectScene->viewRefPoint);
		  	return FALSE;

  	  	case WM_HSCROLL:
  	  	  	switch(LOWORD(wParam)) {
				case SB_PAGELEFT:
					switch(GetDlgCtrlID((HWND)lParam)) {
						case IDC_SCROLLBAR_RHO:
  	  	  	    			pObjectScene->viewRefPoint.rho -= 100;
					  		break;
						case IDC_SCROLLBAR_THETA:
  	  	  	    			pObjectScene->viewRefPoint.theta -= 10;
					  		break;
						case IDC_SCROLLBAR_PHI:
  	  	  	    			pObjectScene->viewRefPoint.phi -= 10;
					  		break;
						case IDC_SCROLLBAR_VIEWDIST:
  	  	  	    			pObjectScene->viewRefPoint.viewPlaneDist -= 100;
					  		break;
					}
					break;

				case SB_LINELEFT:
					switch(GetDlgCtrlID((HWND)lParam)) {
						case IDC_SCROLLBAR_RHO:
  	  	  	    			pObjectScene->viewRefPoint.rho -= 10;
					  		break;
						case IDC_SCROLLBAR_THETA:
  	  	  	    			pObjectScene->viewRefPoint.theta -= 1;
					  		break;
						case IDC_SCROLLBAR_PHI:
  	  	  	    			pObjectScene->viewRefPoint.phi -= 1;
					  		break;
						case IDC_SCROLLBAR_VIEWDIST:
  	  	  	    			pObjectScene->viewRefPoint.viewPlaneDist -= 10;
					  		break;
					}
					break;
				  
				case SB_LINERIGHT:
					switch(GetDlgCtrlID((HWND)lParam)) {
						case IDC_SCROLLBAR_RHO:
  	  	  	    			pObjectScene->viewRefPoint.rho += 10;
					  		break;
						case IDC_SCROLLBAR_THETA:
  	  	  	    			pObjectScene->viewRefPoint.theta += 1;
					  		break;
						case IDC_SCROLLBAR_PHI:
  	  	  	    			pObjectScene->viewRefPoint.phi += 1;
					  		break;
						case IDC_SCROLLBAR_VIEWDIST:
  	  	  	    			pObjectScene->viewRefPoint.viewPlaneDist += 10;
					  		break;
					}
					break;
				  
				case SB_PAGERIGHT:
					switch(GetDlgCtrlID((HWND)lParam)) {
						case IDC_SCROLLBAR_RHO:
  	  	  	    			pObjectScene->viewRefPoint.rho += 100;
					  		break;
						case IDC_SCROLLBAR_THETA:
  	  	  	    			pObjectScene->viewRefPoint.theta += 10;
					  		break;
						case IDC_SCROLLBAR_PHI:
  	  	  	    			pObjectScene->viewRefPoint.phi += 10;
					  		break;
						case IDC_SCROLLBAR_VIEWDIST:
  	  	  	    			pObjectScene->viewRefPoint.viewPlaneDist += 100;
					  		break;
					}
					break;
  	  	  
				case SB_THUMBPOSITION:
				case SB_THUMBTRACK:
                {
  	  	  	  		int value = (int)HIWORD(wParam);
					if (value > 0x7fff) value = - (0xffff - value - 1);
					
					switch(GetDlgCtrlID((HWND)lParam)) {
						case IDC_SCROLLBAR_RHO:
  	  	  	    			pObjectScene->viewRefPoint.rho = value;
					  		break;
						case IDC_SCROLLBAR_THETA:
  	  	  	    			pObjectScene->viewRefPoint.theta = value;
					  		break;
						case IDC_SCROLLBAR_PHI:
  	  	  	    			pObjectScene->viewRefPoint.phi = value;
					  		break;
						case IDC_SCROLLBAR_VIEWDIST:
  	  	  	    			pObjectScene->viewRefPoint.viewPlaneDist = value;
					  		break;
					}
                }
				break;

				default:
				    return FALSE;
			}

  	    	SetDlgItemInt(hwndDlg, IDC_VP_VIEW_DISTANCE, pObjectScene->viewRefPoint.viewPlaneDist, true);
  	    	SetDlgItemInt(hwndDlg, IDC_VP_RHO, pObjectScene->viewRefPoint.rho, true);
  	    	SetDlgItemInt(hwndDlg, IDC_VP_THETA, pObjectScene->viewRefPoint.theta, true);
  	    	SetDlgItemInt(hwndDlg, IDC_VP_PHI, pObjectScene->viewRefPoint.phi, true);

		  	SendMessage(hwndDlg, WM_COMMAND, IDC_EXECUTE, 0L);
		  	return TRUE;

  	  	case WM_COMMAND:
            BOOL translated;
            
  	  	  	switch(LOWORD(wParam)) {
			  	case IDC_VP_VIEW_DISTANCE:
			    	pObjectScene->viewRefPoint.viewPlaneDist = GetDlgItemInt(hwndDlg, IDC_VP_VIEW_DISTANCE, &translated, true);

  	  	    		hwndCtrl = GetDlgItem(hwndDlg, IDC_SCROLLBAR_VIEWDIST);
  	  	    		SetScrollPos(hwndCtrl, SB_CTL, pObjectScene->viewRefPoint.viewPlaneDist, TRUE);
			    	return TRUE;

			  	case IDC_VP_RHO:
			    	pObjectScene->viewRefPoint.rho = GetDlgItemInt(hwndDlg, IDC_VP_RHO, &translated, true);

  	  	    		hwndCtrl = GetDlgItem(hwndDlg, IDC_SCROLLBAR_RHO);
  	  	    		SetScrollPos(hwndCtrl, SB_CTL, pObjectScene->viewRefPoint.rho, TRUE);
			    	return TRUE;

			  	case IDC_VP_THETA:
			    	pObjectScene->viewRefPoint.theta = GetDlgItemInt(hwndDlg, IDC_VP_THETA, &translated, true);

  	  	    		hwndCtrl = GetDlgItem(hwndDlg, IDC_SCROLLBAR_THETA);
  	  	    		SetScrollPos(hwndCtrl, SB_CTL, pObjectScene->viewRefPoint.theta, TRUE);
					return TRUE;

			  	case IDC_VP_PHI:
			    	pObjectScene->viewRefPoint.phi = GetDlgItemInt(hwndDlg, IDC_VP_PHI, &translated, true);

  	  	    		hwndCtrl = GetDlgItem(hwndDlg, IDC_SCROLLBAR_PHI);
  	  	    		SetScrollPos(hwndCtrl, SB_CTL, pObjectScene->viewRefPoint.phi, TRUE);
					return TRUE;

  	  	  		case IDC_EXECUTE:
  	  	    		pObjectScene->SetViewVariables();
  	  	    		pObjectScene->sceneChanged = true;
				    RefreshScreen(hwndParent);
					return TRUE;

  	  	  		case IDC_PV_RESET:
  	  	    		pObjectScene->viewRefPoint = ViewPointRec(1000, 5000, 45, 75);

  	  	    		InitViewRefPointDlg(hwndDlg, &pObjectScene->viewRefPoint);

  	  	    		pObjectScene->SetViewVariables();
  	  	    		pObjectScene->sceneChanged = true;
				    RefreshScreen(hwndParent);
					return TRUE;

			  	case IDOK:
  	  	    		pObjectScene->SetViewVariables();
  	  	    		pObjectScene->sceneChanged = true;
				    RefreshScreen(hwndParent);

					EndDialog(hwndDlg, 0);
			    	return TRUE;

			  	case IDCANCEL:
					EndDialog(hwndDlg, 1);
			    	return TRUE;
			}
			break;
  	}
  	return FALSE;
}

void InitViewRefPointDlg(HWND hwndDlg, ViewPointRec * viewRefPoint)
{
  	HWND hwndCtrl;

  	SetDlgItemInt(hwndDlg, IDC_VP_VIEW_DISTANCE, viewRefPoint->viewPlaneDist, true);

  	hwndCtrl = GetDlgItem(hwndDlg, IDC_SCROLLBAR_VIEWDIST);
  	SetScrollRange(hwndCtrl, SB_CTL, 1, 10000, TRUE);
  	SetScrollPos(hwndCtrl, SB_CTL, viewRefPoint->viewPlaneDist, TRUE);

  	SetDlgItemInt(hwndDlg, IDC_VP_RHO, viewRefPoint->rho, true);

  	hwndCtrl = GetDlgItem(hwndDlg, IDC_SCROLLBAR_RHO);
  	SetScrollRange(hwndCtrl, SB_CTL, 1, 10000, TRUE);
  	SetScrollPos(hwndCtrl, SB_CTL, viewRefPoint->rho, TRUE);

  	SetDlgItemInt(hwndDlg, IDC_VP_THETA, viewRefPoint->theta, true);

  	hwndCtrl = GetDlgItem(hwndDlg, IDC_SCROLLBAR_THETA);
  	SetScrollRange(hwndCtrl, SB_CTL, -180, +180, TRUE);
  	SetScrollPos(hwndCtrl, SB_CTL, viewRefPoint->theta, TRUE);

  	SetDlgItemInt(hwndDlg, IDC_VP_PHI, viewRefPoint->phi, true);

  	hwndCtrl = GetDlgItem(hwndDlg, IDC_SCROLLBAR_PHI);
  	SetScrollRange(hwndCtrl, SB_CTL, -180, +180, TRUE);
  	SetScrollPos(hwndCtrl, SB_CTL, viewRefPoint->phi, TRUE);
}

BOOL CALLBACK AboutBoxDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  	switch(uMsg) {
  	  	case WM_COMMAND:
  	  	  	switch(LOWORD(wParam)) {
				case IDOK:
					EndDialog(hwndDlg, 0);
				  	return TRUE;

				case IDCANCEL:
					EndDialog(hwndDlg, 1);
				  	return TRUE;
			}
  	  		break;
  	}
  	return FALSE;
}
