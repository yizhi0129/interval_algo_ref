#ifndef __CONVERSION_H__
#define __CONVERSION_H__
#include <stdio.h>
#include <math.h>
#include"toolBox.h"


static inline INF_SUP FpInt2InfSup(FP_INT x){
	union ieee754_double f;
	f.d = (double)(x);
	double ulss = uls(x);
	INF_SUP res;
	res.inf = f.d-ulss;
	res.sup = f.d+ulss;
	return res;
}

static inline MID_RAD InfSup2MidRad(INF_SUP x){
	union ieee754_double mid;
	union ieee754_double rad;

	mid.d = x.sup+x.inf;
	mid.ieee.exponent = mid.ieee.exponent-1;
	rad.d = x.sup-x.inf;
	rad.ieee.exponent = rad.ieee.exponent-1;

	MID_RAD res;
	res.mid = mid.d;
	res.rad = rad.d;
	return res;
}

FP_INT intval2FpInt(double center, double rad);

#endif
