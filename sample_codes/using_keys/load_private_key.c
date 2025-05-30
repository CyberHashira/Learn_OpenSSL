/*      This is a hobby project shared for learning purposes under the MIT License.
        Use the code freely, with no warranties or guarantees — @Cyber Hashira.

        OBJECTIVE:
        - This samples demonstrates how to load a private key from a file.
*/


#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>

char *privateKeyFileName = NULL;
EVP_PKEY *privateKey = NULL;



// Loads private key from a file.
void loadPrivateKey()
{
	FILE *readFile;
	readFile = fopen(privateKeyFileName, "rb");
	if(!readFile)
	{
		fprintf(stderr, "Failed to read %s file.\n", privateKeyFileName);
		exit(1);
	}
	privateKey = PEM_read_PrivateKey(readFile, NULL, NULL, NULL);
	if(!privateKey)
	{
		fprintf(stderr, "Failed to load private key. Reason : %s.\n", ERR_error_string(ERR_get_error(), NULL));
		exit(1);
	}
	fclose(readFile);
	printf(" --> RSA keypair loaded successfully.\n");
}


// Displays information about the loaded key.
void displayKeyInfo()
{
	printf(" --> Key Information\n");
	//printf("      > Name : %s.\n", OBJ_nid2ln(EVP_PKEY_base_id(privateKey)));
	printf("      > Name : %s.\n", EVP_PKEY_get0_type_name(privateKey));
	printf("      > Bits : %d.\n", EVP_PKEY_get_bits(privateKey));
}


// Free allocated memory.
void freeMem()
{
	EVP_PKEY_free(privateKey);
}


// Prints the correct syntax for execution.
void usage(const char executableName[30])
{
	printf("\nUsage :\n");
	printf("%s private_key_file_name\n\n", executableName);
}



int main(int argc, char **argv[])
{
	if(argc<2)
	{
		usage((char*)argv[0]);
		exit(0);
	}
	privateKeyFileName = strdup((const char*)argv[1]);
	loadPrivateKey();
	displayKeyInfo();
	freeMem();
}
