/*      This is a hobby project shared for learning purposes under the MIT License.
        Use the code freely, with no warranties or guarantees — @Cyber Hashira.

        OBJECTIVE:
        - This code generates PEM encoded EDDSA keypair and saves them to a file.
*/


#include <stdio.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/evp.h>

EVP_PKEY *eddsa = NULL;

// generates the keypair.
void generateKeyPair()
{
	EVP_PKEY_CTX *eddsaCtx = NULL;
	eddsaCtx = EVP_PKEY_CTX_new_from_name(NULL, "ED25519", NULL);
	EVP_PKEY_keygen_init(eddsaCtx);
	if(EVP_PKEY_keygen(eddsaCtx, &eddsa)!=1)
	{
		fprintf(stderr, "Failed to generate EDDSA keypair. Reason : %s.\n", ERR_error_string(ERR_get_error(), NULL));
		exit(1);
	}
	EVP_PKEY_CTX_free(eddsaCtx);
}

// writes the keypair to a file.
void writeToFile()
{
	FILE *privateKey = NULL;
	FILE *publicKey = NULL;

	privateKey = fopen("eddsa-private-key.pem", "w");
	publicKey = fopen("eddsa-public-key.pem", "w");

	PEM_write_PrivateKey(privateKey, eddsa, NULL, NULL, 0, NULL, NULL);
	PEM_write_PUBKEY(publicKey, eddsa);

	fclose(privateKey);
	fclose(publicKey);
}


int main()
{
	generateKeyPair();
	writeToFile();
	EVP_PKEY_free(eddsa);
	return 0;
}
