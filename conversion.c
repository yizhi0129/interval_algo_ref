
#include"conversion.h"

// returns the Inf-Sup representation of an FP_INT
INF_SUP FpInt2InfSup(FP_INT x){
	union ieee754_double f;
	f.d = (double)(x);
	double ulss = uls(x);
	INF_SUP res;
	res.inf = f.d-ulss;
	res.sup = f.d+ulss;
	return res;
}


//converts an Inf-Sup interval into its center-radius representation
MID_RAD InfSup2MidRad(INF_SUP x){
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

//convert a center-radius x interval into a FP_INT result y so that x is equal or included into y
FP_INT intval2FpInt(double center, double rad){

	union ieee754_double cent;
	cent.d = center;

	int center_e;
	frexp(cent.d,&center_e);

	union ieee754_double radi;
	radi.d = rad;
	int rad_e;
	frexp(radi.d,&rad_e);
	int prec = 52; // number of digits of the mantissa
	if ((center_e-rad_e-1)<prec){
		prec = center_e-rad_e-1;
	}
	union ieee754_double midrnd;
	midrnd.d = center;
	unsigned int mask0o = 0xFFFFF;
	unsigned int mask1o = 0xFFFFFFFF;

	//adjustment of the masks
	if(prec>=20){
		mask1o = mask1o << 52-prec;

	}
	else{
		mask1o = 0x00000000;
		mask0o = mask0o<<20-prec;
	}
	unsigned int mask0 = mask0o;
	unsigned int mask1 = mask1o;
	while (prec>=0){
		if(midrnd.ieee.mantissa1 == 1||midrnd.ieee.mantissa0 ==1){
			midrnd.d = midrnd.d+pow2(prec);
		}
		midrnd.ieee.mantissa1 &= mask1;
		midrnd.ieee.mantissa0 &= mask0;
		if(prec>=20){
			mask1 = mask1o <<1;

		}else{
			mask1 = 0x00000000;
			mask0 = mask0o<<1;
		}
		double x_inf = midrnd.d - pow2(prec);
		double x_sup = midrnd.d + pow2(prec);
		if(x_inf+rad <= center && center+rad<=x_sup){
			return (FP_INT)(midrnd.d);
		}
		prec = prec-1;
	}

	return (FP_INT)(0.0);
}