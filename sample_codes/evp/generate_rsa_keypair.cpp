#include <iostream>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/pem.h>

using namespace std;

int main()
{
	EVP_PKEY_CTX *rsaCtx = NULL;
	EVP_PKEY *rsaKeyPair = NULL;
	int keySize;

	cout << "Enter keysize 	: ";
	cin >> keySize;

	rsaCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
	EVP_PKEY_keygen_init(rsaCtx);
	EVP_PKEY_CTX_set_rsa_keygen_bits(rsaCtx, keySize);

	if(EVP_PKEY_keygen(rsaCtx, &rsaKeyPair)!=1)
	{
		cout << "Failed to generate RSA keypair." << ERR_error_string(ERR_get_error(),NULL) << endl;
		exit(1);
	}

	PEM_write_PrivateKey(stdout, rsaKeyPair, NULL, NULL, 0, NULL, NULL);
	PEM_write_PUBKEY(stdout, rsaKeyPair);

	EVP_PKEY_free(rsaKeyPair);
	EVP_PKEY_CTX_free(rsaCtx);
	EVP_cleanup();
	return 0;
}
