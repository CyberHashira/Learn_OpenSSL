/*
	- Thanks for reading DISCLAIMER.txt
	This sample code has three objectives :-
	1. Generate ECDSA based keypair.
	2. Sign a text using sha256WithECDSA signature algorithm.
	3. Verify the signature.
*/


#include <iostream>
#include <openssl/ec.h>
#include <openssl/evp.h>
#include <openssl/err.h>

using namespace std;

EVP_PKEY_CTX *ecCtx = NULL;
EVP_PKEY *ec = NULL;
EVP_MD_CTX *mdCtx = NULL;
const char curve[] = "secp384r1";
const char message[] = "Earth is the third planet of our Solar System.";
unsigned char *signature = NULL;
unsigned long sigLen = 0;

void generateKeyPair();
void signData();
void verifyData();
void freeMemory();

// Generate ECDSA Keypair
void generateKeyPair()
{
	ecCtx =  EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
	EVP_PKEY_keygen_init(ecCtx);
	EVP_PKEY_CTX_set_ec_paramgen_curve_nid(ecCtx, OBJ_txt2nid(curve));

	if(EVP_PKEY_keygen(ecCtx, &ec)!=1)
	{
		cout << "Failed to generate ECDSA keypair." << ERR_error_string(ERR_get_error(), NULL) << endl;
		exit(1);
	}
}

// Sign data
void signData()
{
	mdCtx = EVP_MD_CTX_new();
	EVP_DigestSignInit(mdCtx, NULL, EVP_sha256(), NULL, ec);
	EVP_DigestSignUpdate(mdCtx, message, sizeof(message)-1);
	EVP_DigestSignFinal(mdCtx, NULL, &sigLen);
	signature = new unsigned char[sigLen];
	EVP_DigestSignFinal(mdCtx, signature, &sigLen);
	if(ERR_get_error()!=0)
	{
		cout << "Signing failed." << endl;
		cout << ERR_error_string(ERR_get_error(), NULL) << endl;
		freeMemory();
		exit(1);
	}
}

// Verify signature
void verifyData()
{
	EVP_DigestVerifyInit(mdCtx, NULL, EVP_sha256(), NULL, ec);
	EVP_DigestVerifyUpdate(mdCtx, message, sizeof(message)-1);
	EVP_DigestVerifyFinal(mdCtx, signature, sigLen);
	if(ERR_get_error()!=0)
	{
		cout << "Signature verification failed." << endl;
		cout << ERR_error_string(ERR_get_error(), NULL) << endl;
		freeMemory();
		exit(1);
	}
}

// Free memory
void freeMemory()
{
	EVP_MD_CTX_free(mdCtx);
	EVP_PKEY_free(ec);
	EVP_PKEY_CTX_free(ecCtx);
	signature = NULL;
}
	
int main()
{
	generateKeyPair();
	cout << "ECDSA keypair generated..." << endl;
	signData();
	cout << "Message signed..." << endl;
	verifyData();
	cout << "Message verified..." << endl;
	freeMemory();	
	cout << "Memory freed." << endl;
	return 0;
}
