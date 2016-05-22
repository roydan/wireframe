#ifndef VIEW_POINT_REC_H
#define VIEW_POINT_REC_H

#include "Matrix.h"

using namespace std;

/**********************************************************
 *
 * classe ViewPointRec
 *
 * structure du systeme de visualisation simple a 4 parametres
 *
 *********************************************************/

class ViewPointRec {
private:
  	double xView, yView, zView;
  	double sinTheta, cosTheta, sinPhi, cosPhi;
  	int viewPlaneDist;
  	int rho, theta, phi;

public:
    ViewPointRec (int viewPlaneDist = 1000, int rho = 5000, int theta = 45, int phi = 45);
    void operator = (const ViewPointRec& another);

    void SetViewVariables (Matrix & viewTransformation);
    void SetViewTransformation (Matrix & viewTransformation);

    double GetXView() { return xView; }
    double GetYView() { return yView; }
    double GetZView() { return zView; }

	/************
    void SetXView (int value) { xView = value; }
    void SetYView (int value) { yView = value; }
    void SetZView (int value) { zView = value; }
	************/

    int GetViewPlaneDist() { return viewPlaneDist; }
    int GetRho() { return rho; }
    int GetTheta() { return theta; }
    int GetPhi() { return phi; }

    void SetViewPlaneDist (int value) { viewPlaneDist = value; }
    void SetRho (int value) { rho = value; }
    void SetTheta (int value) { theta = value; }
    void SetPhi (int value) { phi = value; }
};

#endif   // VIEW_POINT_REC_H
