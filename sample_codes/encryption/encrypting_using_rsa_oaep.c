/*      This is a hobby project shared for learning purposes under the MIT License.
        Use the code freely, with no warranties or guarantees — @Cyber Hashira.

        OBJECTIVE:
	- This sample demonstrates how to encrypt a plaintext using rsa-oaep mechanism.
	- OAEP will default to sha1, mgf1, and no label.
        - It reads an RSA private key from a file and derives the corresponding public.
	- The public key is then used to encrypt the plaintext, and the private key is used to decrypt the ciphertext.
*/


#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/pem.h>


char *privateKeyFileName; // private key's filename.
EVP_PKEY *rsaKey = NULL; // stores private key.
const char message[] = "Hello World, I've been waiting for the chance to see your face."; // plaintext.
unsigned char *encrypted = NULL; size_t encLen; // ciphertext and its length.
unsigned char *decrypted = NULL; size_t decLen; // decrypted text and its length.
void freeMem();


// Reads private key from the specified file.
void loadPrivateKey()
{
	FILE *privateKey = NULL;
	privateKey = fopen(privateKeyFileName, "rb");
	if(!privateKey)
	{
		fprintf(stderr, "Failed to read %s file.\n", privateKeyFileName);
		exit(1);
	}
	rsaKey = PEM_read_PrivateKey(privateKey, NULL, NULL, NULL);
	if(!rsaKey)
	{
		fprintf(stderr, "Failed to load private key. Reason : %s. \n", ERR_error_string(ERR_get_error(), NULL));
		fclose(privateKey);
		freeMem();
		exit(1);
	}
	fclose(privateKey);
	free(privateKeyFileName);
	printf(" --> Private key loaded successfully.\n");
}



// Encrypts the plaintext.
void encryptData()
{
	int retVal = 0;
	EVP_PKEY_CTX *encCtx = EVP_PKEY_CTX_new(rsaKey, NULL); // new context for encryption.
	EVP_PKEY_encrypt_init(encCtx); //initialise encrypt operation.
	EVP_PKEY_CTX_set_rsa_padding(encCtx, RSA_PKCS1_OAEP_PADDING);
	EVP_PKEY_encrypt(encCtx, NULL, &encLen, (const char*)message, strlen(message)); // dummy encrypt to determine the required buffSize.
	encrypted = (unsigned char*)malloc(encLen);
	if(EVP_PKEY_encrypt(encCtx, encrypted, &encLen, (const char*)message, strlen(message))!=1)
	{
		fprintf(stderr, "Failed to encrypt. Reason : %s.\n", ERR_error_string(ERR_get_error(), NULL));
		EVP_PKEY_CTX_free(encCtx);
		freeMem();
		exit(1);
	}
	printf(" --> Plaintext encrypted.\n");
	EVP_PKEY_CTX_free(encCtx);
}



// Decrypts the ciphetext.
void decryptData()
{
	EVP_PKEY_CTX *decCtx = EVP_PKEY_CTX_new(rsaKey, NULL); // new context for decrypting ciphertext.
	EVP_PKEY_decrypt_init(decCtx);
	EVP_PKEY_CTX_set_rsa_padding(decCtx, RSA_PKCS1_OAEP_PADDING);
	EVP_PKEY_decrypt(decCtx, NULL, &decLen, encrypted, encLen); // dummy decrypt to determine the size of memory to allocate.
	decrypted = (unsigned char*)malloc(decLen);
	if(EVP_PKEY_decrypt(decCtx, decrypted, &decLen, encrypted, encLen)!=1)
	{
		fprintf(stderr, "Failed to decrypt. Reason : %s.\n", ERR_error_string(ERR_get_error(), NULL));
		EVP_PKEY_CTX_free(decCtx);
		freeMem();
		exit(1);
	}

	printf(" --> Ciphertext decrypted.\n");
	EVP_PKEY_CTX_free(decCtx);
}



// frees allocated memory
void freeMem()
{
	EVP_PKEY_free(rsaKey);
	free(encrypted);
	free(decrypted);
}



// Prints the syntax.
void usage(const char executableName[30])
{
	printf("\nUsage :\n");
	printf("%s private_key_file_name\n\n", executableName);
}



// Prints hex string.
void toHex(const char *data, size_t len)
{
	for(int ctr=0;ctr<len;ctr++)
	{
		printf("%02x", (unsigned char)data[ctr]);
	}
	printf("\n");
}



int main(int argc, char **argv[])
{
	if(argc<2)
	{
		usage((char*)argv[0]);
		exit(1);
	}
	privateKeyFileName = strdup((const char*)argv[1]);
	loadPrivateKey();
	encryptData();
	decryptData();

	printf("\n --> RESULTS");
	printf("\n   - Plaintext : %s.\n", message);
	printf("\n   - Plaintext (hex) : "); toHex(message, strlen(message));
	printf("\n   - Ciphertext (hex) : "); toHex(encrypted, encLen);
	printf("\n   - Decrypted (hex) : "); toHex(decrypted, decLen);

	freeMem();
	printf("\n");
	return 0;
}
