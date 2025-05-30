/*      This is a hobby project shared for learning purposes under the MIT License.
        Use the code freely, with no warranties or guarantees — @Cyber Hashira.

        OBJECTIVE:
        - This example code demonstrates how to verify a digital signature using OpenSSL API.
        - It uses ML-DSA-{44,65,87} algorithm for signature verification.
        - The data, the signature, and the the public key are read from file.
        - File names are passed as arguments to the executable.
*/

#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/pem.h>

char *signatureFileName = NULL;
char *dataFileName = NULL;
char *publicKeyFileName = NULL;

unsigned char *signature = NULL;
unsigned char *dataToVerify = NULL;
long signatureLen, dataLen = 0;

EVP_PKEY *publicKey = NULL;
char mldsaVariant[10];


// Reads the file containing signature
void readSignatureFile()
{
	FILE *readFile;
	long fileSize;

	readFile = fopen(signatureFileName, "rb");
	if(!readFile)
	{
		fprintf(stderr, "Failed to read %s.\n", signatureFileName);
		exit(1);
	}

	fseek(readFile, 0, SEEK_END);
	fileSize = ftell(readFile);
	rewind(readFile);

	signature = (unsigned char*)malloc(fileSize);
	fread(signature, sizeof(unsigned char), fileSize, readFile);
	fclose(readFile);
	signatureLen = fileSize;
	printf(" --> Signature read.\n");
	free(signatureFileName);
}


// Reads the file containing the data to verify.
void readFileToVerify()
{
	FILE *readFile;
	long fileSize;

	readFile = fopen(dataFileName, "rb");
	if(!readFile)
	{
		fprintf(stderr, "Failed to read %s.\n", dataFileName);
		exit(1);
	}

	fseek(readFile, 0, SEEK_END);
	fileSize = ftell(readFile);
	rewind(readFile);

	dataToVerify = (unsigned char*)malloc(fileSize);
	fread(dataToVerify, sizeof(unsigned char), fileSize, readFile);
	fclose(readFile);
	dataLen = fileSize;
	printf(" --> Data to verify read.\n");
	free(dataFileName);
}


// Reads the public key
void readPublicKey()
{
	FILE *readFile;

	readFile = fopen(publicKeyFileName, "rb");
	if(!readFile)
	{
		fprintf(stderr, "Failed to read %s.\n", publicKeyFileName);
		exit(1);
	}

	publicKey = PEM_read_PUBKEY(readFile, NULL, NULL, NULL);
	if(!publicKey)
	{
		fprintf(stderr, "Failed to load public key. Reason : %s.\n", ERR_error_string(ERR_get_error(), NULL));
		exit(1);
	}
	fclose(readFile);
	free(publicKeyFileName);
	strncpy(mldsaVariant, EVP_PKEY_get0_type_name(publicKey), strlen(EVP_PKEY_get0_type_name(publicKey)));
	printf(" --> Public key loaded.\n");
}


// Verify signature
void verifySignature()
{
	EVP_PKEY_CTX *mldsaCtx = EVP_PKEY_CTX_new_from_pkey(NULL, publicKey, NULL);
	EVP_SIGNATURE *sigAlg = EVP_SIGNATURE_fetch(NULL, mldsaVariant, NULL);
	const OSSL_PARAM mldsaParam[] =
	{
		OSSL_PARAM_octet_string("context-string", (unsigned char*)"", 0),
		OSSL_PARAM_END
	};

	EVP_PKEY_verify_message_init(mldsaCtx, sigAlg, mldsaParam);
	if(EVP_PKEY_verify(mldsaCtx, signature, signatureLen, dataToVerify, dataLen)!=1)
	{
		fprintf(stderr, "Signature verification failed. Reason : %s.\n", ERR_error_string(ERR_get_error(), NULL));
		exit(1);
	}
	printf(" --> Signatured verified successfully.\n");
	EVP_PKEY_CTX_free(mldsaCtx);
}


// Free allocated memory
void freeMem()
{
	free(dataToVerify);
	free(signature);
	EVP_PKEY_free(publicKey);
}


// Prints the syntax.
void usage(const char *executableName)
{
	printf("\nUsage:-\n");
	printf("%s <data_file> <signature_file> <public_key_file>\n\n", executableName);
}


int main(int argc, char **argv[])
{
	if(argc<4)
	{
		usage((const char*)argv[0]);
		exit(0);
	}

	dataFileName = strdup((const char*)argv[1]);
	signatureFileName = strdup((const char*)argv[2]);
	publicKeyFileName = strdup((const char*)argv[3]);

	readFileToVerify();
	readSignatureFile();
	readPublicKey();
	verifySignature();

	freeMem();
	printf("\n");
	return 0;
}


