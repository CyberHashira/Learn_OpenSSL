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

// generares keypair.
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

// prints PEM keypair.
void printKeys()
{
	PEM_write_PrivateKey(stdout, rsaKey, NULL, NULL, 0, NULL, NULL);
	PEM_write_PUBKEY(stdout, rsaKey);
}

// Writes the PEM keypair to a file.
void writeToFile()
{
	FILE *privateKey;
	FILE *publicKey;

	privateKey = fopen("rsa-private-key.pem", "w");
	publicKey = fopen("rsa-public-key.pem", "w");

	PEM_write_PrivateKey(privateKey, rsaKey, NULL, NULL, 0, NULL, NULL);
	PEM_write_PUBKEY(publicKey, rsaKey);

	fclose(privateKey);
	fclose(publicKey);
}

int main()
{
	char choice;
	generateKeyPair();
	printf("Print keys : (Yy/Nn) ? ");
	scanf(" %c", &choice);
	if(choice=='Y' || choice=='y')
		printKeys();
	writeToFile();
	EVP_PKEY_free(rsaKey);
	return 0;
}
