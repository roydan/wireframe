#ifndef VIEW_POINT_REC_H
#define VIEW_POINT_REC_H

#include "Matrix.h"

using namespace std;

// structure du systeme de visualisation simple a 4 parametres
class ViewPointRec {
private:
  	double sinTheta, cosTheta, sinPhi, cosPhi;
  	double xView, yView, zView;

public:
  	int viewPlaneDist;
  	int rho, theta, phi;

    ViewPointRec(int viewPlaneDist = 1000, int rho = 5000, int theta = 45, int phi = 45);
    double GetXView() {return xView; }
    double GetYView() {return yView; }
    double GetZView() {return zView; }
    void SetViewVariables();
    void SetViewTransformation(Matrix & viewTransformation);
};

#endif   // VIEW_POINT_REC_H
