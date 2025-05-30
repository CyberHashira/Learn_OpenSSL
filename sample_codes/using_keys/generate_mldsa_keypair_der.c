/*      This is a hobby project shared for learning purposes under the MIT License.
        Use the code freely, with no warranties or guarantees — @Cyber Hashira.

        OBJECTIVE:
        - This code generates DER encoded ML-DSA keypair and saves then to a file.
*/

#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>

char mldsaSize[7];
EVP_PKEY *mldsa =NULL;

// generates mldsa keypair.
void generateKeyPair()
{
	EVP_PKEY_CTX *mldsaCtx = NULL;

	// Gets the right mldsa context based on the selected variant.
	if(strcmp(mldsaSize, "mldsa44")==0)
		mldsaCtx = EVP_PKEY_CTX_new_from_name(NULL, "ML-DSA-44", NULL);
	else if(strcmp(mldsaSize, "mldsa65")==0)
		mldsaCtx = EVP_PKEY_CTX_new_from_name(NULL, "ML-DSA-65", NULL);
	else if(strcmp(mldsaSize, "mldsa87")==0)
		mldsaCtx = EVP_PKEY_CTX_new_from_name(NULL, "ML-DSA-87", NULL);
	else
	{
		fprintf(stderr, "Not a valid size for MLDSA.\n");
		exit(1);
	}

	EVP_PKEY_keygen_init(mldsaCtx);
	if(EVP_PKEY_keygen(mldsaCtx, &mldsa)!=1)
	{
		fprintf(stderr, "Failed to generated MLDSA keypair. Reason : %s.\n", ERR_error_string(ERR_get_error(), NULL));
		exit(1);
	}
	EVP_PKEY_CTX_free(mldsaCtx);
}

// writes mldsa keypair to a file.
void writeToFile()
{
	FILE *privateKey;
	FILE *publicKey;

	privateKey = fopen("mldsa-private-key.bin", "wb");
	publicKey = fopen("mldsa-public-key.bin", "wb");

	i2d_PrivateKey_fp(privateKey, mldsa);
	i2d_PUBKEY_fp(publicKey, mldsa);

	fclose(privateKey);
	fclose(publicKey);
}

int main()
{
	printf("MLDSA SIZE (mldsa44, mldsa65, mldsa87) : ");
	scanf("%s", mldsaSize);
	generateKeyPair();
	writeToFile();
	EVP_PKEY_free(mldsa);
	return 0;
}

