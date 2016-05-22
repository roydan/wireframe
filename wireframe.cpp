/**********************************************************
 *
 * Program to draw wireframe projection of 3-D objects in 2-D screen space
 *
 * Implementation of linear transformation
 * Implementation of 3-D geometry
 * Implementation of viewing system I
 *
 * data structure:
 *   hierarchical object - surface - polygon scheme
 *
 *   vertex details stored once for polygons
 *   a shared vertex between 2 surfaces is stored for each surface
 *
 *   polygon normals are stored for culling
 *   vertex normals are stored for shading
 *
 *   vertices are stored in a defined order:
 *     counterclockwise with respect to viewing from the outside of the object
 *
 **********************************************************
 *
 * CHANGED:
 *		TO											FROM
 *
 * class ObjectScene
 * 		vector<ObjectCell *> objectHead;		ObjectCell * objectHead;
 *
 *      objectHead.size()		                noOfObjects
 *
 * class ObjectCell
 * 		vector<SurfaceCell *> surfaceHead;		SurfaceCell * surfaceHead;
 * 		vector<VertexCell *>  vertexHead;		VertexCell *  vertexHead;
 * 		vector<VertexCell *>  vertexAt;			VertexCell *  vertexAt[MAX_NO_OF_VERTICES];
 *
 * class SurfaceCell
 *   	vector<PolygonCell *> polygonHead;		PolygonCell * polygonHead;
 *
 *
 *
 * REMOVED:
 *
 * class ObjectCell
 *		ObjectCell * next;
 *
 * class SurfaceCell
 *		SurfaceCell * next;
 *
 *
 *
 *
 *
 **********************************************************
 * 
 * added points FROM vertexAt[] TO vertexHead
 * 
 *
 *
 *
 *
 **********************************************************
 * 
 * 	guicon.cpp / .h
 * 	Popfile.cpp / .h
 * 	ScreenRec.cpp / .h
 * 
 * 	MapRec.cpp / .h
 * 
 **********************************************************
 *
 * TO DO:
 *
 * class PolygonList
 *   	PolygonList is in reality a vector of <PolygonCell *> object : vector<PolygonCell *> poly;
 *   	remove: PolygonList * next;
 *
 * class PolygonCell
 *   	VertexList * vertexListHead;  ->  vector<VertexList *> vertexListHead;
 *   	remove: PolygonCell * next;
 *
 * class VertexList
 *   	VertexList is in reality a vector of <VertexCell *> object : vector<VertexCell *> vertex;
 *   	remove: VertexList * next;
 *
 * class VertexCell
 *   	PolygonList is in reality a vector of <PolygonCell *> object : vector<PolygonCell *> polyListHead;
 *   	remove: VertexCell * next;
 *
 *
 *
 *
 *
 *********************************************************/

#include <windows.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "wireframe.h"
#include "resource.h"
#include "popfile.h"
#include "renderer.h"
#include "ObjectScene.h"
#include "graphics3d.h"
#include "Vector.h"
#include "wireframe_app.h"

const char FILTER_OBJECT[] = "Object Files (*.OBJ)\0*.obj\0" \
	                         "All Files (*.*)\0*.*\0\0";

const char EXT_OBJECT[] = "obj";

const char FILTER_SCENE[] = "Scene Files (*.SCN)\0*.scn\0" \
	                         "All Files (*.*)\0*.*\0\0";

const char EXT_SCENE[] = "scn";

ObjectScene * ptrScene = NULL;

FileStruct fileObject;
FileStruct fileScene;
 
/**********************************************************
 * WireframeFunction
 * 
 * WireframeFunction
 * 
 * parameters IN:
 * 	int iMsg
 *		WIREFRAME_WM_CREATE        1
 *		WIREFRAME_LOAD_OBJECT      2
 *		WIREFRAME_LOAD_SCENE       9
 *		WIREFRAME_WM_PAINT         3
 *		WIREFRAME_RENDER_SCENE     4
 *		WIREFRAME_TRANSFORM_OBJECT 5
 *		WIREFRAME_VIEW_POINT       6
 *		WIREFRAME_WM_DESTROY       7
 *		WIREFRAME_ABOUT            8
 *	HWND hWnd
 *	WPARAM wParam
 *	LPARAM lParam
 * 
 * return value : int
 *********************************************************/
int WireframeFunction (int iMsg, HWND hWnd, WPARAM wParam, LPARAM lParam) {
  	static HINSTANCE hInst;

  	switch (iMsg) {
  	  	case WIREFRAME_WM_CREATE:
		{
			// instanciate new scene
  	  	  	ptrScene = new ObjectScene (hWnd);
  	  	  	if (lParam) hInst = ((LPCREATESTRUCT) lParam)->hInstance;

            memcpy(fileObject.lpstrFilter, FILTER_OBJECT, 46);
            strcpy(fileObject.lpstrDefExt, EXT_OBJECT);
            fileObject.PopFileInit (hWnd);

            memcpy(fileScene.lpstrFilter, FILTER_SCENE, 46);
            strcpy(fileScene.lpstrDefExt, EXT_SCENE);
            fileScene.PopFileInit (hWnd);
		}
		break;

  	  	case WIREFRAME_LOAD_OBJECT:
        {
  	  		// Load an object into the current scene
  	  	  	char fileName[MAX_STRING_LENGTH];
  	  	  	char objectName[MAX_STRING_LENGTH];

			strcpy(fileName, "");
			strcpy(objectName, "");

			if (fileObject.PopFileOpen (hWnd, fileName, objectName)) {
               	//?? cout << "fileName=   " << fileName << endl;
               	//?? cout << "objectName= " << objectName << endl;
               	if (strstr(objectName, ".obj") != NULL) {
                	// load an object
  	  	  			if (ptrScene->LoadObjectExt (fileName, objectName) == false) {
  	  	    			ptrScene->sceneChanged = true;
					} else {
						return 1;
					}
               	} else if (strstr(objectName, ".scn") != NULL) {
  	  		   	    // Load a scene
  	  	  			if (ptrScene->LoadScene (hWnd, fileName, objectName) == false) {
  	  	    		  	ptrScene->sceneChanged = true;
					} else {
					 	return 1;
					}
               	}
			}
        }	
  	    break;

  	  	case WIREFRAME_LOAD_SCENE:
        {
  	  		// Load an object into the current scene
  	  	  	char fileName[MAX_STRING_LENGTH];
  	  	  	char objectName[MAX_STRING_LENGTH];

			strcpy(fileName, "");
			strcpy(objectName, "");

			if (fileScene.PopFileOpen (hWnd, fileName, objectName)) {
               	//?? cout << "fileName=   " << fileName << endl;
               	//?? cout << "objectName= " << objectName << endl;
               	if (strstr(objectName, ".scn") != NULL) {
  	  		   		// Load a scene
  	  		   	    if (ptrScene->LoadScene (hWnd, fileName, objectName) == false) {
  	  	    			ptrScene->sceneChanged = true;
					} else {
						return 1;
					}
               	} else if (strstr(objectName, ".obj") != NULL) {
               		// load an object
  	  	  			if (ptrScene->LoadObjectExt (fileName, objectName) == false) {
  	  	    			ptrScene->sceneChanged = true;
					} else {
						return 1;
					}
               	}
			}
        }	
  	    break;

  	  	case WIREFRAME_WM_PAINT:
		{
  	  	  	// Wireframe scene
			if (ptrScene->objectHead.size() > 0) {
  	  	    	if (ptrScene->sceneChanged) {
  	  	      		ptrScene->TransformScene();
				}
  	  	    	ptrScene->WireFrameScene (hWnd);
			}
		}
  	  	break;

  	  	case WIREFRAME_RENDER_SCENE:
        {
			// Render Scene
			if (ptrScene->objectHead.size() > 0) {
				RenderScene (hWnd, ptrScene->objectHead);
			} else {
  	  	    	MessageBox (NULL, "No objects currently loaded", NULL, MB_OK);
			}
        }
        break;

  	  	case WIREFRAME_TRANSFORM_OBJECT:
		{
  	  	  	// Transform an object
			if (ptrScene->objectHead.size() > 0) {
  	  	    	DialogBoxParam(hInst, "TRANSFORMATION_DLG", hWnd, TransformationDlgProc, 0L);
			} else {
  	  	    	MessageBox (NULL, "No objects currently loaded", NULL, MB_OK);
			}
		}
  	  	break;

  	  	case WIREFRAME_VIEW_POINT:
		{
  	  	  	// Move view point
			if (ptrScene->objectHead.size() > 0) {
  	  	    	DialogBoxParam(hInst, "POINT_VUE_DLG", hWnd, ViewRefPointDlgProc, 0L);
			} else {
  	  	    	MessageBox (NULL, "No objects currently loaded", NULL, MB_OK);
			}
		}
  	  	break;

  	  	case WIREFRAME_WM_DESTROY:
		{
  	  	 	// store settings
  	  	 	WritePrivateProfileInt ("VIEWING_INFORMATION", "ViewPlaneDistance", ptrScene->viewRefPoint.GetViewPlaneDist(), DEFAULT_INI.c_str());
  	  	 	WritePrivateProfileInt ("VIEWING_INFORMATION", "Rho", ptrScene->viewRefPoint.GetRho(), DEFAULT_INI.c_str());
  	  	 	WritePrivateProfileInt ("VIEWING_INFORMATION", "Theta", ptrScene->viewRefPoint.GetTheta(), DEFAULT_INI.c_str());
  	  	 	WritePrivateProfileInt ("VIEWING_INFORMATION", "Phi", ptrScene->viewRefPoint.GetPhi(), DEFAULT_INI.c_str());

  	  	 	WritePrivateProfileInt ("VIEWING_INFORMATION", "DrawVertexNormals", ptrScene->drawVertexNormals, DEFAULT_INI.c_str());
		}
  	  	break;

  	  	case WIREFRAME_ABOUT:
		{
  	  	  	DialogBox (hInst, "ABOUT_BOX", hWnd, AboutBoxDlgProc);
		}
  	  	break;
  	}
	return 0;
}

/**********************************************************
 * TransformationDlgProc
 * 
 * TransformationDlgProc
 * 
 * parameters IN:
 *	HWND hwndDlg
 * 	UINT uMsg
 *		WM_INITDIALOG
 * 
 * 	  	WM_COMMAND
 *	  		LOWORD(wParam)
 * 	  	  		IDC_CB_OBJECT
 *					HIWORD(wParam)
 *					  	CBN_SELCHANGE
 *				DL_ROTATION_X
 *				DL_ROTATION_Y
 *				DL_ROTATION_Z
 *				DL_SCALING_X
 *				DL_SCALING_Y
 *				DL_SCALING_Z
 *				DL_TRANSLATION_X
 *				DL_TRANSLATION_Y
 *				DL_TRANSLATION_Z
 *				IDC_EXEC_ROTATION
 *				IDC_EXEC_SCALING
 *				IDC_EXEC_TRANSLATION
 *				IDC_TRANSFORMATION_RESET
 *				IDOK
 *				IDCANCEL
 * 
 *	WPARAM wParam
 *	LPARAM lParam
 * 
 * return value : BOOL
 *********************************************************/
BOOL CALLBACK TransformationDlgProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  	static ObjectCell * currentObject;
  	static Vector rv (0,0,0);
  	static Vector sv (1,1,1);
  	static Vector tv (0,0,0);
  	static LONG index = -1;
  	static HWND hwndParent = NULL;

	switch (uMsg) {
  	  	case WM_INITDIALOG:
		{
		  	hwndParent = GetParent (hwndDlg);

		  	// select object
			for (vector<ObjectCell *>::iterator it = ptrScene->objectHead.begin(); it != ptrScene->objectHead.end(); ++it) {
				currentObject = *it;
		    	index = SendDlgItemMessage (hwndDlg, IDC_CB_OBJECT, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)currentObject->name.c_str());
		  	}

		  	index = 0;
		  	SendDlgItemMessage (hwndDlg, IDC_CB_OBJECT, CB_SETCURSEL, (WPARAM)index, 0);

		  	currentObject = ptrScene->objectHead[index];

  	    	InitTransformationDlg (hwndDlg, &rv, &sv, &tv);
		  	return FALSE;
		}

  	  	case WM_COMMAND:
		{
  	  	  	switch (LOWORD(wParam)) {
  	  	  		case IDC_CB_OBJECT:
				{
					switch (HIWORD(wParam)) {
					  	case CBN_SELCHANGE:
						{
				        	index = SendDlgItemMessage (hwndDlg, IDC_CB_OBJECT, CB_GETCURSEL, 0, 0);
				        	currentObject = ptrScene->objectHead[index];
				        	return TRUE;
						}
					}	// LOWORD(wParam)
					return FALSE;
				}

				case DL_ROTATION_X:
                    rv.SetX( GetDlgItemDouble (hwndDlg, DL_ROTATION_X));
				    return TRUE;

				case DL_ROTATION_Y:
                    rv.SetY( GetDlgItemDouble (hwndDlg, DL_ROTATION_Y));
				    return TRUE;

				case DL_ROTATION_Z:
                    rv.SetZ( GetDlgItemDouble (hwndDlg, DL_ROTATION_Z));
				    return TRUE;

				case DL_SCALING_X:
                    sv.SetX( GetDlgItemDouble (hwndDlg, DL_SCALING_X));
				    return TRUE;

				case DL_SCALING_Y:
                    sv.SetY( GetDlgItemDouble (hwndDlg, DL_SCALING_Y));
				    return TRUE;

				case DL_SCALING_Z:
                    sv.SetZ( GetDlgItemDouble (hwndDlg, DL_SCALING_Z));
				    return TRUE;

				case DL_TRANSLATION_X:
                    tv.SetX( GetDlgItemDouble (hwndDlg, DL_TRANSLATION_X));
				    return TRUE;

				case DL_TRANSLATION_Y:
                    tv.SetY( GetDlgItemDouble (hwndDlg, DL_TRANSLATION_Y));
				    return TRUE;

				case DL_TRANSLATION_Z:
                    tv.SetZ( GetDlgItemDouble (hwndDlg, DL_TRANSLATION_Z));
				    return TRUE;

  	  	  		case IDC_EXEC_ROTATION:
				{
				    currentObject->GetTransformation (ROTATION, rv);
  	  	  	  		ptrScene->sceneChanged = true;
				    RefreshScreen (hwndParent);
					return TRUE;
				}

  	  	  		case IDC_EXEC_SCALING:
				{
					currentObject->GetTransformation (SCALING, sv);
  	  	  	  		ptrScene->sceneChanged = true;
				    RefreshScreen (hwndParent);
					return TRUE;
				}

  	  	  		case IDC_EXEC_TRANSLATION:
				{
					currentObject->GetTransformation (TRANSLATION, tv);
  	  	  	  		ptrScene->sceneChanged = true;
				    RefreshScreen (hwndParent);
					return TRUE;
				}

  	  	  		case IDC_TRANSFORMATION_RESET:
				{
  	  	  	  		InitTransformationDlg(hwndDlg, &rv, &sv, &tv);
					currentObject->GetTransformation (0, tv);
  	  	  	  		ptrScene->sceneChanged = true;
				    RefreshScreen (hwndParent);
					return TRUE;
				}

				case IDOK:
					EndDialog(hwndDlg, 0);
				    return TRUE;

				case IDCANCEL:
					EndDialog(hwndDlg, 1);
				    return TRUE;

			}	// LOWORD(wParam)
		}	// WM_COMMAND
  	}	// switch (uMsg)

	return FALSE;
}

/**********************************************************
 * InitTransformationDlg
 * 
 * InitTransformationDlg
 * 
 * parameters IN:
 *	HWND hwndDlg
 * 	Vector * rv
 *	Vector * sv
 *	Vector * tv
 * 
 * return value : void
 *********************************************************/
void InitTransformationDlg(HWND hwndDlg, Vector * rv, Vector * sv, Vector * tv)
{
  	*rv = Vector(0., 0., 0.);
    SetDlgItemDouble (hwndDlg, DL_ROTATION_X, rv->GetX());
    SetDlgItemDouble (hwndDlg, DL_ROTATION_Y, rv->GetY());
    SetDlgItemDouble (hwndDlg, DL_ROTATION_Z, rv->GetZ());

  	*sv = Vector(1., 1., 1.);
    SetDlgItemDouble (hwndDlg, DL_SCALING_X, sv->GetX());
    SetDlgItemDouble (hwndDlg, DL_SCALING_Y, sv->GetY());
    SetDlgItemDouble (hwndDlg, DL_SCALING_Z, sv->GetZ());

  	*tv = Vector(0., 0., 0.);
    SetDlgItemDouble (hwndDlg, DL_TRANSLATION_X, tv->GetX());
    SetDlgItemDouble (hwndDlg, DL_TRANSLATION_Y, tv->GetY());
    SetDlgItemDouble (hwndDlg, DL_TRANSLATION_Z, tv->GetZ());
}

/**********************************************************
 * ViewRefPointDlgProc
 * 
 * ViewRefPointDlgProc
 * 
 * parameters IN:
 *	HWND hwndDlg
 * 	UINT uMsg
 * 	  	WM_INITDIALOG
 * 
 * 	  	WM_HSCROLL
 * 	  	  	LOWORD(wParam)
 *				SB_PAGELEFT
 *				SB_LINELEFT
 *				SB_LINERIGHT
 *				SB_PAGERIGHT
 *				SB_THUMBPOSITION
 *				SB_THUMBTRACK
 *					IDC_SCROLLBAR_RHO
 *					IDC_SCROLLBAR_THETA
 *					IDC_SCROLLBAR_PHI
 *					IDC_SCROLLBAR_VIEWDIST
 * 
 * 	  	case WM_COMMAND
 * 	  	  	LOWORD(wParam)
 *			  	IDC_VP_VIEW_DISTANCE
 *			  	IDC_VP_RHO
 *			  	IDC_VP_THETA
 *			  	IDC_VP_PHI
 *			  	IDC_EXECUTE
 *			  	IDC_PV_RESET
 *			  	IDOK
 *			  	IDCANCEL
 * 
 * 
 *	WPARAM wParam
 *	LPARAM lParam
 * 
 * return value : BOOL
 *********************************************************/
BOOL CALLBACK ViewRefPointDlgProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  	static HWND hwndParent = NULL;
  	HWND hwndCtrl;

  	switch(uMsg) {
  	  	case WM_INITDIALOG:
		  	hwndParent = GetParent (hwndDlg);
  	      	InitViewRefPointDlg(hwndDlg, &ptrScene->viewRefPoint);
		  	return FALSE;

  	  	case WM_HSCROLL:
  	  	  	switch(LOWORD(wParam)) {
				case SB_PAGELEFT:
					switch(GetDlgCtrlID((HWND)lParam)) {
						case IDC_SCROLLBAR_RHO:
  	  	  	    			ptrScene->viewRefPoint.SetRho (ptrScene->viewRefPoint.GetRho() - 100);
					  		break;
							
						case IDC_SCROLLBAR_THETA:
  	  	  	    			ptrScene->viewRefPoint.SetTheta (ptrScene->viewRefPoint.GetTheta() - 10);
					  		break;
							
						case IDC_SCROLLBAR_PHI:
  	  	  	    			ptrScene->viewRefPoint.SetPhi (ptrScene->viewRefPoint.GetPhi() - 10);
					  		break;
							
						case IDC_SCROLLBAR_VIEWDIST:
  	  	  	    			ptrScene->viewRefPoint.SetViewPlaneDist (ptrScene->viewRefPoint.GetViewPlaneDist() - 100);
					  		break;
					}
					break;

				case SB_LINELEFT:
					switch(GetDlgCtrlID((HWND)lParam)) {
						case IDC_SCROLLBAR_RHO:
  	  	  	    			ptrScene->viewRefPoint.SetRho (ptrScene->viewRefPoint.GetRho() - 10);
					  		break;
							
						case IDC_SCROLLBAR_THETA:
  	  	  	    			ptrScene->viewRefPoint.SetTheta (ptrScene->viewRefPoint.GetTheta() - 1);
					  		break;
							
						case IDC_SCROLLBAR_PHI:
  	  	  	    			ptrScene->viewRefPoint.SetPhi (ptrScene->viewRefPoint.GetPhi() - 1);
					  		break;
							
						case IDC_SCROLLBAR_VIEWDIST:
  	  	  	    			ptrScene->viewRefPoint.SetViewPlaneDist (ptrScene->viewRefPoint.GetViewPlaneDist() - 10);
					  		break;
					}
					break;
				  
				case SB_LINERIGHT:
					switch(GetDlgCtrlID((HWND)lParam)) {
						case IDC_SCROLLBAR_RHO:
  	  	  	    			ptrScene->viewRefPoint.SetRho (ptrScene->viewRefPoint.GetRho() + 10);
					  		break;
							
						case IDC_SCROLLBAR_THETA:
  	  	  	    			ptrScene->viewRefPoint.SetTheta (ptrScene->viewRefPoint.GetTheta() + 1);
					  		break;
							
						case IDC_SCROLLBAR_PHI:
  	  	  	    			ptrScene->viewRefPoint.SetPhi (ptrScene->viewRefPoint.GetPhi() + 1);
					  		break;
							
						case IDC_SCROLLBAR_VIEWDIST:
  	  	  	    			ptrScene->viewRefPoint.SetViewPlaneDist (ptrScene->viewRefPoint.GetViewPlaneDist() + 10);
					  		break;
					}
					break;
				  
				case SB_PAGERIGHT:
					switch(GetDlgCtrlID((HWND)lParam)) {
						case IDC_SCROLLBAR_RHO:
  	  	  	    			ptrScene->viewRefPoint.SetRho (ptrScene->viewRefPoint.GetRho() + 100);
					  		break;
							
						case IDC_SCROLLBAR_THETA:
  	  	  	    			ptrScene->viewRefPoint.SetTheta (ptrScene->viewRefPoint.GetTheta() + 10);
					  		break;
							
						case IDC_SCROLLBAR_PHI:
  	  	  	    			ptrScene->viewRefPoint.SetPhi (ptrScene->viewRefPoint.GetPhi() + 10);
					  		break;
							
						case IDC_SCROLLBAR_VIEWDIST:
  	  	  	    			ptrScene->viewRefPoint.SetViewPlaneDist (ptrScene->viewRefPoint.GetViewPlaneDist() + 100);
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
  	  	  	    			ptrScene->viewRefPoint.SetRho (value);
					  		break;
							
						case IDC_SCROLLBAR_THETA:
  	  	  	    			ptrScene->viewRefPoint.SetTheta (value);
					  		break;
							
						case IDC_SCROLLBAR_PHI:
  	  	  	    			ptrScene->viewRefPoint.SetPhi (value);
					  		break;
							
						case IDC_SCROLLBAR_VIEWDIST:
  	  	  	    			ptrScene->viewRefPoint.SetViewPlaneDist (value);
					  		break;
					}
                }
				break;

				default:
				    return FALSE;
			}

  	    	SetDlgItemInt (hwndDlg, IDC_VP_VIEW_DISTANCE, ptrScene->viewRefPoint.GetViewPlaneDist(), true);
  	    	SetDlgItemInt (hwndDlg, IDC_VP_RHO, ptrScene->viewRefPoint.GetRho(), true);
  	    	SetDlgItemInt (hwndDlg, IDC_VP_THETA, ptrScene->viewRefPoint.GetTheta(), true);
  	    	SetDlgItemInt (hwndDlg, IDC_VP_PHI, ptrScene->viewRefPoint.GetPhi(), true);

		  	SendMessage (hwndDlg, WM_COMMAND, IDC_EXECUTE, 0L);
		  	return TRUE;

  	  	case WM_COMMAND:
            BOOL translated;
            
  	  	  	switch(LOWORD(wParam)) {
			  	case IDC_VP_VIEW_DISTANCE:
                {
			    	int intValue = GetDlgItemInt (hwndDlg, IDC_VP_VIEW_DISTANCE, &translated, true);
			    	ptrScene->viewRefPoint.SetViewPlaneDist (intValue);
  	  	    		hwndCtrl = GetDlgItem(hwndDlg, IDC_SCROLLBAR_VIEWDIST);
  	  	    		SetScrollPos (hwndCtrl, SB_CTL, intValue, TRUE);
			    	return TRUE;
                }

			  	case IDC_VP_RHO:
                {
			    	int intValue = GetDlgItemInt (hwndDlg, IDC_VP_RHO, &translated, true);
			    	ptrScene->viewRefPoint.SetRho (intValue);
  	  	    		hwndCtrl = GetDlgItem(hwndDlg, IDC_SCROLLBAR_RHO);
  	  	    		SetScrollPos (hwndCtrl, SB_CTL, intValue, TRUE);
			    	return TRUE;
                }

			  	case IDC_VP_THETA:
                {
			    	int intValue = GetDlgItemInt (hwndDlg, IDC_VP_THETA, &translated, true);
			    	ptrScene->viewRefPoint.SetTheta (intValue);
  	  	    		hwndCtrl = GetDlgItem(hwndDlg, IDC_SCROLLBAR_THETA);
  	  	    		SetScrollPos (hwndCtrl, SB_CTL, intValue, TRUE);
					return TRUE;
                }

			  	case IDC_VP_PHI:
                {
			    	int intValue = GetDlgItemInt (hwndDlg, IDC_VP_PHI, &translated, true);
			    	ptrScene->viewRefPoint.SetPhi (intValue);
  	  	    		hwndCtrl = GetDlgItem(hwndDlg, IDC_SCROLLBAR_PHI);
  	  	    		SetScrollPos (hwndCtrl, SB_CTL, intValue, TRUE);
					return TRUE;
                }

  	  	  		case IDC_EXECUTE:
                {
  	  	    		ptrScene->SetViewVariables();
  	  	    		ptrScene->sceneChanged = true;
				    RefreshScreen (hwndParent);
					return TRUE;
                }

  	  	  		case IDC_PV_RESET:
                {
  	  	    		ptrScene->viewRefPoint = ViewPointRec (1000, 5000, 45, 75);
  	  	    		InitViewRefPointDlg(hwndDlg, &ptrScene->viewRefPoint);
  	  	    		ptrScene->SetViewVariables();
  	  	    		ptrScene->sceneChanged = true;
				    RefreshScreen (hwndParent);
					return TRUE;
                }

			  	case IDOK:
                {
  	  	    		ptrScene->SetViewVariables();
  	  	    		ptrScene->sceneChanged = true;
				    RefreshScreen (hwndParent);

					EndDialog(hwndDlg, 0);
			    	return TRUE;
                }

			  	case IDCANCEL:
                {
					EndDialog(hwndDlg, 1);
			    	return TRUE;
                }
			}
			break;
  	}
  	return FALSE;
}

/**********************************************************
 * InitViewRefPointDlg
 * 
 * InitViewRefPointDlg
 * 
 * parameters IN:
 *	HWND hwndDlg
 * 	ViewPointRec * viewRefPoint
 * 
 * return value : void
 *********************************************************/
void InitViewRefPointDlg(HWND hwndDlg, ViewPointRec * viewRefPoint)
{
  	HWND hwndCtrl;

  	SetDlgItemInt (hwndDlg, IDC_VP_VIEW_DISTANCE, viewRefPoint->GetViewPlaneDist(), true);
  	hwndCtrl = GetDlgItem(hwndDlg, IDC_SCROLLBAR_VIEWDIST);
  	SetScrollRange (hwndCtrl, SB_CTL, 1, 10000, TRUE);
  	SetScrollPos (hwndCtrl, SB_CTL, viewRefPoint->GetViewPlaneDist(), TRUE);

  	SetDlgItemInt (hwndDlg, IDC_VP_RHO, viewRefPoint->GetRho(), true);
  	hwndCtrl = GetDlgItem(hwndDlg, IDC_SCROLLBAR_RHO);
  	SetScrollRange (hwndCtrl, SB_CTL, 1, 10000, TRUE);
  	SetScrollPos (hwndCtrl, SB_CTL, viewRefPoint->GetRho(), TRUE);

  	SetDlgItemInt (hwndDlg, IDC_VP_THETA, viewRefPoint->GetTheta(), true);
  	hwndCtrl = GetDlgItem(hwndDlg, IDC_SCROLLBAR_THETA);
  	SetScrollRange (hwndCtrl, SB_CTL, -180, +180, TRUE);
  	SetScrollPos (hwndCtrl, SB_CTL, viewRefPoint->GetTheta(), TRUE);

  	SetDlgItemInt (hwndDlg, IDC_VP_PHI, viewRefPoint->GetPhi(), true);
  	hwndCtrl = GetDlgItem(hwndDlg, IDC_SCROLLBAR_PHI);
  	SetScrollRange (hwndCtrl, SB_CTL, -180, +180, TRUE);
  	SetScrollPos (hwndCtrl, SB_CTL, viewRefPoint->GetPhi(), TRUE);
}

/**********************************************************
 * AboutBoxDlgProc
 * 
 * AboutBoxDlgProc
 * 
 * parameters IN:
 *	HWND hwndDlg
 * 	UINT uMsg
 * 	  	WM_COMMAND
 * 	  	  	LOWORD(wParam)
 *				IDOK
 *				IDCANCEL
 * 
 *	WPARAM wParam
 *	LPARAM lParam
 * 
 * return value : BOOL
 *********************************************************/
BOOL CALLBACK AboutBoxDlgProc (HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
