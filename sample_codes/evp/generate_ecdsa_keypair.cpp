#include <iostream>
#include <openssl/ec.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/pem.h>

using namespace std;

int main()
{
	EVP_PKEY_CTX *ecCtx = NULL;
	EVP_PKEY *ecKey = NULL;
	char curve_name[10];

	cout << "Enter curve 	: ";
	cin >> curve_name;

	ecCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
	EVP_PKEY_keygen_init(ecCtx);
	EVP_PKEY_CTX_set_ec_paramgen_curve_nid(ecCtx, OBJ_txt2nid(curve_name));

	if(EVP_PKEY_keygen(ecCtx, &ecKey)!=1)
	{
		cout << "Failed to generate RSA keypair." << ERR_error_string(ERR_get_error(),NULL) << endl;
		exit(1);
	}

	PEM_write_PrivateKey(stdout, ecKey, NULL, NULL, 0, NULL, NULL);
	//PEM_write_PUBKEY(stdout, ecKey);

	EVP_PKEY_free(ecKey);
	EVP_PKEY_CTX_free(ecCtx);
	EVP_cleanup();
	return 0;
}
