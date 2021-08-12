#ifndef __CONVERSION_H__
#define __CONVERSION_H__
#include <stdio.h>
#include <math.h>
#include"toolBox.h"


INF_SUP FpInt2InfSup(FP_INT x);

MID_RAD InfSup2MidRad(INF_SUP x);

FP_INT intval2FpInt(double center, double rad);

#endif
