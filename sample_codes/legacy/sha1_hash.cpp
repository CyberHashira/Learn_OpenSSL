/*
	- Thanks for reading DISCLAIMER.txt
    This sample demonstrates how to calculate SHA1 message digest.
    This method of calculating hash is deprecated since OpenSSL 3.x and it is now a part of OpenSSL legacy API.
*/

#include <iostream>
#include <openssl/sha.h>

using namespace std;

int main()
{
	char text[] = "Hello World";
	unsigned char DIGEST[SHA_DIGEST_LENGTH];

	SHA_CTX sha;
	SHA1_Init(&sha);
	SHA1_Update(&sha, text, sizeof(text)-1);
	SHA1_Final(DIGEST, &sha);

	for(int ctr = 0; ctr< SHA_DIGEST_LENGTH; ctr++)
	{
		printf("%02x", DIGEST[ctr]);
	}
	cout << endl;
	return 0;
}


