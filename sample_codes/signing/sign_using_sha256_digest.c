/*      This is a hobby project shared for learning purposes under the MIT License.
        Use the code freely, with no warranties or guarantees — @Cyber Hashira.

        OBJECTIVE:
        - This example code demonstrates how to sign a data using OpenSSL API.
	- It uses SHA-256 digest algorithm to calculate the digest.
	- Depending on the private key used, the resulting signature will be a sha256 variant such as sha256WithRSA, sha256WithDSA or sha256WithECDSA.
	- Both the data to be signed and the the signing key are read from file.
	- Files names are passed as arguments to the executable.
*/

#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/pem.h>

char *fileToSign = NULL;
char *privateKeyFileName = NULL;
long fileSize = 0;
EVP_PKEY *signingKey = NULL;
unsigned char *dataToSign = NULL;
unsigned char *signature = NULL;
long sigLen = 0;


// Reads the file to sign
void readFileToSign()
{
	FILE *readFile;

	readFile = fopen(fileToSign, "rb");
	if(!readFile)
	{
		fprintf(stderr, "Failed to read %s.\n", fileToSign);
		exit(1);
	}

	fseek(readFile, 0, SEEK_END);
	fileSize = ftell(readFile);
	rewind(readFile);

	if(fileSize > 32768)
	{
		fprintf(stderr, "This sample supports file of upto 32KB size.\n");
		fclose(readFile);
		exit(1);
	}
	dataToSign = (unsigned char*)malloc(fileSize);
	fread(dataToSign, sizeof(unsigned char), fileSize, readFile);
	fclose(readFile);
	printf(" --> File to sign read.\n");

}


// Reads the private key.
void readPrivateKey()
{
	FILE *readFile;

	readFile = fopen(privateKeyFileName, "rb");
	if(!readFile)
	{
		fprintf(stderr, "Failed to read %s file.\n", privateKeyFileName);
		exit(1);
	}

	signingKey = PEM_read_PrivateKey(readFile, NULL, NULL, NULL);
	if(!signingKey)
	{
		fprintf(stderr, "Failed to load private key. Reason : %s.\n", ERR_error_string(ERR_get_error(), NULL));
		exit(1);
	}
	fclose(readFile);
	free(privateKeyFileName);
	printf(" --> Signing key loaded.\n");
}


// signs the data
void signData()
{
	EVP_MD_CTX *mdCtx = EVP_MD_CTX_new();
	EVP_DigestSignInit(mdCtx, NULL, EVP_sha256(), NULL, signingKey);
	EVP_DigestSignUpdate(mdCtx, dataToSign, fileSize);
	EVP_DigestSignFinal(mdCtx, NULL, &sigLen);
	signature = (unsigned char*)malloc(sigLen);
	EVP_DigestSignFinal(mdCtx, signature, &sigLen);
	printf(" --> Data signed.\n");
	EVP_MD_CTX_free(mdCtx);
}


// Writes signature to a file.
void writeSignatureToFile()
{
	FILE *fileWrite;
	char *signatureFileName = NULL;
	size_t fileNameLen = strlen(fileToSign);
	size_t fileExtensionLen = 4;

	signatureFileName = (char*)malloc(fileNameLen + fileExtensionLen);
	strncpy(signatureFileName, fileToSign, fileNameLen);
	strncat(signatureFileName, ".sig", fileExtensionLen);

	fileWrite = fopen(signatureFileName, "wb");
	fwrite(signature, sizeof(unsigned char), sigLen, fileWrite);
	fclose(fileWrite);
	printf(" --> Signature written to %s.\n", signatureFileName);
	free(signatureFileName);
	free(fileToSign);
	free(signature);
	free(dataToSign);
	EVP_PKEY_free(signingKey);
}


// prints the usage.
void usage(const char *executableName)
{
	printf("\nUsage :-\n");
	printf("%s <file_to_sign> <PEM_encoded_private_key>\n\n", executableName);
}


int main(int argc, char **argv[])
{
	if(argc<3)
	{
		usage((const char*)argv[0]);
		exit(0);
	}

	fileToSign = strdup((const char*)argv[1]);
	privateKeyFileName = strdup((const char*)argv[2]);

	readFileToSign();
	readPrivateKey();
	signData();
	writeSignatureToFile();

	printf("\n");
	return 0;
}
