/*      This is a hobby project shared for learning purposes under the MIT License.
        Use the code freely, with no warranties or guarantees — @Cyber Hashira.

        OBJECTIVE:
        - This example code demonstrates how to calculate shake256 digest using OpenSSL.
*/


#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>

int main()
{
	const char text[] = "Hello World";
	char digestAlg[] = "shake256"; // change this to shake128 or shake256
	size_t shakeDigestLen = 0;
	unsigned char *DIGEST = NULL;
	const EVP_MD *md; // Stores implementation for a message digest
	EVP_MD_CTX *mdCtx; // Stores the context for message digest

	// Changes the digest length based on the chosen shake variant.
	if(strcmp(digestAlg, "shake128")==0)
		shakeDigestLen = 32;
	else if(strcmp(digestAlg, "shake256")==0)
		shakeDigestLen = 64;

	DIGEST = (unsigned char*)malloc(shakeDigestLen);

	// SHAKE requires xoflen as a parameter.
	OSSL_PARAM shakeParam[] =
	{
		OSSL_PARAM_size_t("xoflen", &shakeDigestLen),
		OSSL_PARAM_END
	};

	md = EVP_get_digestbyname(digestAlg);
	if(!md)
	{
		fprintf(stderr, "%s algorithm not found.\n", digestAlg);
		exit(1);
	}

	mdCtx = EVP_MD_CTX_new();
	EVP_MD_CTX_set_params(mdCtx, shakeParam);
	if(mdCtx==NULL)
	{
		fprintf(stderr, "failed to initialize context.\n");
		exit(1);
	}

	EVP_DigestInit_ex(mdCtx, md, NULL);
	EVP_DigestUpdate(mdCtx, text, sizeof(text)-1);
	EVP_DigestFinalXOF(mdCtx, DIGEST, shakeDigestLen);

	printf("%s : ", digestAlg);
	for(int ctr=0; ctr<shakeDigestLen; ctr++)
	{
		printf("%02x", DIGEST[ctr]);
	}
	printf("\n");

	EVP_MD_CTX_free(mdCtx);
	free(DIGEST);

	return 0;
}
