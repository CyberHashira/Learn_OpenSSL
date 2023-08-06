/*
	- Thanks for reading DISCLAIMER.txt
	This code generates a keypair and writes that keypair into a file.
	A user can choose between RSA and ECDSA type.
*/

#include <iostream>
#include <openssl/rsa.h>
#include <openssl/ec.h>
#include <openssl/err.h>
#include <openssl/pem.h>

using namespace std;

int keySize = 0;
EVP_PKEY *pKey = NULL;
EVP_PKEY_CTX *pCtx = NULL;
char curve_name[10];
FILE *privateKey = NULL;
FILE *publicKey = NULL;

void writeKeys()
{
	privateKey = fopen("Key.pri","wb");
        publicKey = fopen("Key.pub","wb");
        PEM_write_PKCS8PrivateKey(privateKey, pKey, NULL, NULL, 0, NULL, NULL);
        PEM_write_PUBKEY(publicKey, pKey);
        fclose(privateKey);
        fclose(publicKey);
}

void cleanUp()
{
	EVP_PKEY_CTX_free(pCtx);
	EVP_PKEY_free(pKey);
	EVP_cleanup();
}

int main()
{
	int kp_type;

	cout << "1. RSA, 2. ECDSA" << endl;
	cout << "KeyPair Type : ";
	cin >> kp_type;

	switch(kp_type)
	{
		case 1: 
			cout << "RSA Keysize : "; 
			cin >> keySize;
			pCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
			EVP_PKEY_keygen_init(pCtx);
			EVP_PKEY_CTX_set_rsa_keygen_bits(pCtx, keySize);
		break;
		case 2: 
			cout << "ECDSA curve : "; 
			cin >> curve_name;
			pCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
			EVP_PKEY_keygen_init(pCtx);
			EVP_PKEY_CTX_set_ec_paramgen_curve_nid(pCtx, OBJ_txt2nid(curve_name));
		break;
		default : cout << "Not an option." << endl; exit(1);
	}

	if(EVP_PKEY_keygen(pCtx, &pKey)!=1)
	{
		cout << "Failed to generate keypair." << endl;
		cout << "Reason : " << ERR_error_string(ERR_get_error(), NULL) << endl;
		exit(1);
	}

	writeKeys();
	cleanUp();

	return 0;
}
