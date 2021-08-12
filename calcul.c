#include"calcul.h"
#include <fenv.h>
#include<math.h>




int in(double x,double a,double b){
	if (x >=a && x <= b)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

FP_INT add(FP_INT x1, FP_INT x2){
	MID_RAD intval1;
	intval1.mid = (double)x1+(double)x2;
	fesetround(FE_UPWARD);
	intval1.rad = uls(x1)+uls(x2)+fabs(intval1.mid)*pow2(53);
	fesetround(FE_TONEAREST);
	return intval2FpInt(intval1.mid,intval1.rad);
}

FP_INT sous(FP_INT x1, FP_INT x2){
	MID_RAD intval1;
	intval1.mid = (double)x1-(double)x2;
	fesetround(FE_UPWARD);
	intval1.rad = uls(x1)+uls(x2)+fabs(intval1.mid)*pow2(53);
	fesetround(FE_TONEAREST);
	return intval2FpInt(intval1.mid,intval1.rad);
}


FP_INT mult(FP_INT x1, FP_INT x2){
	MID_RAD intval1;
	intval1.mid = (double)x1*(double)x2;
	double alpha = uls(x1);
	double beta = uls(x2);
	fesetround(FE_UPWARD);
	intval1.rad = fabs((double)x1)*beta+alpha*fabs((double)x2)+ alpha*beta+fabs(intval1.mid)*pow(2,-53)+pow(2,-1074);
	fesetround(FE_TONEAREST);
	return intval2FpInt(intval1.mid,intval1.rad);
}

FP_INT inv(FP_INT x){
	double nx = (double)x;
	double r = uls(x);
	fesetround(FE_DOWNWARD);
	double c1 = (-1)/(-fabs(nx)-r);
	fesetround(FE_UPWARD);
	double c2 = ((-1)/(-fabs(nx)+r));
	double c = (c1+0.5*(c2-c1));
	double lambda = (c-c1);
	fesetround(FE_TONEAREST);
	if (nx<=0){
		c = -1*c;
	}
	return intval2FpInt(c,lambda);
}

FP_INT divi(FP_INT x1, FP_INT x2){
	return mult(x1,inv(x2));
}

FP_INT intersection(FP_INT x1, FP_INT x2){
	MID_RAD intval1;
	intval1.mid = (double) x1;
	intval1.rad = uls(x1);

	MID_RAD intval2;
	intval2.mid = (double) x2;
	intval2.rad = uls(x2);
	int swap = 0;
	if(x1==x2){
		if (intval1.rad <= intval2.rad){
			return x1;
		}else{
			return x2;
		}
	}else{
		if (intval2.mid < intval1.mid){
			intval1 = intval2;
			intval2.mid = (double) x1;
			intval2.rad = uls(x1);
			swap = 1;
		}
		double dist = fabs(intval2.mid-intval1.mid);
		if (intval1.rad+intval2.rad < dist){
			printf("interval disjoint\n");
			return (FP_INT)0;
		}
		if (dist+intval1.rad<intval2.rad){
			if (swap){
				return x1;
			}else{
				return x2;
			}
		}
		if (dist+intval2.rad<intval1.rad){
			if (swap){
				return x2;
			}else{
				return x1;
			}
		}else{
			INF_SUP intval3;
			intval3.inf = intval2.mid+intval2.rad;
			intval3.sup = intval1.mid+intval1.rad;
			MID_RAD intval3bis = InfSup2MidRad(intval3);
			return intval2FpInt(intval3bis.mid,intval3bis.rad);
		}

	}
	return (FP_INT)0;
}


// Newton method to obtain Sqrt(2)
FP_INT newton(FP_INT A, int imax){
	FP_INT I = A;
	FP_INT res;
	FP_INT Inv = 0;
	MID_RAD xs;
	MID_RAD invers;
	xs.mid = (double)I;
	xs.rad = uls(I);
	printf("hey\n");
	FP_INT Inver;
	int exprI,exprInv,exprX;
	double x;
	double centreInv,centrexInv,rayonxInv;
	int i;
	FP_INT X;
	double centre;
	int e;
	double test1;
	int test2;

	for (i = 0;i<imax;i++){
		xs.mid = (double)I;
		xs.rad = uls(I);
		X = I;
		I = intval2FpInt(2*xs.mid,xs.rad*2);
		//I = CenterRadius2FPInter(2*xs,exprI+1);
		Inver = inv(I);
		invers.mid = (double)Inver;
		invers.rad = uls(Inver);
		x = xs.mid*xs.mid-2;
		invers.mid = -invers.mid * x;
		frexp(invers.mid,&e);
		e--;
		rayonxInv = invers.rad*x;
		centre = invers.mid + xs.mid;
		//exprX= (int) ceil(log(fabs(rayonxInv))/log(2));

		I = intval2FpInt(centre,fabs(rayonxInv));
		res = X;
		FpInterBinaryPrint(X);
	//test2--;
		printf("centre = %.16lf Rayon = %16lf \n",(double)(X),uls(X));	
		printf("cpt : %d\n",i);

	}

	return res;

}


FP_INT FpIntExp(FP_INT x){
	double r = uls(x);
	double rayon;
	fesetround(FE_DOWNWARD);
	double a = (double)x-r;
	fesetround(FE_UPWARD);
	double b = (double)x+r;
	fesetround(FE_TONEAREST);
	double mid = (exp(a)+exp(b))/2.0;
	int expo;
	frexp(mid,&expo);
	expo--;
	fesetround(FE_UPWARD);
	rayon = (exp(b)-exp(a))/2.0 + pow2(-(-53+expo))+pow2(1074);
	fesetround(FE_TONEAREST);
	return intval2FpInt(mid,rayon);

}


FP_INT FpIntSin(FP_INT I){

	double center,centerSin;
	int expr,exprSin;
	double RadiusSin;
	double rayon;

	// On recupere le rayon et le centre de 1 intervalle
	center = (double)I;
	rayon = uls(I);
	double a,b;
	center = fmod(center,2*M_PI);
	double max,min;
	fesetround(FE_DOWNWARD);
	a = center-rayon;
	fesetround(FE_UPWARD);
	b = center+rayon;
	// CAS 1:
	// Cas 2
	fesetround(FE_TONEAREST);
	if (in(M_PI/2,a,b)){
		printf("Cas 1\n");
		// max = 1;
		min = fmin(sin(a),sin(b));
		if (in((3/2)*M_PI,a,b)){
				return (FP_INT)(0);
		}
		centerSin = (1 + min) /2;
		int e;
		frexp(centerSin,&e);
		e--;
		fesetround(FE_UPWARD);
		RadiusSin = fabs(1-min)/2+pow2(-(-53+e));
		fesetround(FE_TONEAREST);
		//exprSin = (int) ceil(log(RadiusSin)/log(2)) ;
		return intval2FpInt(centerSin, RadiusSin);
				
	}
	else{

		if (in((3/2)*M_PI,a,b)){
				printf("Cas 2\n");
				max = fmax(sin(a),sin(b));
				min = -1;
				centerSin = (-1 + max) /2;
				int e;
				frexp(centerSin,&e);
				e--;
				fesetround(FE_UPWARD);
				RadiusSin = fabs(-1+max)/2+pow2(-(-53+e));
				fesetround(FE_TONEAREST);
				//exprSin = (int) ceil(log(RadiusSin)/log(2)) ;
				return intval2FpInt(centerSin, RadiusSin);			
		}
		else{
			printf("Cas 3\n");
			centerSin = (sin(a)+sin(b))/2;
			//printf("sin(a) = %.53lf sin(b) = %.53lf centersin = %.53lf \n\n",sin(a),sin(b),centerSin);
			int e;
			frexp(centerSin,&e);
			e--;
			fesetround(FE_UPWARD);
			RadiusSin = fabs(sin(b)-sin(a))/2+pow2(-(-53+e));
			fesetround(FE_TONEAREST);
			//exprSin = (int) ceil(log(RadiusSin)/log(2)) ;

			return intval2FpInt(centerSin, RadiusSin);
		}


	}

}
