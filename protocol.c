#include <cblas.h>
#include <stdio.h>
#include <ieee754.h>
#include<string.h>
#include <stdlib.h>
#include <fenv.h>
#include<time.h>
#include<math.h>



typedef double FP_INT;

typedef struct{
	double inf;
	double sup;
}INF_SUP;

typedef struct{
	double mid;
	double rad;
}MID_RAD;



static inline int countZeros(int x){
  int count = 0;
  while ((x & 1) == 0) {
      x = x >> 1;
      count++;
  }
  return count;
}








//calculates the uls of an FP_INT interval
static inline double uls(FP_INT f){
	union ieee754_double x;
	long long int N = 1;

	x.d = f;
	int e;
	frexp(x.d,&e);
	if(x.ieee.mantissa1 !=0){
		int nb_zero = countZeros(x.ieee.mantissa1);
		return  /*pow(2,-nb_zero);*/1.0/(N<<(52-nb_zero-e+1));
	}else{
		int nb_zero = countZeros(x.ieee.mantissa0);
		return /*pow(2,-nb_zero);*/1.0/(N<<(20-nb_zero-e+1));
	}
	return 1;
}


















FP_INT intval2FpInt(double center, double rad){
	long long int N = 1;
	
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
		mask1o = mask1o << (52-prec);

	}
	else{
		mask1o = 0x00000000;
		mask0o = mask0o<<(20-prec);
	}
	unsigned int mask0 = mask0o;
	unsigned int mask1 = mask1o;
	double x_inf;
	double x_sup;
	while (prec>=0){
		if(midrnd.ieee.mantissa1 == 1||midrnd.ieee.mantissa0 ==1){
			midrnd.d = midrnd.d+/*pow(2,-nb_zero);*/1.0/(N<<prec);
			N = 1;
		}
		midrnd.ieee.mantissa1 &= mask1;
		midrnd.ieee.mantissa0 &= mask0;
		if(prec>=20){
			mask1 = mask1o <<1;

		}else{
			mask1 = 0x00000000;
			mask0 = mask0o<<1;
		}
		x_inf = midrnd.d - /*pow(2,-nb_zero);*/1.0/(N<<prec);
		N = 1;
		x_sup = midrnd.d + /*pow(2,-nb_zero);*/1.0/(N<<prec);
		N = 1;
		if(x_inf+rad <= center && center+rad<=x_sup){
			return (FP_INT)(midrnd.d);
		}
		prec = prec-1;
	}

	return (FP_INT)(0.0);
}

void mult(double * mA, double* mB,double *rA, double *rB,int k,double *mC,double*rC, double* Id,double eps, double realmin,double * Ones){

	fesetround(FE_TONEAREST);
	cblas_dgemm(CblasColMajor, CblasNoTrans, CblasTrans,k,k,k,1,mA, k, mB, k,0,mC,k);//mC
	
	
	for(int i = 0; i<k;i++){
		for(int j = 0;j<k; j++){
			mA[i*k+j] = abs(mA[i*k+j]);
			mB[i*k+j] = abs(mB[i*k+j]);
		}
	}
	
	double * temp_Id  = malloc(k*k*sizeof(double));
	double * temp_rB  = malloc(k*k*sizeof(double));
	cblas_dcopy(k*k,Id,1,temp_Id,1);
	cblas_dcopy(k*k,rB,1,temp_rB,1);
	fesetround(FE_UPWARD);
	
	
	cblas_dgemm(CblasColMajor, CblasNoTrans, CblasTrans,k,k,k,(k+2)*eps,mB, k, Id, k,1,rB,k);//rB
	
	cblas_dgemm(CblasColMajor, CblasNoTrans, CblasTrans,k,k,k,1,mA, k, rB, k,realmin,Ones,k);//Ones
	
	cblas_dgemm(CblasColMajor, CblasNoTrans, CblasTrans,k,k,k,1,mB, k, temp_Id, k,1,temp_rB,k);//temp_rB
	
	cblas_dgemm(CblasColMajor, CblasNoTrans, CblasTrans,k,k,k,1,rA, k, temp_rB, k,1,rC,k);//rC
	
	cblas_dgemm(CblasColMajor, CblasNoTrans, CblasTrans,k,k,k,1,Ones, k, Id, k,1,rC,k);//ones
	
	free(temp_Id);
	free(temp_rB);
	fesetround(FE_TONEAREST);
	
}


int main(int argc, char **argv){

	FILE *fwrite = fopen(argv[1],"w");
	int max = atoi(argv[2]);
	int pas = atoi(argv[3]);
	double eps = pow(2,-53);
	double realmin = 0.5*1/eps*pow(2,-1074)	;
	for(int k = 1;k<max;k=k+pas){
	
		printf("%d\n",k);
		int i=0;
		double *A = malloc(k*k*sizeof(double));         
		double *B = malloc(k*k*sizeof(double)); 
		double *C = malloc(k*k*sizeof(double)); 
		double *ulsA = malloc(k*k*sizeof(double));
		double *ulsB = malloc(k*k*sizeof(double));
		double *rC = malloc(k*k*sizeof(double));
		double *Id = malloc(k*k*sizeof(double));
		double *ones = malloc(k*k*sizeof(double));
		
		
		for(i = 0;i<k;i++){
			for(int j = 0;j<k;j++){
				A[i*k+j] = ((double)rand()/(double)(RAND_MAX));
				B[i*k+j] = ((double)rand()/(double)(RAND_MAX));
				C[i*k+j] = 0.0;
				rC[i*k+j] = 0.0;
				if(i==j){
					Id[i*k+j] = 1;
				}else{
					Id[i*k+j] = 0;
				}
				ones[i*k+j] = 1;
			}
		} 
		clock_t begin1 = clock();
		for(i = 0;i<k;i++){
			for(int j = 0;j<k;j++){
				ulsA[i*k+j] = uls((FP_INT)A[i*k+j]);
				ulsB[i*k+j] = uls((FP_INT)B[i*k+j]);
			}
		} 
		clock_t end1 = clock();

		
		clock_t begin = clock();
		
		 
		mult(A,B,ulsA,ulsB,k,C,rC,Id,eps,realmin,ones);
		
		
		
		clock_t end = clock(); 
		
		
		unsigned long t1 = (end-begin)*1000/CLOCKS_PER_SEC;
		//printf("temps de calcul mult : %ld ms\n",t);
		
		clock_t begin2 = clock();
		for(i = 0;i<k;i++){
			for(int j = 0;j<k;j++){
				intval2FpInt(C[i*k+j],rC[i*k+j]);
				
			}
		} 
		clock_t end2 = clock();
		unsigned long t2 = ((end1-begin1+end2-begin2))*1000/CLOCKS_PER_SEC;
		//printf("temps de calcul conv : %ld ms\n",t);
		fprintf(fwrite,"%d %ld %ld\n",k,t1,t2);
		free(A);
		free(B);
		free(C);
		free(rC);
		free(ones);
		free(Id);
		free(ulsA);
		free(ulsB);
	}
	fclose(fwrite);
  return 0;
}
