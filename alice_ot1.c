#include<stdio.h> 
#include<stdlib.h>
#include<gmp.h>
#include "rsa.h"
int main(){
	FILE *fptr;
	fptr = fopen("randomx.txt", "w"); //file to store randomly generated x_0, x_1
	mpz_t x_0, x_1;
	int seed;
	
	printf("Enter seed: ");
	scanf("%d",&seed);
	
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	gmp_randseed_ui(state,seed);
	mpz_init(x_0);
	mpz_init(x_1);
	mpz_urandomb(x_0, state, 500); //generating random 500-bit x_0
	mpz_urandomb(x_1, state, 500); //generating random 500-bit x_1
	gmp_fprintf(fptr, "%Zd", x_0); //writing x_0 to file "randomx.txt"
	gmp_fprintf(fptr, "\n");
	gmp_fprintf(fptr, "%Zd", x_1); //writing x_1 to file "randomx.txt"
	fclose(fptr);
	mpz_clear(x_0);
	mpz_clear(x_1);

}
