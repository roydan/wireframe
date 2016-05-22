#include <cmath>

#include "ViewPointRec.h"
#include "Util.h"

ViewPointRec::ViewPointRec(int viewPlaneDist, int rho, int theta, int phi) {
    this->viewPlaneDist = viewPlaneDist;
  	this->rho = rho;
    this->theta = theta;
    this->phi = phi;
}

void ViewPointRec::SetViewVariables() {
  	sinTheta = sin(InRadians(theta));
  	cosTheta = cos(InRadians(theta));
  	sinPhi = sin(InRadians(phi));
  	cosPhi = cos(InRadians(phi));
  	xView = rho * cosTheta * sinPhi;
  	yView = rho * sinTheta * sinPhi;
  	zView = rho * cosPhi;
}

void ViewPointRec::SetViewTransformation(Matrix & viewTransformation) {
  	viewTransformation.SetValue(0, 0, -sinTheta);
  	viewTransformation.SetValue(0, 1, -cosTheta * cosPhi);
  	viewTransformation.SetValue(0, 2, -cosTheta * sinPhi);
  	viewTransformation.SetValue(1, 0,  cosTheta);
  	viewTransformation.SetValue(1, 1, -sinTheta * cosPhi);
  	viewTransformation.SetValue(1, 2, -sinTheta * sinPhi);
  	viewTransformation.SetValue(2, 1,  sinPhi);
  	viewTransformation.SetValue(2, 2, -cosPhi);
  	viewTransformation.SetValue(3, 2,  rho);
}
