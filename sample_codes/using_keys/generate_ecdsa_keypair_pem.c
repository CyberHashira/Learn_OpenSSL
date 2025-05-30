/*  	This is a hobby project shared for learning purposes under the MIT License.
     	Use the code freely, with no warranties or guarantees — @Cyber Hashira.

	OBJECTIVE:
	- This code generates PEM encoded, ECDSA keypair.
	- It also saves the key pair to a file after generating it.
*/

#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/ec.h>

EVP_PKEY *ecKey = NULL;
char curve_name[10];

// Generates keypair.
void generate_keypair()
{
	EVP_PKEY_CTX *ecCtx = NULL;
	ecCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
	EVP_PKEY_keygen_init(ecCtx);
	EVP_PKEY_CTX_set_ec_paramgen_curve_nid(ecCtx, OBJ_txt2nid(curve_name));

	if(EVP_PKEY_keygen(ecCtx, &ecKey)!=1)
	{
		fprintf(stderr, "Failed to generate EC keypair. Reason : %s.\n", ERR_error_string(ERR_get_error(),NULL));
		exit(1);
	}
	EVP_PKEY_CTX_free(ecCtx);
}

// Writes the keypair to a file.
void writeToFile()
{
	FILE *privateKey;
	FILE *publicKey;

	privateKey = fopen("ecdsa-private-key.pem", "w");
	publicKey = fopen("ecdsa-public-key.pem", "w");

	PEM_write_PrivateKey(privateKey, ecKey, NULL, NULL, 0, NULL, NULL);
	PEM_write_PUBKEY(publicKey, ecKey);

	fclose(privateKey);
	fclose(publicKey);
}

int main()
{
	printf ("EC Curve : ");
	scanf("%s", curve_name);
	generate_keypair();
	writeToFile();
	EVP_PKEY_free(ecKey);
	return 0;
}


