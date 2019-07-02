

//mpz_init(temp);
//mpz_init(temp_2);

//Calculates gcd of a and b and stores in g
void gcd(mpz_t g,mpz_t a,mpz_t b){
	mpz_t rem,temp,temp_2;
	mpz_t x,y;
	mpz_init(temp);
	mpz_init(temp_2);
	mpz_init(x);
	mpz_init(y);
	mpz_set(x,a);
	mpz_set(y,b);

	if(mpz_cmp(x,y)<0)
		mpz_swap(x,y);

	while(mpz_cmp_si(y,0)!=0){
		mpz_set(temp,y);
		mpz_fdiv_r(temp_2,x,y);
		mpz_set(y,temp_2);
		mpz_set(x,temp);
	}
	mpz_set(g,x);
	mpz_clear(x);
	mpz_clear(y);
	mpz_clear(temp);
	mpz_clear(temp_2);
}

//Calculates a^t mod n and stores in b /******WRONG FUNCTION -- NEED TO UPDATE *******/
void pmod(mpz_t b,mpz_t a,mpz_t t,mpz_t n){
	mpz_t rem,temp,temp_2;
	mpz_init(temp);
	mpz_init(temp_2);

	//bit stores the bit representation of t
	int *bit=(int *)malloc(1024*sizeof(int));
	int c=0;
	while(mpz_cmp_si(t,0)!=0){
		bit[c]=mpz_fdiv_q_ui(t,t,2);
		c+=1;
	}
	bit=(int *)realloc(bit,(c+1)*sizeof(int));

	mpz_set_si(b,1);

	for(int i=c-1;i>=0;i--){
		mpz_fdiv_r(temp,b,n);
		mpz_fdiv_r(temp_2,b,n);
		mpz_mul(temp,temp,temp_2);
		mpz_fdiv_r(temp,temp,n);
		mpz_set(b,temp);
		if(bit[i]==1){
			mpz_fdiv_r(temp,b,n);
			mpz_fdiv_r(temp_2,a,n);
			mpz_mul(temp,temp,temp_2);
			mpz_fdiv_r(temp,temp,n);
			mpz_set(b,temp);
		}
	}

	mpz_clear(temp);
	mpz_clear(temp_2);

}
/******WRONG FUNCTION -- NEED TO UPDATE**********/

//calculates the multiplicative inverse of m in mod phi_n and stores in ans
void mul_inv(mpz_t ans,mpz_t m,mpz_t phi_n){
	mpz_t t,a,q,r,n,e,temp;
	mpz_init(e);
	mpz_init(n);
	mpz_init(t);
	mpz_init(a);
	mpz_init(q);
	mpz_init(r);
	mpz_init(temp);
	
	mpz_set(e,m);
	mpz_set(n,phi_n);
	mpz_set_ui(t,1);
	mpz_set_ui(ans,0);
	mpz_set(a,n);
	mpz_fdiv_q(q,e,n);
	mpz_fdiv_r(r,e,n);

	while(mpz_cmp_si(r,0)>0){
		mpz_mul(temp,q,ans);
		mpz_sub(temp,t,temp);
		mpz_set(t,ans);
		mpz_set(ans,temp);
		mpz_set(e,n);
		mpz_set(n,r);
		mpz_fdiv_q(q,e,n);
		mpz_fdiv_r(r,e,n);
	}
	

	mpz_fdiv_r(ans,ans,a);
	if(mpz_cmp_si(ans,0)==0)
		mpz_set(ans,a);


	mpz_clear(e);
	mpz_clear(n);
	mpz_clear(t);
	mpz_clear(a);
	mpz_clear(q);
	mpz_clear(r);
	mpz_clear(temp);
}

int m_r(mpz_t n){
	mpz_t m,k,a,b,p,i,temp,temp_2;
	mpz_init(m);
	mpz_init(k);
	mpz_init(p);
	mpz_init(a);
	mpz_init(b);
	mpz_init(i);

	gmp_randstate_t state;
	gmp_randinit_mt(state);
	
	mpz_sub_ui(m,n,1);
	mpz_sub_ui(p,m,1);
	mpz_set_si(k,0);

	//find p and k such that m=p*2^k
	while(mpz_fdiv_q_ui(m,m,2)==0)
		mpz_add_ui(k,k,1);
	mpz_mul_ui(m,m,2);
	mpz_add_ui(m,m,1);

	mpz_urandomm(a,state,p);
	mpz_add_ui(a,a,1);
	mpz_add_ui(p,p,1);

	pmod(b,a,m,n);
	if(mpz_cmp_si(b,1)==0){
		mpz_clear(m);
		mpz_clear(k);
		mpz_clear(a);
		mpz_clear(b);
		mpz_clear(i);
		return 1;
	}

	mpz_init(temp);
	mpz_init(temp_2);

	for(mpz_set_si(i,0);mpz_cmp(i,k)<0;mpz_add_ui(i,i,1)){
		if(mpz_cmp(b,p)==0){
			mpz_clear(temp);
			mpz_clear(temp_2);
			mpz_clear(m);
			mpz_clear(k);
			mpz_clear(a);
			mpz_clear(b);
			mpz_clear(i);
			return 1;
		}
		mpz_fdiv_r(temp,b,n);
		mpz_fdiv_r(temp_2,b,n);
		mpz_mul(temp,temp,temp_2);
		mpz_fdiv_r(temp,temp,n);
		mpz_set(b,temp);
	}

	mpz_clear(temp);
	mpz_clear(temp_2);
	mpz_clear(m);
	mpz_clear(k);
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(i);
	return 0;
}

//check if p is prime
int is_prime(mpz_t p){
	//perform miller-rabin test 20 times, should pass every time if prime
	for(int i=0;i<20;i++){
		if(m_r(p)==0)
			return 0;
	}
	return 1;
}
