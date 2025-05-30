/*      This is a hobby project shared for learning purposes under the MIT License.
        Use the code freely, with no warranties or guarantees — @Cyber Hashira.

        OBJECTIVE:
        - This code generates DER encoded RSA keypair.
        - It also saves the key pair to a file after generating it.
*/

#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/pem.h>

EVP_PKEY *rsaKey = NULL;

// generates rsa keypair.
void generateKeyPair()
{
	EVP_PKEY_CTX *rsaCtx = NULL;
	int keySize;
	rsaCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
	printf("Enter Key Size : ");
	scanf("%d", &keySize);
	EVP_PKEY_keygen_init(rsaCtx);
	EVP_PKEY_CTX_set_rsa_keygen_bits(rsaCtx, keySize);
        if(EVP_PKEY_keygen(rsaCtx, &rsaKey)!=1)
        {
                fprintf(stderr, "Failed to generate RSA keypair. Reason: %s.\n", ERR_error_string(ERR_get_error(), NULL));
                exit(1);
        }
	EVP_PKEY_CTX_free(rsaCtx);
}

// writes the keypair to a file.
void writeToFile()
{
	FILE *privateKey;
	FILE *publicKey;

	privateKey = fopen("rsa-private-key.bin", "wb");
	publicKey = fopen("rsa-public-key.bin", "wb");

	i2d_PrivateKey_fp(privateKey, rsaKey);
	i2d_PUBKEY_fp(publicKey, rsaKey);

	fclose(privateKey);
	fclose(publicKey);
}

int main()
{
	char choice;
	generateKeyPair();
	writeToFile();
	EVP_PKEY_free(rsaKey);
	return 0;
}
