/*      This is a hobby project shared for learning purposes under the MIT License.
        Use the code freely, with no warranties or guarantees — @Cyber Hashira.

        OBJECTIVE:
        - This example code demonstrates how to generate random data using OpenSSL.
*/


#include <stdio.h>
#include <openssl/evp.h>

int main()
{
	unsigned char randomData[32];

	EVP_RAND_CTX *randCtx = NULL;
	randCtx = EVP_RAND_CTX_new(EVP_RAND_fetch(NULL, "CTR-DRBG", NULL), NULL);

	EVP_RAND_instantiate(randCtx, 32, 0, NULL, 0, NULL);
	EVP_RAND_generate(randCtx, randomData, sizeof(randomData), 0, 0, NULL, 0);
	EVP_RAND_CTX_free(randCtx);

	for(int ctr=0;ctr<32;ctr++)
	{
		printf("%02x", (unsigned char)randomData[ctr]);
	}
	printf("\n");

	return 0;
}
