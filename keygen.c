#include<stdio.h>
#include<gmp.h>
#include<stdlib.h>
#include"rsa.h"

int main(void)
{
	mpz_t p,q,n,phi_n,e,d,bit_len,rem,temp_a,temp_b;
	mpz_init(rem);
	mpz_init(temp_a);
	mpz_init(temp_b);
	mpz_init(p);
	mpz_init(q);
	mpz_init(n);
	mpz_init(phi_n);
	mpz_init(e);
	mpz_init(d);
	mpz_init(bit_len);
	mpz_set_ui(temp_a,1);
	mpz_mul_2exp(bit_len,temp_a,511);
	int seed;
	
	printf("Enter seed: ");
	scanf("%d",&seed);
	
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	gmp_randseed_ui(state,seed);
	
	//generate a random p
	mpz_urandomb(p,state,511);
	mpz_ior(p,p,bit_len);
	
	//if p is a multiple of 2, add 1
	mpz_fdiv_r_ui(rem,p,2);
	if(mpz_cmp_si(rem,0)==0)
		mpz_add_ui(p,p,1);
	//keep adding 2 to p until it becomes prime
	while(is_prime(p)==0)
		mpz_add_ui(p,p,2);


	//generate a random q. Repeat the procedure for p
	mpz_urandomb(q,state,511);
	mpz_ior(q,q,bit_len);
	
	mpz_fdiv_r_ui(rem,q,2);
	if(mpz_cmp_si(rem,0)==0)
		mpz_add_ui(q,q,1);
	//q!=p
	while( (is_prime(q) == 0) || (mpz_cmp(q,p) == 0) )
		mpz_add_ui(q,q,2);

	//n=p*q
	mpz_mul(n,p,q);

	//phi_n=(p-1)*(q-1)
	mpz_sub_ui(temp_a,p,1);
	mpz_sub_ui(temp_b,q,1);
	mpz_mul(phi_n,temp_a,temp_b);

	//generate e and d such that (e*d)mod phi_n = 1
	mpz_set_si(d,-1);
	mpz_set_si(e,2);
	while( (mpz_cmp_si(d,-1) == 0) || (mpz_cmp(e,d) == 0) ){
		mpz_urandomm(e,state,phi_n);
		gcd(temp_a,phi_n,e);
		if( (mpz_cmp_ui(e,2) > 0) && (mpz_cmp_si(temp_a,1) == 0) && (mpz_fdiv_r_ui(temp_a,e,2) == 1) )
			mul_inv(d,e,phi_n);
	}

	//gmp_printf("phi_n=%Zd\ne=%Zd\nd=%Zd\n",phi_n,e,d);
	mpz_mul(temp_a,e,d);
	mpz_fdiv_r(temp_a,temp_a,phi_n);
	//gmp_printf("should be 1: %Zd\n",temp_a);

	FILE* public=fopen("public_key.txt","w");
	FILE* private=fopen("private_key.txt","w");
	mpz_out_str(public,10,n);
	fprintf(public,"\n");
	mpz_out_str(public,10,e);
	mpz_out_str(private,10,n);
	fprintf(private,"\n");
	mpz_out_str(private,10,d);

	fclose(public);
	fclose(private);	
	mpz_clear(rem);
	mpz_clear(temp_a);
	mpz_clear(temp_b);
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(n);
	mpz_clear(phi_n);
	mpz_clear(e);
	mpz_clear(d);
	mpz_clear(bit_len);
}
