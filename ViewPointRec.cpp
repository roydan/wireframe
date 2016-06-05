#include <cmath>

#include "ViewPointRec.h"
#include "Util.h"

/**********************************************************
 * 
 * ViewPointRec
 * 
 * parameters IN:
 * 	int viewPlaneDist	default = 1000
 *	int rho				default = 5000
 *	int theta			default = 45
 *	int phi				default = 45
 * 
 * return value : ViewPointRec *
 * 
 *********************************************************/
ViewPointRec::ViewPointRec (int viewPlaneDist, int rho, int theta, int phi) {
    this->viewPlaneDist = viewPlaneDist;
  	this->rho = rho;
    this->theta = theta;
    this->phi = phi;
}

/**********************************************************
 * 
 * operator =
 * 
 * parameters IN :
 *	const ViewPointRec & value
 * 
 *********************************************************/
void ViewPointRec::operator = (const ViewPointRec & value)
{
    this->viewPlaneDist = value.viewPlaneDist;
  	this->rho = value.rho;
    this->theta = value.theta;
    this->phi = value.phi;
}

/**********************************************************
 * 
 * SetViewVariables
 * 
 * parameters IN :
 *	MyMatrix & viewTransformation
 * 
 *********************************************************/
void ViewPointRec::SetViewVariables (MyMatrix & viewTransformation) {
  	sinTheta = sin ( Util::InRadians (theta) );
  	cosTheta = cos ( Util::InRadians (theta) );
  	sinPhi = sin ( Util::InRadians (phi) );
  	cosPhi = cos ( Util::InRadians (phi) );
  	xView = rho * cosTheta * sinPhi;
  	yView = rho * sinTheta * sinPhi;
  	zView = rho * cosPhi;

  	SetViewTransformation (viewTransformation);
}

/**********************************************************
 * 
 * SetViewTransformation
 * 
 * parameters IN :
 *	MyMatrix & viewTransformation
 * 
 *********************************************************/
void ViewPointRec::SetViewTransformation (MyMatrix & viewTransformation) {
  	viewTransformation.Identity();

  	viewTransformation[0][0] = -sinTheta;
  	viewTransformation[0][1] = -cosTheta * cosPhi;
  	viewTransformation[0][2] = -cosTheta * sinPhi;
  	viewTransformation[1][0] =  cosTheta;
  	viewTransformation[1][1] = -sinTheta * cosPhi;
  	viewTransformation[1][2] = -sinTheta * sinPhi;
  	viewTransformation[2][1] =  sinPhi;
  	viewTransformation[2][2] = -cosPhi;
  	viewTransformation[3][2] =  rho;
}

