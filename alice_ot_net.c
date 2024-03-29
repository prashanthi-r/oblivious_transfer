#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h> 
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<string.h>
#include<arpa/inet.h>
#include<fcntl.h> 
#include<unistd.h>
#include <strings.h>
#include<gmp.h>

int alice_ot1();
int alice_ot2();

int main(int argc, char** argv){
	int alice, bob;
	alice = socket(AF_INET, SOCK_STREAM, 0); //establishing an endpoint

	struct sockaddr_in server_alice;
	struct sockaddr_in server_bob;
	server_alice.sin_family=AF_INET;
	server_alice.sin_port=htons(atoi(argv[1]));
	server_alice.sin_addr.s_addr=INADDR_ANY;
	bzero(&server_alice.sin_zero, 8);
	
	alice_ot1();

	//reading x_0 and x_1 from file
	FILE *randomx = fopen("randomx.txt", "r"); //file to read randomly generated x_0, x_1 by Alice
	char *x_0 = (char*) malloc(500*sizeof(char));
	char *x_1 = (char*) malloc(500*sizeof(char));

	fscanf(randomx, "%s", x_0);
	fscanf(randomx, "%s", x_1);
	printf("x_0 as string: %s\n", x_0);

	int len = sizeof(struct sockaddr_in);

	if((bind(alice, (struct sockaddr *)&server_alice, sizeof(struct sockaddr_in))) != 0){
		printf("Server binding unsuccessful!\n");
		exit(0);
	}
	else
	{
		printf("Server bound to descriptor alice. \n");
	}

	//Waiting for connection requests
	if((listen(alice, 5))==-1) 
	{
		perror("\nListen failed! ");
		exit(0);
	}
	else
	{
		printf("Alice is listening...\n");
	}

	//Accepting a connection
	if((bob = accept(alice, (struct sockaddr *)&server_bob, &len))==-1)
	{
		perror("\nCouldn't accept any network! ");
		exit(0);
	}
	else
	{
		printf("Connected to Bob!\n");
	}
	
	printf("Sending to Bob:\n");
	//Oblivious transfer: sending x_0, x_1
	int l1 = strlen(x_0);
  	unsigned len1;
	len1 = htonl((unsigned)l1);
	send(bob, (char *)&len1, sizeof(long), 0); //sending the length of x_0
	send(bob, x_0, l1, 0);

	int l2 = strlen(x_1);
  	unsigned len2;
	len2 = htonl((unsigned)l2);
	send(bob, (char *)&len2, sizeof(long), 0); //sending length of x_1
	send(bob, x_1, l2, 0);
	
	//Receiving c=(x_b + (k^e)) mod n	
	FILE *bob_c = fopen("bob_c.txt", "w"); //file to store c from Bob
	
	int l;
	unsigned length;
  	recv(bob, (char *)&length, sizeof(long), 0); //receiving the size of c
  	l = (int)ntohl(length);
  	printf("length of c = %d\n", l);
  	char *c = (char*) malloc(l*sizeof(char));

	recv(bob, c, l, 0);
	fprintf(bob_c, "%s", c);
	printf("Received c!\n");
	
	alice_ot2();

	//reading m'_0 and m'_1 from file
	FILE *enc_messages = fopen("enc_messages.txt", "r"); //file to read randomly generated x_0, x_1 by Alice
	char *md_0 = (char*) malloc(500*sizeof(char));
	char *md_1 = (char*) malloc(500*sizeof(char));

	fscanf(enc_messages, "%s", md_0);
	fscanf(enc_messages, "%s", md_1);
	
	printf("Sending m's to Bob...\n");

	l1 = strlen(md_0);
	len1 = htonl((unsigned)l1);
	send(bob, (char *)&len1, sizeof(long), 0); //sending length of md_0
	send(bob, md_0, l1, 0);

	l2 = strlen(md_1);
	len2 = htonl((unsigned)l2);
	send(bob, (char *)&len2, sizeof(long), 0); //sending length of md_1
	send(bob, md_1, l2, 0);

	close(bob);
}

int alice_ot1(){
	FILE *fptr;
	fptr = fopen("randomx.txt", "w"); //file to store randomly generated x_0, x_1
	mpz_t x_0, x_1;
	int seed;
	
	printf("Enter seed for alice_ot1: ");
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

int alice_ot2(){
	FILE *fp1, *fp2, *fp3, *fp4, *fp5;
	fp1 = fopen("private_key.txt", "r"); //private key d is extracted from "private_key.txt"
	fp2 = fopen("bob_c.txt", "r"); //c, generated by Bob is extracted from "bob_c.txt" --> c = (x_b + k^e) mod n
	fp3 = fopen("strings.txt", "r"); //the two strings are read from "strings.txt"
	fp4 = fopen("enc_messages.txt", "w"); // to write m'_0 (= m_0 - v_0) and m'_1 (= m_1 - v_1) to "enc_messages.txt"
	fp5 = fopen("randomx.txt", "r"); //to read x_0 and x_1 from "randomx.txt"
	mpz_t x_0, x_1, v_0, v_1, n, d, c, m_0, m_1, md_0, md_1;
	mpz_init(x_0);
	mpz_init(x_1);
	mpz_init(v_0);
	mpz_init(v_1);
	mpz_init(n);
	mpz_init(d);
	mpz_init(c);
	mpz_init(m_0);
	mpz_init(m_1);
	mpz_init(md_0);
	mpz_init(md_1);
	gmp_fscanf(fp1, "%Zd", n); //extracting the value of n generated by keygen.c in "private_key.txt"
	gmp_fscanf(fp1, "%Zd", d); //extracting the value of d generated by keygen.c in "private_key.txt"
 	gmp_fscanf(fp2, "%Zd", c); //extracting the value of c from "bobc.txt"
 	mpz_fdiv_r(c, c, n);
 	gmp_fscanf(fp5, "%Zd", x_0); //extracting the value of x_0 (first string) from "randomx.txt"
 	mpz_fdiv_r(x_0, x_0, n);
	gmp_fscanf(fp5, "%Zd", x_1); //extracting the value of x_1 (second string) from "randomx.txt"
	mpz_fdiv_r(x_1, x_1, n);
	mpz_sub(v_0, c, x_0); // v_0 = c - x_0
	mpz_fdiv_r(v_0, v_0, n);
	mpz_sub(v_1, c, x_1); //v_1 = c - x_1
	mpz_fdiv_r(v_1, v_1, n);
	mpz_powm(v_0, v_0, d, n); //v_0 = (c - x_0)^d mod n
	mpz_powm(v_1, v_1, d, n); //v_1 = (c - x_1)^d mod n
	gmp_fscanf(fp3, "%Zd", m_0); //extracting the value of m_0 (first string) from "strings.txt"
	gmp_fscanf(fp3, "%Zd", m_1); //extracting the value of m_1 (second string) from "strings.txt"
	mpz_sub(md_0, m_0, v_0); //m'_0 = m_0 - v_0
	mpz_sub(md_1, m_1, v_1); //m'_1 = m_1 - v_1
	gmp_fprintf(fp4, "%Zd", md_0); //writing m'_0 to file "enc_messages.txt"
	gmp_fprintf(fp4, "\n");
	gmp_fprintf(fp4, "%Zd", md_1); //writing m'_1 to file "enc_messages.txt"
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	fclose(fp5);
	mpz_clear(x_0);
	mpz_clear(x_1);
	mpz_clear(v_0);
	mpz_clear(v_1);
	mpz_clear(n);
	mpz_clear(d);
	mpz_clear(c);
	mpz_clear(m_0);
	mpz_clear(m_1);
	mpz_clear(md_0);
	mpz_clear(md_1);
}
