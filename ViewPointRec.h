#ifndef VIEW_POINT_REC_H
#define VIEW_POINT_REC_H

#include "MyMatrix.h"

using namespace std;

const int DEFAULT_VIEW_PLANE_DISTANCE = 1000;
const int DEFAULT_RHO = 5000;
const int DEFAULT_THETA = 45;
const int DEFAULT_PHI = 75;

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

    ViewPointRec (int viewPlaneDist = DEFAULT_VIEW_PLANE_DISTANCE,
					int rho = DEFAULT_RHO,
					int theta = DEFAULT_THETA,
					int phi = DEFAULT_PHI);
					
    void operator = (const ViewPointRec& value);

    void SetViewVariables (MyMatrix & viewTransformation);
    void SetViewTransformation (MyMatrix & viewTransformation);

    double GetXView() { return xView; }
    double GetYView() { return yView; }
    double GetZView() { return zView; }

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

