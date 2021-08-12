#include "conversion.h"
#include "toolBox.h"
#include "calcul.h"
#include<stdio.h>
#include<stdlib.h>


int main(){

	printf("test 1\n\n");
	double d1 = 4.8128;
	double d3= 8.8482;

	
	FP_INT f = (FP_INT)(d1); 
	double d2 = uls(f);
	FP_INT f2 = (FP_INT)(d2); 
	FP_INT f3 = intval2FpInt(d1,d2);
	FP_INT f4= (FP_INT)(d3);
	FP_INT f5 = add(f,f4);
	FP_INT f6 = sous(f,f4);
	FP_INT f7 = mult(f,f4);
	FP_INT f8 = divi(f,f4);
	printf("x = ");
	FpInterBinaryPrint(f);
	printf("uls(x) = ");
	FpInterBinaryPrint(f2);
	printf("y = ");
	FpInterBinaryPrint(f4);
	printf("en écriture décimale : \n");
	printf(" x : %.15f\n",d1);
	printf(" y : %.15f\n\n",d3);
	printf("test des opérations sur les FP_INT\n");
	printf(" x+y : %.15f\n",(double)f5);
	FpInterBinaryPrint(f5);
	printf("\n");
	printf(" x-y : %.15f\n",(double)f6);
	FpInterBinaryPrint(f6);
	printf("\n");
	printf(" x*y : %.15f\n",(double)f7);
	FpInterBinaryPrint(f7);
	printf("\n");
	printf(" x/y : %.15f\n",(double)f8);
	FpInterBinaryPrint(f8);
	printf("\n");

	printf("test 2\n\n");
	d1 = 4.5625;
	d3= 8.625;

	
	f = (FP_INT)(d1); 
	d2 = uls(f);
	f2 = (FP_INT)(d2); 
	f3 = intval2FpInt(d1,d2);
	f4= (FP_INT)(d3);
	f5 = add(f,f4);
	f6 = sous(f,f4);
	f7 = mult(f,f4);
	f8 = divi(f,f4);
	printf("x = ");
	FpInterBinaryPrint(f);
	printf("uls(x) = ");
	FpInterBinaryPrint(f2);
	printf("y = ");
	FpInterBinaryPrint(f4);
	printf("en écriture décimale : \n");
	printf(" x : %.15f\n",d1);
	printf(" y : %.15f\n\n",d3);
	printf("test des opérations sur les FP_INT\n");
	printf(" x+y : %.15f\n",(double)f5);
	FpInterBinaryPrint(f5);
	printf("\n");
	printf(" x-y : %.15f\n",(double)f6);
	FpInterBinaryPrint(f6);
	printf("\n");
	printf(" x*y : %.15f\n",(double)f7);
	FpInterBinaryPrint(f7);
	printf("\n");
	printf(" x/y : %.15f\n",(double)f8);
	FpInterBinaryPrint(f8);
	printf("\n");
	//printf("center : %f\n",d1);
	//printf("rayon : %15f\n",d2);
	//printf("intval : %f\n",(double)f3);

	printf("test 3\n\n");
	//union ieee754_double dd1;
	//union ieee754_double dd3;
	d1 = 3.147117;
	d3= 2.2578;

	
	f = (FP_INT)(d1); 
	d2 = uls(f);
	f2 = (FP_INT)(d2); 
	f3 = intval2FpInt(d1,d2);
	f4= (FP_INT)(d3);
	f5 = add(f,f4);
	f6 = sous(f,f4);
	f7 = mult(f,f4);
	f8 = divi(f,f4);
	FP_INT f9 = intersection(f,f4);
	printf("x = ");
	FpInterBinaryPrint(f);
	printf("uls(x) = ");
	FpInterBinaryPrint(f2);
	printf("y = ");
	FpInterBinaryPrint(f4);
	printf("en écriture décimale : \n");
	printf(" x : %.15f\n",d1);
	printf(" y : %.15f\n\n",d3);
	printf("test des opérations sur les FP_INT\n");
	printf(" x+y : %.15f\n",(double)f5);
	FpInterBinaryPrint(f5);
	printf("\n");
	printf(" x-y : %.15f\n",(double)f6);
	FpInterBinaryPrint(f6);
	printf("\n");
	printf(" x*y : %.15f\n",(double)f7);
	FpInterBinaryPrint(f7);
	printf("\n");
	printf(" x/y : %.15f\n",(double)f8);
	FpInterBinaryPrint(f8);
	printf("\n");
	printf(" x inter y : %.15f\n",(double)f9);
	FpInterBinaryPrint(f9);
	printf("\n");
	//printf("center : %f\n",d1);
	//printf("rayon : %15f\n",d2);
	//printf("intval : %f\n",(double)f3);


	printf("test 4\n\n");
	d1 = 1.414213562373;
	f = (FP_INT)(d1); 
	f2 = double2FpInter(d1);
	printf("avec un cast : \n");
	FpInterBinaryPrint(f);
	printf("en utilisant une fonction de conversion trouvée : \n");
	FpInterBinaryPrint(f2);
	printf("\n\ntest de la méthode de Newton\n\n");
	union ieee754_double sqrt2;
	sqrt2.d = 2;
	//sqrt2.ieee.mantissa0 = 0x60101;
	sqrt2.ieee.mantissa1 = 0x00F00FF;
	newton((FP_INT)sqrt2.d,8);

	printf("test des fonctions usuelles\n\nexponentielle : \n");

	FP_INT t1 = (FP_INT)0.64;

	printf("t1 = %f\nexp(t1) = %f\n",t1,FpIntExp(t1) );
	FpInterBinaryPrint(t1);
	printf("en binaire : ");
	FpInterBinaryPrint(FpIntExp(t1));
	printf("\n\nsinus\n\n");


	t1 = (FP_INT)4.5625;

	printf("t2 = %f\nsin(t2) = %f\n",t1,FpIntSin(t1) );
	FpInterBinaryPrint(t1);
	printf("en binaire : ");
	FpInterBinaryPrint(FpIntSin(t1));

	return 0;
}