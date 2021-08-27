
#include "toolBox.h"
FP_INT CenterRadius2FPInt(double center,int expR);






//calculates the uls of an FP_INT interval
double uls(FP_INT f){
	union ieee754_double x;
	union ieee754_double res;
	res.d = 2;
	x.d = f;
	int e;
	long long int N = 1;
	frexp(x.d,&e);
	if(x.ieee.mantissa1 !=0){
		int nb_zero = countZeros(x.ieee.mantissa1);
		res.ieee.mantissa1 = 0x80000000;
		res.ieee.mantissa1 = res.ieee.mantissa1>>33-nb_zero-e;
		res.d = res.d-2;
		//res.ieee.exponent = x.ieee.exponent;
		//return res.d;

		return 1.0/(N<<52-nb_zero-e+1);
	}else{
		int nb_zero = countZeros(x.ieee.mantissa0);
		res.ieee.mantissa0 = 0x80000;
		res.ieee.mantissa0 = res.ieee.mantissa0>>21-nb_zero-e;
		res.d = res.d-2;
		//res.ieee.exponent = x.ieee.exponent;
		//return res.d;
		return 1.0/(N<<20-nb_zero-e+1);
	}
	return 1;
}




// Fonction qui à partir d'un FP_intervalle rend un centre et le rayon correspondant
double FPInt2CenterRad(FP_INT I,int *expR){

	union ieee754_double nb;
	nb.d = (double)I;
	int c;
	int e;
	frexp(nb.d, &e);
	if (I==0){
		*expR = 0;
		return nb.d;
	}
	else {
	// On compte d'abord le nombre de 0 dans la mantisse 1 (en partant de la droite)
	if (nb.ieee.mantissa1 !=0){
		c = countZeros(nb.ieee.mantissa1);
		// On enleve le significant flag
		nb.ieee.mantissa1 ^= 1<<c;
		*expR = -(32+20-c);

	}
	// Si la mantisse 1 est nulle
	else{

		if (nb.ieee.mantissa0 != 0){
			c = countZeros(nb.ieee.mantissa0);
			// On enleve le significant flag
			nb.ieee.mantissa0 ^= 1<<c;
			*expR = -(20-c);

		}
		// Si les mantisse 1 et 0 sont nulles( par exemple pour I = 1 ou 2 ou 4 ...
		else{	
			nb.d = 0;
			*expR = 0;	
		}
	}
	// On oublie pas l'exposant
	*expR += e-1;
	}
	return nb.d;
}




	
FP_INT double2FpInter(double c){	
	union ieee754_double nb;
	int pos;
	nb.d = c;
	FP_INT I;
	// Cas 1: Regardons dans la mantissa 1 le dernier 1;
	if (nb.ieee.mantissa1 == 0){	
		nb.ieee.mantissa1 = 1;
		I = nb.d;
	}
	else{
		if (nb.ieee.mantissa1 & 1){
			return I = nb.d;
		}
		else{
			nb.ieee.mantissa1 |= 1;
			I = nb.d;
			
		}
	}
	return I;

}	



FP_INT double2FpInter2(double c){
	union ieee754_double nb;
	int pos;
	nb.d = c;
	FP_INT I;
	
	// Cas 1: Regardons dans la mantissa 1 le dernier 1;
	if (nb.ieee.mantissa1 != 0){
		pos = countZeros(nb.ieee.mantissa1);
		if (pos == 0){
			return (FP_INT) c;
		}
		nb.ieee.mantissa1 |= 1<<(pos-1);
		I = nb.d;
	}
	else{
		
		if (nb.ieee.mantissa0 != 0){
			pos = countZeros(nb.ieee.mantissa0);
			nb.ieee.mantissa0 |= 1<<(pos-1);
			I = nb.d;
		}
		else{
			int e;
			frexp(nb.d, &e);
			e--;
			I = CenterRadius2FPInt(nb.d,--e);
		}
	}
	return I;
}
// Fonction qui à partir d'un centre et du rayon renvoie le FP_INTERV correspondant
FP_INT CenterRadius2FPInt(double center,int expR){	
	union ieee754_double nb;
	nb.d = center;
	int c;
	int e;
	frexp(nb.d,&e);
	// Position du 1 qui code le rayon
	int pos = expR-e+2;
	if (pos>=0){
		return double2FpInter2(center);
	}
	if(-pos>=20){
		nb.ieee.mantissa1 &= (0xFFFFFFFF<< (52+pos));
		nb.ieee.mantissa1 |= (1<< (51+pos));
	}
	// Mantisse0
	
	else{
		nb.ieee.mantissa1 &= 0x00000000;
		nb.ieee.mantissa0 &= (0xFFFFF<< (20+pos));
		nb.ieee.mantissa0 |= (1 << (19+pos));
	}

	return nb.d;
}





int FpInterBinaryPrint(FP_INT I){
	int i;
	if(I==0){
		for(i=0; i<52; i++)
			printf("0");
	printf("\n");
	return 0;
	}
	union ieee754_double nb;
	nb.d = (double)I;
	int c;
	if (nb.ieee.negative == 1){
		printf("- ");
	}
	else{
		printf("+ ");
	}
	unsigned int mask;
	printf("1."); 
	mask = pow(2,19);
	if(nb.ieee.mantissa0){
		c = countZeros(nb.ieee.mantissa0);
		for(i=0;i<20-(c);i++){
			if(nb.ieee.mantissa0 & mask>>i){
				printf("1");
			}
			else{
				printf("0");
			}
		}
		if(!nb.ieee.mantissa1){
			c--;
			for(i=0;i<c;i++){
				printf("_");
			}
			
		}
		else{
			for(i=20-(c+1);i<20;i++){
				if(nb.ieee.mantissa0 & mask>>i){
					printf("1");
				}
				else{
					printf("0");
				}
			}			
		}

	}

	else{
		if(!nb.ieee.mantissa0&&!nb.ieee.mantissa1){
			for(i=0;i<20;i++){
				printf("_");
			}
		}else if(!nb.ieee.mantissa0){
			for(i=0;i<20;i++){
				printf("0");
			}			
		}
	}
	mask = pow(2,31);
	if(nb.ieee.mantissa1){
		c = countZeros(nb.ieee.mantissa1);
		for(i=0;i<32-(c);i++){
			if(nb.ieee.mantissa1 & mask>>i){
				printf("1");
			}
			else{
				printf("0");
			}

		}

		for(i=0;i<c;i++){
			printf("_");
		}
	}
	else{
		if(!nb.ieee.mantissa1){
			for(i=0;i<32;i++){
				printf("_");
			}
		}
	}
	int e;
	frexp((double)I,&e);
	e--;
	printf("* 2^(%d)\n",e);
	return 0;
}


//print the value (mantissa and exposant) of a FP_INT
int doublePrint(FP_INT I){
	int i;
	union ieee754_double nb;
	nb.d = (double)I;
	if(I==0){
		for(i=0; i<52; i++)
			printf("0");
	printf("\n");
	return 0;
	}

	if (nb.ieee.negative == 1){
		printf("- ");
	}
	else{
		printf("+ ");
	}
	printf("1."); 
	unsigned int mask;
	mask = pow(2,19);
	for(i=0;i<20;i++){
		if(nb.ieee.mantissa0 & mask>>i){
			printf("1");
		}
		else{				
			printf("0");
		}

	}
	mask = pow(2,31);
	for(i=0;i<32;i++){			
		if(nb.ieee.mantissa1 & mask>>i){
			printf("1");
		}
		else{
			printf("0");
		}

	}
	int e;
	frexp((double)I,&e);
	e--;
	printf("* 2^(%d)\n",e);

	return 0;
	}

