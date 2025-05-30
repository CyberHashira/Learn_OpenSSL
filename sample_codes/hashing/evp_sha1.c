/*      This is a hobby project shared for learning purposes under the MIT License.
        Use the code freely, with no warranties or guarantees — @Cyber Hashira.

        OBJECTIVE:
        - This example code demonstrates how to calculate sha1 digest using OpenSSL.
*/



#include <stdio.h>
#include <openssl/evp.h>

int main()
{
	const EVP_MD *md; // Stores implementation for a message digest
	EVP_MD_CTX *mdCtx; // Stores the context for message digest

	char text[] = "Hello World"; // text to digest
	unsigned char DIGEST[EVP_MAX_MD_SIZE]; // to store digest.
	unsigned int hashLen; // digest length.

	md = EVP_get_digestbyname("sha1");

	if(!md)
	{
		printf("sha1 algorithm not found.");
		exit(1);
	}

	mdCtx = EVP_MD_CTX_new();
	EVP_DigestInit_ex(mdCtx, md, NULL);
	EVP_DigestUpdate(mdCtx, text, sizeof(text)-1);
	EVP_DigestFinal(mdCtx, DIGEST, &hashLen);

	for(int ctr=0; ctr<hashLen; ctr++)
	{
		printf("%02x", DIGEST[ctr]);
	}

	printf("\n");
	return 0;
}
