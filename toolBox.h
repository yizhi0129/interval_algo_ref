#ifndef __TOOLBOX_H__
#define __TOOLBOX_H__

#include <ieee754.h>
#include<stdio.h>
#include <math.h>
#include<string.h>
#include <stdlib.h>

typedef double FP_INT;

typedef struct{
	double inf;
	double sup;
}INF_SUP;

typedef struct{
	double mid;
	double rad;
}MID_RAD;

double pow2(int p);

// Fonction qui à partir d'un FP_intervalle rend un centre et le rayon correspondant
double FPInterv2CenterRadius(FP_INT I,int *expR);

// Fonction qui à partir d'un centre et du rayon renvoie le FP_INTERV correspondant
FP_INT CenterRadius2FPInt(double c, int expR);

// Fonction qui à partir d'un flottant en double précision renvoie le FP_INTERV correspondant
FP_INT double2FpInter(double c);

FP_INT double2FpInter2(double c);

int countZeros(int x);

double uls(FP_INT f);

// Affiche l'intervalle
int FpInterBinaryPrint(FP_INT I);

int doublePrint(FP_INT I);


#endif