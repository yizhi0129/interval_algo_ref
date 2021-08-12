#ifndef __CALCUL_H__
#define __CALCUL_H__
#include"calcul.h"
#include"toolBox.h"
#include"conversion.h"
#include<math.h>

int in(double x,double a,double b);

FP_INT add(FP_INT x1, FP_INT x2);


FP_INT sous(FP_INT x1, FP_INT x2);


FP_INT mult(FP_INT x1, FP_INT x2);

FP_INT inv(FP_INT x);

FP_INT divi(FP_INT x1, FP_INT x2);

FP_INT intersection(FP_INT x1, FP_INT x2);
FP_INT newton(FP_INT A, int imax);
FP_INT FpIntExp(FP_INT x);
FP_INT FpIntSin(FP_INT I);
#endif