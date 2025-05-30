/*      This is a hobby project shared for learning purposes under the MIT License.
        Use the code freely, with no warranties or guarantees — @Cyber Hashira.

        OBJECTIVE:
        - This example code demonstrates how to verify a digital signature using OpenSSL API.
        - It uses SHA-256 digest algorithm to for signature verification.
        - Depending on the public key algorithm used, the signature is verified using sha256 variant such as sha256WithRSA, sha256WithDSA, or sha256WithECDSA.
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

EVP_PKEY *verificationKey = NULL;


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

	verificationKey = PEM_read_PUBKEY(readFile, NULL, NULL, NULL);
	if(!verificationKey)
	{
		fprintf(stderr, "Failed to load public key. Reason : %s.\n", ERR_error_string(ERR_get_error(), NULL));
		exit(1);
	}
	fclose(readFile);
	free(publicKeyFileName);
	printf(" --> Public key loaded.\n");
}


// Verify signature
void verifySignature()
{
	EVP_MD_CTX *mdCtx = EVP_MD_CTX_new();
	EVP_DigestVerifyInit(mdCtx, NULL, EVP_sha256(), NULL, verificationKey);
	EVP_DigestVerifyUpdate(mdCtx, dataToVerify, dataLen);
	if(EVP_DigestVerifyFinal(mdCtx, signature, signatureLen)!=1)
	{
		fprintf(stderr, "Verification failed. Reason : %s.\n", ERR_error_string(ERR_get_error(), NULL));
		exit(1);
	}
	printf(" --> Signature verified successfully.\n");
	EVP_MD_CTX_free(mdCtx);
}


// Free allocated memory
void freeMem()
{
	free(dataToVerify);
	free(signature);
	EVP_PKEY_free(verificationKey);
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


