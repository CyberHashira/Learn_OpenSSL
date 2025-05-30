/*      This is a hobby project shared for learning purposes under the MIT License.
        Use the code freely, with no warranties or guarantees — @Cyber Hashira.

        OBJECTIVE:
        - This example code demonstrates how to generate random data using OpenSSL.
*/


#include <stdio.h>
#include <openssl/rand.h>

int main()
{
	static int SIZE = 32;
	unsigned char randomData[32];
	RAND_bytes(randomData, SIZE);

	for(int ctr=0;ctr<SIZE;ctr++)
	{
		printf("%02x", randomData[ctr]);
	}
	printf("\n");
	return 0;
}
